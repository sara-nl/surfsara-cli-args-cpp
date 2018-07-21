/*****************************************************************************
MIT License

Copyright (c) 2018 stefan-wolfsheimer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/

#include <sstream>
#include <catch2/catch.hpp>
#include <cli_flag.h>

using Flag = Cli::Flag;
using Argument = Cli::Argument;

static std::string getHelp(std::shared_ptr<Argument> arg)
{
  std::stringstream buff;
  arg->printHelp(buff, 0);
  return buff.str();
}

TEST_CASE("flag-constructor-with-name", "[Flag]")
{
  REQUIRE(Flag::make("name")->isFlag());
  REQUIRE(Flag::make("name")->getName() == "name");
  REQUIRE(Flag::make("name")->getShortName() == '\0');
  REQUIRE_FALSE(Flag::make("name")->isSet());
}

TEST_CASE("flag-constructor-with-short-name", "[Flag]")
{
  REQUIRE(Flag::make('n')->isFlag());
  REQUIRE(Flag::make('n')->getName() == "");
  REQUIRE(Flag::make('n')->getShortName() == 'n');
  REQUIRE_FALSE(Flag::make('n')->isSet());
}

TEST_CASE("flag-constructor-with-name-and-short-name", "[Flag]")
{
  REQUIRE(Flag::make('n', "name")->isFlag());
  REQUIRE(Flag::make('n', "name")->getName() == "name");
  REQUIRE(Flag::make('n', "name")->getShortName() == 'n');
  REQUIRE_FALSE(Flag::make('n', "name")->isSet());
}

TEST_CASE("flag-constructor-with-name-and-doc", "[Flag]")
{
  std::string docstring("documentation\nstring");
  REQUIRE(Flag::make("name", Cli::Doc(docstring))->isFlag());
  REQUIRE(Flag::make("name", Cli::Doc(docstring))->getName() == "name");
  REQUIRE(Flag::make("name", Cli::Doc(docstring))->getShortName() == '\0');
  REQUIRE_FALSE(Flag::make("name", Cli::Doc(docstring))->isSet());
  REQUIRE(getHelp(Flag::make("name", Cli::Doc(docstring))) == 
          "--name  documentation\n"
          "        string\n");
}

TEST_CASE("flag-constructor-with-short-name-and-doc", "[Flag]")
{
  REQUIRE(Flag::make('n', Cli::Doc("documentation string"))->isFlag());
  REQUIRE(Flag::make('n', Cli::Doc("documentation string"))->getName() == "");
  REQUIRE(Flag::make('n', Cli::Doc("documentation string"))->getShortName() == 'n');
  REQUIRE_FALSE(Flag::make('n', Cli::Doc("documentation string"))->isSet());
}

TEST_CASE("flag-constructor-with-name-short-name-and-doc", "[Flag]")
{
  REQUIRE(Flag::make('n', "name", Cli::Doc("documentation string"))->isFlag());
  REQUIRE(Flag::make('n', "name", Cli::Doc("documentation string"))->getName() == "name");
  REQUIRE(Flag::make('n', "name", Cli::Doc("documentation string"))->getShortName() == 'n');
  REQUIRE_FALSE(Flag::make('n', "name", Cli::Doc("documentation string"))->isSet());
}

TEST_CASE("flag-parse", "[Flag]")
{
  auto flag = Flag::make("name");
  int argc;
  const char * argv[] = { "progr" };
  std::vector<std::string> err;
  int i = 0;
  REQUIRE(flag->parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE(err.empty());
  REQUIRE(flag->isSet());
}

TEST_CASE("flag-parse-twice", "[Flag]")
{
  auto flag = Flag::make("name");
  int argc;
  const char * argv[] = { "progr" };
  std::vector<std::string> err;
  int i = 0;
  REQUIRE(flag->parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE_FALSE(flag->parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE(err.size() == 1u);
  REQUIRE(flag->isSet());
}
