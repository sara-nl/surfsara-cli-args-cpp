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
#include <catch2/catch.hpp>
#include <cli_value.h>
#include <cli_positional_value.h>

using IntValue = Cli::Value<int>;
using PositionalIntValue = Cli::PositionalValue<int>;

TEST_CASE("value-constructor-with-name", "[Value]")
{
  int v = 0;
  REQUIRE_FALSE(IntValue::make(v, "name")->isFlag());
  REQUIRE_FALSE(IntValue::make(v, "name")->isPositional());
  REQUIRE(IntValue::make(v, "name")->getName() == "name");
  REQUIRE(IntValue::make(v, "name")->getShortName() == '\0');
  REQUIRE_FALSE(IntValue::make(v, "name")->isSet());
}

TEST_CASE("value-constructor-with-short-name", "[Value]")
{
  int v = 0;
  REQUIRE_FALSE(IntValue::make(v, 'n')->isFlag());
  REQUIRE_FALSE(IntValue::make(v, 'n')->isPositional());
  REQUIRE(IntValue::make(v, 'n')->getName() == "");
  REQUIRE(IntValue::make(v, 'n')->getShortName() == 'n');
  REQUIRE_FALSE(IntValue::make(v, 'n')->isSet());
}

TEST_CASE("value-constructor-with-name-and-short-name", "[Value]")
{
  int v = 0;
  REQUIRE_FALSE(IntValue::make(v, 'n', "name")->isFlag());
  REQUIRE_FALSE(IntValue::make(v, 'n', "name")->isPositional());
  REQUIRE(IntValue::make(v, 'n', "name")->getName() == "name");
  REQUIRE(IntValue::make(v, 'n', "name")->getShortName() == 'n');
  REQUIRE_FALSE(IntValue::make(v, 'n')->isSet());
}

TEST_CASE("value-constructor-positional", "[Value]")
{
  int v = 0;
  REQUIRE_FALSE(PositionalIntValue::make(v)->isFlag());
  REQUIRE(PositionalIntValue::make(v)->isPositional());
  REQUIRE(PositionalIntValue::make(v, "test")->getName() == "test");
  REQUIRE(PositionalIntValue::make(v)->getShortName() == '\0');
  REQUIRE_FALSE(PositionalIntValue::make(v)->isSet());
}

TEST_CASE("value-parse", "[Value]")
{
  int v = 0;
  auto value = IntValue::make(v, "name");
  int argc;
  const char * argv[] = { "progr", "--name", "12" };
  std::vector<std::string> err;
  int i = 1;
  REQUIRE(value->parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE(err.empty());
  REQUIRE(value->isSet());
  REQUIRE(i == 2);
  REQUIRE(v == 12);
}

TEST_CASE("value-parse-twice", "[Value]")
{
  int v = 0;
  auto value = IntValue::make(v, "name");
  int argc;
  const char * argv1[] = { "progr", "--name", "12" };
  const char * argv2[] = { "progr", "--name", "13" };
  std::vector<std::string> err;
  int i = 1;
  REQUIRE(value->parse(sizeof(argv1)/sizeof(char*), argv1, i, err));
  REQUIRE(err.empty());
  REQUIRE(value->isSet());
  REQUIRE(i == 2);
  REQUIRE_FALSE(value->parse(sizeof(argv2)/sizeof(char*), argv2, i, err));
  REQUIRE(err.size() == 1u);
  REQUIRE(v == 12);
}

TEST_CASE("value-parse-argument-required", "[Value]")
{
  int v = 0;
  auto value = IntValue::make(v, "name");
  int argc;
  const char * argv[] = { "progr", "--name" };
  std::vector<std::string> err;
  int i = 1;
  REQUIRE_FALSE(value->parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE(err.size() == 1u);
  REQUIRE_FALSE(value->isSet());
  REQUIRE(i == 1);
  REQUIRE(v == 0);
}

TEST_CASE("value-parse-invalid-value", "[Value]")
{
  int v = 0;
  auto value = IntValue::make(v, "name");
  int argc;
  const char * argv[] = { "progr", "--name", "12xx" };
  std::vector<std::string> err;
  int i = 1;
  REQUIRE_FALSE(value->parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE(err.size() == 1u);
  REQUIRE(value->isSet());
  REQUIRE(i == 2);
  REQUIRE(v == 0);
}

TEST_CASE("value-parse-positional", "[Value]")
{
  int v = 0;
  auto value = PositionalIntValue::make(v, "name");
  int argc;
  const char * argv[] = { "progr", "12" };
  std::vector<std::string> err;
  int i = 1;
  REQUIRE(value->parse(sizeof(argv)/sizeof(char*), argv, i, err));
  REQUIRE(err.empty());
  REQUIRE(value->isSet());
  REQUIRE(i == 1);
  REQUIRE(v == 12);
}

