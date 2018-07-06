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
#include <sstream>
#include <typeinfo>
#include <memory>
#include "cli_argument.h"

#include <iostream>

namespace Cli
{
  template<typename T>
  class Value : public Argument
  {
  public:
    typedef Value<T> self_type;
    typedef std::shared_ptr<self_type> shared_type;

    static shared_type make(T & _refValue,
                            const std::string & _name,
                            const Doc & _doc=Doc(""))
    {
      return shared_type(new self_type(_refValue, '\0', _name, _doc));
    }
    
    static shared_type make(T & _refValue,
                            char _shortname,
                            const Doc & _doc=Doc(""))
    {
      return shared_type(new self_type(_refValue, _shortname, "", _doc));
    }

    static shared_type make(T & _refValue,
                            char _shortname,
                            const std::string & _name,
                            const Doc & _doc=Doc("")) 
    {
      return shared_type(new self_type(_refValue, _shortname, _name, _doc));
    }

    static shared_type make(T & _refValue,
                            const Doc & _doc=Doc(""))
    {
      return shared_type(new self_type(_refValue, '\0',"", _doc));
    }

    virtual bool isFlag() const override
    {
      return false;
    }

    bool isSet() const override
    {
      return valueset;
    }

    virtual bool isMultiple() const override
    {
      return false;
    }

    bool parse(int argc, const char ** argv,
               int & i, std::vector<std::string> & err) override
    {
      if( getShortName() != '\0' || !getName().empty())
      {
        if( i + 1 >= argc)
        {
          err.push_back(getFullName() + " requires an argument");
          return false;
        }
        i++;
      }
      if(valueset && !twice)
      {
        twice = true;
        err.push_back(std::string("value ") + getFullName() +
                      std::string(" set twice."));
        return false;
      }
      valueset = true;
      std::stringstream stream(argv[i]);
      T value;
      stream >> value;
      if(stream.fail() || !stream.eof()) 
      {
        err.push_back(std::string(argv[i]) +
                      " is not a valid " +
                      typeid(T).name());
        return false;
      }
      refValue = value;
      return true;
    }

  private:
    T & refValue;
    bool valueset;
    bool twice;

    Value(T & _refValue,
          char _shortname,
          const std::string & _name,
          const Doc & _doc=Doc("")) :
      Argument(_shortname, _name, _doc),
      refValue(_refValue),
      valueset(false),
      twice(false)
    {
    }
  };
}

