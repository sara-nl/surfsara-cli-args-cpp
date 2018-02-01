# CliArgs
CliArgs is a library for parsing command line arguments.
Since it is a header-only library without any other dependency, it can be easily
included in any C++11 project.

## Rquirements
A c++ compiler with c++11 standard. The following compilers have been tested:
* gcc 5.4.0
* clang 5.0
* Visual C++ 2017

## Building the example application
The following program illustrates the basic usage of the program.
```cpp
#include <cli.h>
#include <vector>
#include <string>

int main(int argc, const char ** argv)
{
  Cli::Parser parser("example",
		     "This example program illustrates the usage of the CliArgs library.\n"
		     "It uses a\n"
		     "- Cli::Flag,\n"
		     "- Cli::MultipleFlag,\n"
		     "- Cli::Value and \n"
		     "- Cli::Multivalue arguments.");
  std::string str_value = "default";
  std::vector<int> values;
  parser.add(Cli::Flag::make('h', "help",
                             Cli::Doc("show help")));
  parser.add(Cli::MultipleFlag::make('v', "verbose",
                                     Cli::Doc("verbose output.\n"
                                              "The flag can be used multiple times to increase verbosity")));
  parser.add(Cli::Value<std::string>::make(str_value,
                                           's', "str",
                                           Cli::Doc("An arbitrary string value.")));
  parser.add(Cli::MultipleValue<int>::make(values,
                                           'm', "multivalue",
                                           Cli::Doc("Integer values "
                                                    "that can be passed multiple times")));

  std::vector<std::string> err;
  if(!parser.parse(argc, argv, err))
  {
    for(auto line : err)
    {
      std::cerr << line << std::endl;
    }
    parser.printHelp(std::cerr);
    return 8;
  }
  if(parser.isSet("help"))
  {
    parser.printHelp(std::cout);
    return 0;
  }
  return 0;
}

```
## Running unit tests
While the library itself does not depend on any third party component, the unit test
suite makes use of Catch2. To compile and run the tests, update the Catch2 submodule
```
   git submodule update --init --recursive
```
and compile the sources in tests. For unix-like build systems a makefile is provided.


