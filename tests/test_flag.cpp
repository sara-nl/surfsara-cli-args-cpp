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
#include "cli_flag.h"

TEST_CASE("flag-constructor-with-name", "[Flag]")
{
  REQUIRE(Cli::Flag("name").isFlag());
  REQUIRE(Cli::Flag("name").getName() == "name");
  REQUIRE(Cli::Flag("name").getShortName() == '\0');
  REQUIRE_FALSE(Cli::Flag("name").isSet());
}

TEST_CASE("flag-constructor-with-short-name", "[Flag]")
{
  REQUIRE(Cli::Flag('n').isFlag());
  REQUIRE(Cli::Flag('n').getName() == "");
  REQUIRE(Cli::Flag('n').getShortName() == 'n');
  REQUIRE_FALSE(Cli::Flag('n').isSet());
}

TEST_CASE("flag-constructor-with-name-and-short-name", "[Flag]")
{
  REQUIRE(Cli::Flag('n', "name").isFlag());
  REQUIRE(Cli::Flag('n', "name").getName() == "name");
  REQUIRE(Cli::Flag('n', "name").getShortName() == 'n');
  REQUIRE_FALSE(Cli::Flag('n').isSet());
}

TEST_CASE("flag-parse", "[Flag]")
{
  Cli::Flag flag("name");
  int argc;
  const char * argv[] = { "progr" };
  std::vector<std::string> err;
  int i = 0;
  REQUIRE(flag.parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE(err.empty());
  REQUIRE(flag.isSet());
}

TEST_CASE("flag-parse-twice", "[Flag]")
{
  Cli::Flag flag("name");
  int argc;
  const char * argv[] = { "progr" };
  std::vector<std::string> err;
  int i = 0;
  REQUIRE(flag.parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE_FALSE(flag.parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE(err.size() == 1u);
  REQUIRE(flag.isSet());
}
