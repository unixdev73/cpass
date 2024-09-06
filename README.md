# CPASS - A simple CLI random password generator

## Building

There are no dependencies that have to be met,
other than a functional c++ environment (at least c++17).

```console
cmake -B build
make -C build
```

The generated executable is called 'cpass',
and it can be found in ./build/src/.

## Installing

```console
make -C build install
```

By default, the executable will be installed to /usr/local/bin,
so you might need root privilages.
The destination can be modified using the CMAKE\_INSTALL\_PREFIX variable
during the configuration stage.

## Usage

By default, executing cpass with no arguments will
generate a 10 digit password from hexadecimal characters.
(same as using \-\-hex \-\-len=10).

In general, cpass generates a password from a source\_string.
If no password length is specified, the generated password
will be of the same length as the source string.
In other words, the generated password
will simply be the source string shuffled.

The source\_string can be appended manually, as well as by using
the following presets:

- \-\-hex

This flag appends all the characters from "abcdefABCDEF0123456789"
to the source string, if they are not a part of it.

- \-\-hex\-lcase

This flag appends all the characters from "abcdef0123456789"
to the source string, if they are not a part of it.

- \-\-hex\-ucase

This flag appends all the characters from "ABCDEF0123456789"
to the source string, if they are not a part of it.

- \-\-alnum

This flag appends all the characters from
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
to the source string, if they are not a part of it.

- \-\-alnum\-lcase

This flag appends all the characters from
"abcdefghijklmnopqrstuvwxyz0123456789"
to the source string, if they are not a part of it.

- \-\-alnum\-ucase

This flag appends all the characters from
"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
to the source string, if they are not a part of it.

- \-\-alpha

This flag appends all the characters from
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
to the source string, if they are not a part of it.

- \-\-alpha\-lcase

This flag appends all the characters from
"abcdefghijklmnopqrstuvwxyz"
to the source string, if they are not a part of it.

- \-\-alpha\-ucase

This flag appends all the characters from
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
to the source string, if they are not a part of it.

- \-\-src

This option appends characters set as its value to the source string,
regardless of whether they part of the source string.

- \-\-len

This option specifies the length of the password.
It also changes the method of password generation.
If this option is not specified, then the resulting password
will simply be the source string shuffled.

- \-\-verbose, \-v

This flag prints what options / flags have been applied,
as well as the final source string and the password length.

- \-\-version

Prints the name and version of the utility.

## Examples

To generate a password that must include some characters use the \-\-src
option.

```console
cpass --src="abcd1234,_+"
```

Example output:

```console
12b,a3+c4_d
```

As previously specified, this simply shuffles the source string.

To generate a random password of a given length from only lower case
hexadecimal characters, and some custom characters, the following could work.

```console
cpass --len=10 --src=".,_" --hex
```

Example output:

```console
,,_d52bC,,
```

Sometimes, the generated random password does not meet some criteria,
for example, that the ',' character may only occur once.

This can be solved using the \-\-src no \-\-len method, but it can also be
solved with a simple script using grep:

```console
while [[ "${r}" != "0" ]]; do ps="$(cpass --len=10 --src=".,_" --hex)"; echo "${ps}" | grep -qE "^[^,]*,[^,]*$"; r="$?"; done; r="1"; echo "${ps}";
```
