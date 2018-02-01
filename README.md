# CliArgs
CliArgs is a library for parsing command line arguments.
Since it is a header-only library without any other dependency, it can be easily
included in any C++11 project.

## Rquirements
A c++ compiler with c++11 standard. The following compilers have been tested:
* gcc 5.4.0
* clang 5.0
*

## Building the example application

## Running unit tests
While the library itself does not depend on any third party component, the unit test
suite makes use of Catch2. To compile and run the tests, update the Catch2 submodule
```
   git submodule update --init --recursive
```
and compile the sources in tests. For unix-like build systems a makefile is provided.


