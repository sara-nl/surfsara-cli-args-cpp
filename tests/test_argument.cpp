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
#include "cli_argument.h"

/*@todo rename to test_argument */
TEST_CASE("slit-lines", "[Doc]")
{
  REQUIRE(Cli::Argument::splitLines("line1\nline2\nline3\n") ==
          std::vector<std::string>({"line1", "line2", "line3"}));
  REQUIRE(Cli::Argument::splitLines("line1\nline2\nline3") ==
          std::vector<std::string>({"line1", "line2", "line3"}));
  REQUIRE(Cli::Argument::splitLines("line1\r\nline2\nline3") ==
          std::vector<std::string>({"line1", "line2", "line3"}));
}
