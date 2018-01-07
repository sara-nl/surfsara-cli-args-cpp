/******************************************************************************
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
SOFTWARE. */
#pragma once
#include "cli_argument.h"

namespace Cli
{
  class MultipleFlag : public Argument
  {
  public:
    MultipleFlag(const std::string & _name) :
      shortname('\0'), name(_name), count(0)
    {
    }

    MultipleFlag(char _shortname, const std::string & _name="") :
      shortname(_shortname), name(_name), count(0)
    {
    }

    virtual bool isFlag() const
    {
      return true;
    }

    std::string getName() const override
    {
      return name;
    }

    char getShortName() const override
    {
      return shortname;
    }

    bool isSet() const override
    {
      return count > 0;
    }

    std::size_t numSet() const
    {
      return count;
    }

    bool parse(int argc, const char ** argv,
               int & i, std::vector<std::string> & err) override
    {
      count++;
      return true;
    }

  private:
    char shortname;
    std::string name;
    std::size_t count;
  };
}

