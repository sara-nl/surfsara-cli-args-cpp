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

#include <catch.hpp>
#include "cli_parser.h"
#include "cli_flag.h"

TEST_CASE("add argument with name", "[Arguments]")
{
  Cli::Parser args;
  auto flag = args.add(std::make_shared<Cli::Flag>("myflag"));
  REQUIRE(args.hasArgument("myflag"));
  REQUIRE_FALSE(args.hasArgument("myflag2"));
  REQUIRE_FALSE(args.hasArgument('f'));
  REQUIRE_FALSE(args.isSet("myflag"));
  REQUIRE_FALSE(args.isSet("myflag2"));
  REQUIRE_FALSE(args.isSet('f'));

  flag->parse(0, {});
  REQUIRE(args.isSet("myflag"));
  REQUIRE_FALSE(args.isSet('f'));
  REQUIRE_FALSE(args.isSet("myflag2"));
  REQUIRE_FALSE(args.isSet('g'));
}

TEST_CASE("add argument with shortName", "[Arguments]")
{
  Cli::Parser args;
  auto flag = args.add(std::make_shared<Cli::Flag>('f'));
  REQUIRE_FALSE(args.hasArgument("myflag"));
  REQUIRE(args.hasArgument('f'));
  REQUIRE_FALSE(args.hasArgument('g'));
  REQUIRE_FALSE(args.isSet("myflag"));
  REQUIRE_FALSE(args.isSet('g'));
  REQUIRE_FALSE(args.isSet('f'));

  flag->parse(0, {});
  REQUIRE(args.isSet('f'));
  REQUIRE_FALSE(args.isSet("myflag"));
  REQUIRE_FALSE(args.isSet("myflag2"));
  REQUIRE_FALSE(args.isSet('g'));
}

TEST_CASE("add argument with name and shortName", "[Arguments]")
{
  Cli::Parser args;
  auto flag = args.add(std::make_shared<Cli::Flag>('f', "myflag"));
  REQUIRE(args.hasArgument("myflag"));
  REQUIRE(args.hasArgument('f'));
  REQUIRE_FALSE(args.hasArgument("myflag2"));
  REQUIRE_FALSE(args.hasArgument('g'));
  REQUIRE_FALSE(args.isSet("myflag"));
  REQUIRE_FALSE(args.isSet("myflag2"));
  REQUIRE_FALSE(args.isSet('f'));
  REQUIRE_FALSE(args.isSet('g'));

  flag->parse(0, {});
  REQUIRE(args.isSet("myflag"));
  REQUIRE(args.isSet('f'));
  REQUIRE_FALSE(args.isSet("myflag2"));
  REQUIRE_FALSE(args.isSet('g'));
}

TEST_CASE("first trival test", "[Parser]")
{
  //int argc = 4;
  //const char * argv[] = { "progr", "--flag1", "-g", "-hij", "--flag2" };
  //Cli::Parser parser;
  //parser.parse(sizeof(argv)/sizeof(char*), argv);
  //REQUIRE(true);
}

