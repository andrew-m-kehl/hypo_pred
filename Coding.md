Coding Conventions
==================

API
---

### Constructors and Destructors

Mark single-argument constructors as `explicit` to disable implicit conversions.

Do not make constructors that accept one universal reference or a variadic list
of universal references. These constructors will take precedence over copy and
move constructors.

Mark move constructors as `noexcept` when possible to allow `std::vector` move
elements when resizing.

When declaring a virtual destuctor for a base class, when possible, add
defaulted move (and consequently - copy) constructor and assignment (especially
if the destructor of the base class does nothing). Otherwise, move constructor
and assignment will be disabled.

### Collections

For collections backed by an array, provide access by index.

For new collections, when possible, provide API that uses `int` for indices and
sizes, especially if the collection is not expected to have billons of elements.
This allows the user to use `int` for indices without compiler warnings, and
signed indices are much more comfortable to use.

### Runtime Checks and Exceptions

Checks that should not normally fail, enable only in debug mode, by enclosing
them in `#ifndef NDEBUG ... #endif`.

To signal a failed assertion, throw an exception instead of using `assert()`,
`exit()`, etc. An exception interacts correctly with tests; it fails the current
test case and allows other tests to execute, while `assert()` and `exit()`
interrupt the execution of tests.

Shared Libraries and DLLs
-------------------------

Avoid deriving from class templates without a good reason (CRTP is a good
reason). MSVC will automatically export the instantiation of the base class from
the DLL which may cause link errors later, for an example, see
[here](https://www.codesynthesis.com/~boris/blog/2010/01/18/dll-export-cxx-templates/).

### Windows

Disable warnings 4251 (using non-exported class as return type or data member)
and 4275 (using non-exported base class) when they are related to usage of
standard library types or to class templates. Disable them per class using
`pragma warning(disable: ...) ... pragma warning(default: ...)`.

Avoid using `CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS` in CMake, it may cause builds
to fail for no apparent reason.


Text Encoding
-------------

Internally, store text in utf-8 encoding using `char` data type (`char *`,
`std::string`, etc).

Encode the source files in ascii. Use the special syntax available in C++
(`u8"..."` and `\u...`) to create utf-8 string literals.

Unless explicitly asked otherwise, assume text input to be encoded in utf-8
and write text output in utf-8 encoding. In particular, by default write utf-8
to stdout. In Windows, users can make `cmd` display utf-8 text (see below). They
cannot input utf-8 text from the console, but this is an acceptable loss.

Do not directly use standard functions that will not work properly with utf-8
strings in Windows, e.g., `fopen`. Instead, use a(n existing or a new wrapper)
that will convert the string to utf-16 and call the 'wide' version of the
function.

Do not write your code directly in `main` or `wmain`.
Instead, use conditional compilation as follows.
In Windows, use `wmain` as the entry point, convert the command line arguments
to utf-8 and then call your code.
In Linux, use `main` as the entry point and then call your code.

### Windows

In Windows, users can make `cmd` display utf-8 text by selecting a non-raster
font (Right click on the `cmd` window title, `Properties -> Font`, select a font
with a 'TT' icon, e.g., 'Lucida Console') and setting the codepage to utf-8 with
`chcp 65001`. Input of utf-8 text does from the console does not work correctly,
but redirecting from a file works. A custom solution may be needed, that would
detect when stdin is attached to console, and that call console IO functions
instead of stdio functions.

To display utf-8 correctly, `cmd` needs to receive complete glyphs from the
application. If it receives an incomplete glyph and then its continuation, it
won't display it properly. Because of that:

- When writing to stdout do not write bytes one by one, e.g., do not use `fputc`.
  Instead, use `fputs`, `printf`, etc.

- Do not use `std::cout` to write to stdout, instead use `fputs`, `printf`, etc.
  Microsoft's implementation of `filebuf` writes the bytes to the underlying
  file one by one.

### Linux

In Linux, assume that all text input (command line arguments, text files, etc)
is encoded in utf-8, unless explicitly instructed otherwise.


Input-Output
------------

Do not use C++ streams for file and console output. In windows, they may be slow
and they break the display of utf-8 in `cmd`. For reference, see the now deleted
bug report at MS Connect:
[here](https://web.archive.org/web/20170329163751/https://connect.microsoft.com/VisualStudio/feedback/details/642876/std-wcout-is-ten-times-slower-than-wprintf-performance-bug-in-c-library).

When defining a template stream output `operator<<` that works with an arbitrary
stream type, also define the output operator for `std::ostream`.
Otherwise, the code will not compile when used together with Googletest.

Misc
----

Don't use the `#pragma once` directive, instead use the
`#ifndef ... #define ... #endif` sequence. The `#pragma once` may not work
correctly if the same file is accessed with different names.
