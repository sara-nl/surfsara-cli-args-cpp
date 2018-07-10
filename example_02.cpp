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
  std::string strValue = "default";
  std::vector<int> values;
  std::vector<std::string> args;

  // a simple flag
  parser.addFlag('h', "help",
                 Cli::Doc("show help"));

  // a flag that can be passed multiple time
  Cli::Doc verboseDoc("verbose output.\n"
                      "The flag can be used multiple times to increase verbosity");
  auto verbose = parser.addMultipleFlag('v', "verbose", verboseDoc);
  
  // a string valued argument
  Cli::Doc strDoc("An arbitrary string value.");
  parser.addValue<std::string>(strValue, 's', "str", strDoc);

  Cli::Doc multiDoc("Integer values that can be passed multiple times");
  parser.addMultipleValue<int>(values, 'm', "multivalue", multiDoc);

  // positional Args
  Cli::Doc positionalDoc("Unnamed arguments");
  parser.addPositionalMultipleValue<std::string>(args,
                                                 "ARGS..",
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
  if(parser.isSet("help"))
  {
    parser.printHelp(std::cout);
    return 0;
  }
  std::cout << "verboseFlag:    " << verbose->numSet() << std::endl;
  std::cout << "strArg:         " << strValue << std::endl;
  std::cout << "multiArg:       ";
  for(auto str : values)
  {
    std::cout << str << " ";
  }
  std::cout << std::endl;
  std::cout << "positionalArgs: ";
  for(auto str : args)
  {
    std::cout << str << " ";
  }
  std::cout << std::endl;

  return 0;
}
