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
#include <string>
#include <vector>
#include <ostream>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include "cli_doc.h"

namespace Cli { class Argument; }

inline std::ostream & operator<<(std::ostream & ost, const Cli::Argument & arg);
  
namespace Cli
{
  class Argument
  {
  public:
    virtual ~Argument() {}

    virtual std::string getName() const
    {
      return name;
    }
    
    virtual char getShortName() const
    {
      return shortname;
    }

    virtual bool isSet() const = 0;
    virtual bool isMultiple() const = 0;
    virtual bool isPositional() const = 0;

    virtual bool isFlag() const
    {
      return false;
    }

    bool parse(int argc, const char ** argv,
                       int & i, std::vector<std::string> & err)
    {
      parseArgument(argc, argv, i, err);
    }

    bool parse(int argc, const char ** argv, int & i)
    {
      std::vector<std::string> err;
      return parseArgument(argc, argv, i, err);
    }

    bool parse(int argc, const char ** argv, std::vector<std::string> & err)
    {
      int i;
      return parseArgument(argc, argv, i, err);
    }

    bool parse(int argc, const char ** argv)
    {
      std::vector<std::string> err;
      int i = 0;
      return parseArgument(argc, argv, i, err);
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    // Print help functions
    //
    ///////////////////////////////////////////////////////////////////////////
    virtual std::size_t getArgumentsHelpWidth() const
    {
      std::size_t ret = 0;
      std::string name = getName();
      char shortname = getShortName();
      if(shortname != '\0')
      {
        ret+= 4;
      }
      if(name != "")
      {
        if(ret > 0) ret+= 1;
        ret+= name.size() + 4;
      }
      return ret;
    }

    virtual void printHelp(std::ostream & ost, std::size_t width=0) const
    {
      bool shortNameWritten = false;
      std::size_t col = 0;
      std::string name = getName();
      char shortname = getShortName();
      if(width == 0)
      {
        width = getArgumentsHelpWidth();
      }
      if(shortname != '\0')
      {
        ost << "-" << shortname;
        col += 2;
        shortNameWritten = true;
      }
      if(name != "")
      {
        if(shortNameWritten)
        {
          ost << ",";
          col += 1;
        }
        if(!isPositional())
        {
          ost << "--";
          col += 2;
        }
        ost << name;
        col += name.size();
      }
      ost << "  ";
      col+= 2;
      auto docLines = splitLines(doc);
      bool firstLine = true;
      for(auto line : docLines)
      {
        if(firstLine)
        {
          firstLine = false;
          while(col < width)
          {
            ost.put(' ');
            col++;
          }
        }
        else
        {
          for(std::size_t i = 0; i < col; i++)
          {
            ost.put(' ');
          }
        }
        ost << line << std::endl;
      }
    }

    static std::vector<std::string> splitLines(const std::string & str)
    {
      std::vector<std::string> ret;
      std::string::size_type pos = 0;
      std::string::size_type prev = 0;
      while(prev < str.size())
      {
        std::string::size_type pos_r = str.find('\r', prev);
        std::string::size_type pos_n = str.find('\n', prev);
        if(pos_r != std::string::npos && pos_n != std::string::npos && pos_r+1 == pos_n)
        {
          ret.push_back(str.substr(prev, pos_r - prev));
          prev = pos_n + 1;
        }
        else if(pos_n != std::string::npos)
        {
          ret.push_back(str.substr(prev, pos_n - prev));
          prev = pos_n + 1;
        }
        else
        {
          break;
        }
      }
      if(prev < str.size())
      {
        ret.push_back(str.substr(prev, str.size() - prev));
      }
      return ret;
    }

    virtual void streamOut(std::ostream & ost) const = 0;
    virtual const std::type_info& getTypeInfo() const = 0;

    template<typename T>
    const T & valueCast() const
    {
      if(std::type_index(typeid(T)) != std::type_index(getTypeInfo()))
      {
        throw std::bad_cast();
      }
      return *static_cast<T*>(valueCastImpl(typeid(T)));
    }

  protected:
    Argument(char _shortname, const std::string & _name, const Doc & _doc)
      : doc(_doc.doc), shortname(_shortname), name(_name)
    {
    }

    virtual bool parseArgument(int argc, const char ** argv,
                               int & i, std::vector<std::string> & err) = 0;

    std::string getFullName() const
    {
      if(getName().empty())
      {
        return std::string("-") + getShortName();
      }
      else
      {
        return std::string("--") + getName();
      }
    }
    virtual void * valueCastImpl(const std::type_info& ti) const = 0;

  private:
    char shortname;
    std::string name;
    std::string doc;
  };
}


std::ostream & operator<<(std::ostream & ost, const Cli::Argument & arg)
{
  arg.streamOut(ost);
  return ost;
}
