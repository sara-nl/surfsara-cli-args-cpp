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
  std::vector<std::string> args;

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
  parser.add(Cli::MultipleValue<std::string>::make(args,
                                                   Cli::Doc("Unnamed arguments")));

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
