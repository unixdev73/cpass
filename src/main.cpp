#include "version.hpp"
#include <algorithm>
#include <cctype>
#include <concepts>
#include <iostream>
#include <random>
#include <regex>
#include <string>

namespace cp {
enum class operation_t {
  permutation, // Simple shuffle, the output is of the same length as the input
  variation    // A random selection from the source string of a specified size
};

struct data_t {
  operation_t source_method = operation_t::permutation;
  std::string source_string;
  std::size_t passwd_length = 0;
  bool verbose = false;
  bool print_version = false;
};

data_t handle_input(int, char **);

std::size_t mkrand(const std::string &s) {
  static std::mt19937 twister{std::random_device{}()};
  std::uniform_int_distribution<std::size_t> d(0, s.size() - 1);
  return d(twister);
}
} // namespace cp

int main(int argc, char *argv[]) {
  try {
    auto d = cp::handle_input(argc, argv);
    std::string password{};

    if (d.print_version) {
      std::cout << "cpass " << MCR_CPASS_VERSION << std::endl;
      return 0;
    }

    switch (d.source_method) {
    case cp::operation_t::permutation: {
      std::mt19937 t{std::random_device{}()};
      password = d.source_string;
      std::shuffle(password.begin(), password.end(), t);
      break;
    }
    case cp::operation_t::variation: {
      for (decltype(d.passwd_length) i = 0; i < d.passwd_length; ++i)
        password += d.source_string[cp::mkrand(d.source_string)];
      break;
    }
    }

    if (d.verbose)
      std::cout << "password: ";
    std::cout << password << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Caught exception: " << e.what() << std::endl;
    return 1;

  } catch (...) {
    std::cerr << "Caught unknown exception." << std::endl;
    return 2;
  }

  return 0;
}

namespace cp {
void append_if_unique(std::string &s, std::string d) {
  for (auto c : d)
    if (s.find(c) == std::string::npos)
      s += c;
}

template <typename Logger>
bool handle_hex(const std::string &f, data_t &d, Logger l) {
  if (f == "--hex") {
    l("Adding hexadecimal chars to source string.");
    append_if_unique(d.source_string, "abcdefABCDEF0123456789");
    return true;
  }

  if (f == "--hex-ucase") {
    l("Adding upper case hexadecimal chars to source string.");
    append_if_unique(d.source_string, "ABCDEF0123456789");
    return true;
  }

  if (f == "--hex-lcase") {
    l("Adding lower case hexadecimal chars to source string.");
    append_if_unique(d.source_string, "abcdef0123456789");
    return true;
  }
  return false;
}

template <typename Logger>
bool handle_alnum(const std::string &f, data_t &d, Logger l) {
  if (f == "--alnum") {
    l("Adding alnum chars to source string.");
    append_if_unique(d.source_string, "0123456789");
    append_if_unique(d.source_string, "abcdefghijklmnopqrstuvwxyz");
    append_if_unique(d.source_string, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    return true;
  }

  if (f == "--alnum-ucase") {
    l("Adding upper case alnum chars to source string.");
    append_if_unique(d.source_string, "0123456789");
    append_if_unique(d.source_string, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    return true;
  }

  if (f == "--alnum-lcase") {
    l("Adding lower case alnum chars to source string.");
    append_if_unique(d.source_string, "0123456789");
    append_if_unique(d.source_string, "abcdefghijklmnopqrstuvwxyz");
    return true;
  }
  return false;
}

template <typename Logger>
bool handle_alpha(const std::string &f, data_t &d, Logger l) {
  if (f == "--alpha") {
    l("Adding alpha chars to source string.");
    append_if_unique(d.source_string, "abcdefghijklmnopqrstuvwxyz");
    append_if_unique(d.source_string, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    return true;
  }

  if (f == "--alpha-ucase") {
    l("Adding upper case alpha chars to source string.");
    append_if_unique(d.source_string, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    return true;
  }

  if (f == "--alpha-lcase") {
    l("Adding upper case alpha chars to source string.");
    append_if_unique(d.source_string, "abcdefghijklmnopqrstuvwxyz");
    return true;
  }
  return false;
}

template <typename Logger>
bool handle_custom_source(const std::string &f, data_t &d, Logger l) {
  if (std::string s{"--src="}; std::regex_search(f, std::regex{s + ".*"})) {
    auto src = f.substr(s.size(), f.size() - s.size());
    if (!src.size())
      throw std::runtime_error{"A custom source string cannot be empty!"};
    l("Adding '" + src + "' to source string.");
    d.source_string += src;
    return true;
  }
  return false;
}

template <typename Logger>
bool handle_passwd_length(const std::string &f, data_t &d, Logger l) {
  if (std::string s{"--len="}; std::regex_search(f, std::regex{s + ".*"})) {
    auto src = f.substr(s.size(), f.size() - s.size());
    l("Setting the password length to '" + src + "'.");
    if (!src.size())
      throw std::runtime_error{"The length option requires a value!"};

    try {
      d.passwd_length = std::stoul(src);
    } catch (...) {
      throw std::runtime_error{"The length requires a numerical value!"};
    }

    if (!d.passwd_length)
      throw std::runtime_error{"The length cannot be 0!"};
    d.source_method = operation_t::variation;
    return true;
  }
  return false;
}

void handle_field_initialization(std::string f, data_t &d) {
  auto log_if = [v = d.verbose](std::string str) {
    if (v)
      std::cout << str << std::endl;
  };

  if (handle_hex(f, d, log_if))
    return;
  if (handle_alnum(f, d, log_if))
    return;
  if (handle_alpha(f, d, log_if))
    return;
  if (handle_custom_source(f, d, log_if))
    return;
  if (handle_passwd_length(f, d, log_if))
    return;
  if (f == "--verbose" || f == "-v")
    return;
  if (f == "--version") {
    d.print_version = true;
    return;
  }

  throw std::runtime_error{"Unknown parameter: '" + f + "'."};
}
} // namespace cp

namespace cp {
void use_defaults(data_t &p) {
  p.source_string = "abcdefABCDEF0123456789";
  p.source_method = operation_t::variation;
  p.passwd_length = 10;
}

data_t handle_input(int c, char **v) {
  data_t p{};

  if (c > 1) {
    for (int i = 1; i < c; ++i)
      if (auto f = std::string{v[i]}; f == "--verbose" || f == "-v") {
        p.verbose = true;
        break;
      }

    for (int i = 1; i < c; ++i)
      handle_field_initialization(v[i], p);

    if (p.print_version)
      return p;
    if (!p.source_string.size())
      throw std::runtime_error{"The source string cannot be empty!"};
    if (!p.passwd_length)
      p.passwd_length = p.source_string.size();
  } else
    use_defaults(p);

  if (p.verbose) {
    std::cout << "source string = \"" << p.source_string << "\"\n";
    std::cout << "passwd length = " << p.passwd_length << std::endl;
  }
  return p;
}
} // namespace cp
