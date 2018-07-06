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
#include <cli_parser.h>
#include <cli_flag.h>
#include <cli_value.h>
#include <cli_multiple_value.h>

using Parser = Cli::Parser;
using Flag = Cli::Flag;

TEST_CASE("add-argument-with-name", "[Parser]")
{
  Parser args;
  auto flag = args.add(Flag::make("myflag"));
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

TEST_CASE("add-argument-with-shortName", "[Parser]")
{
  Parser args;
  auto flag = args.add(Flag::make('f'));
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

TEST_CASE("add-argument-with-name-and-shortName", "[Parser]")
{
  Parser args;
  auto flag = args.add(Flag::make('f', "myflag"));
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

TEST_CASE("parse-flags", "[Parser]")
{
  Parser parser;
  auto flag1 = parser.add(Flag::make('f', "flag1"));
  auto flag2 = parser.add(Flag::make('g', "flag2"));
  auto flagh = parser.add(Flag::make('h'));
  auto flag3 = parser.add(Flag::make("flag3"));
  {
    const char * argv[] = { "progr" };
    std::vector<std::string> err;
    REQUIRE(parser.parse(sizeof(argv)/sizeof(char*), argv, err));
    REQUIRE(err.empty());
    REQUIRE_FALSE(parser.isSet("flag1"));
    REQUIRE_FALSE(parser.isSet("flag2"));
    REQUIRE_FALSE(parser.isSet("flag3"));
    REQUIRE_FALSE(parser.isSet('h'));

  }
  {
    const char * argv[] = { "progr", "--flag1", "-gh", "--flag3"};
    std::vector<std::string> err;
    REQUIRE(parser.parse(sizeof(argv)/sizeof(char*), argv, err));
    REQUIRE(err.empty());
    REQUIRE(parser.isSet("flag1"));
    REQUIRE(parser.isSet("flag2"));
    REQUIRE(parser.isSet("flag3"));
    REQUIRE(parser.isSet('h'));
  }
  {
    const char * argv[] = { "progr", "--flag1", "-f", "--flag1"};
    std::vector<std::string> err;
    REQUIRE_FALSE(parser.parse(sizeof(argv)/sizeof(char*), argv, err));
    REQUIRE(err.size() == 1u);
  }
}

TEST_CASE("parse-flags-named-and-unnamed", "[Parser]")
{
  Parser parser;
  int i1 = 0;
  std::vector<int> n;
  int i2 = 0;
  std::string str;
  auto flag1   = parser.add(Flag::make('f', "flag1"));
  auto flagh   = parser.add(Flag::make('h'));
  auto arg_i1  = parser.add(Cli::Value<int>::make(i1, 'i'));
  auto arg_str = parser.add(Cli::Value<std::string>::make(str));
  auto arg_n   = parser.add(Cli::MultipleValue<int>::make(n, 'n'));
  auto arg_i2  = parser.add(Cli::Value<int>::make(i2));
  {
    const char * argv[] = { "progr" };
    std::vector<std::string> err;
    REQUIRE(parser.parse(sizeof(argv)/sizeof(char*), argv, err));
    REQUIRE(err.empty());
    REQUIRE_FALSE(parser.isSet('f'));
    REQUIRE_FALSE(parser.isSet('h'));
    REQUIRE_FALSE(parser.isSet('n'));
    REQUIRE_FALSE(arg_str->isSet());
    REQUIRE_FALSE(arg_i2->isSet());
  }
  {
    const char * argv[] = { "progr", "--flag1"};
    std::vector<std::string> err;
    REQUIRE(parser.parse(sizeof(argv)/sizeof(char*), argv, err));
    REQUIRE(err.empty());
    REQUIRE(parser.isSet("flag1"));
  }
}

