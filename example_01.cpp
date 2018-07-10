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
