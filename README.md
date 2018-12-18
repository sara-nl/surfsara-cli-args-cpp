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
The following program illustrates the basic usage of the library.
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
  // a simple flag
  auto helpFlag = parser.addFlag('h', "help",
                                 Cli::Doc("show help"));

  // a flag that can be passed multiple time
  Cli::Doc verboseDoc("verbose output.\n"
                      "The flag can be used multiple times to increase verbosity");
  auto verboseFlag = parser.addMultipleFlag('v', "verbose", verboseDoc);

  // a string valued argument
  Cli::Doc strDoc("An arbitrary string value.");
  auto strArg =  parser.addValue<std::string>('s', "str", strDoc);

  Cli::Doc multiDoc("Integer values that can be passed multiple times");
  auto multiArg = parser.addMultipleValue<int>('m', "multivalue", multiDoc);

  // positional Args
  Cli::Doc positionalDoc("Unnamed arguments");
  auto positionalArgs = parser.addPositionalMultipleValue<std::string>("ARGS..",
                                                                       positionalDoc);

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
  if(helpFlag->isSet())
  {
    parser.printHelp(std::cout);
    return 0;
  }

  std::cout << "helpFlag:       " << helpFlag->isSet() << std::endl;
  std::cout << "verboseFlag:    " << verboseFlag->numSet() << std::endl;
  std::cout << "strArg:         " << strArg->getValue() << std::endl;
  std::cout << "multiArg:       ";
  for(auto str : multiArg->getValue())
  {
    std::cout << str << " ";
  }
  std::cout << std::endl;
  std::cout << "positionalArgs: ";
  for(auto str : positionalArgs->getValue())
  {
    std::cout << str << " ";
  }
  std::cout << std::endl;

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


