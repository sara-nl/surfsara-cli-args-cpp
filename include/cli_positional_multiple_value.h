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
#include "cli_converter.h"
#include <string>
#include <sstream>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <iostream>

namespace Cli
{
  template<typename T>
  class PositionalMultipleValue : public Argument
  {
  public:
    typedef PositionalMultipleValue<T> self_type;
    typedef std::vector<T> vector_type;
    typedef std::shared_ptr<self_type> shared_type;

    static shared_type make(const std::string & _name = "ARGS",
                            const Doc & _doc=Doc(""))
    {
      vector_type * value = new vector_type;
      return shared_type(new self_type(value, *value, _name, _doc));
    }

    static shared_type make(vector_type & _refValue,
                            const std::string & _name = "ARGS",
                            const Doc & _doc=Doc(""))
    {
      return shared_type(new self_type(nullptr, _refValue, _name, _doc));
    }

    virtual bool isFlag() const override
    {
      return false;
    }

    virtual bool isSet() const override
    {
      return valueset;
    }

    std::vector<T> & getValue()
    {
      return refValue;
    }

    const std::vector<T> & getValue() const
    {
      return refValue;
    }

    void setValue(const std::vector<T> & value)
    {
      refValue = value;
    }

    virtual bool isMultiple() const override
    {
      return true;
    }

    virtual bool isPositional() const override
    {
      return true;
    }

    virtual bool parseArgument(int argc, const char ** argv,
                               int & i, std::vector<std::string> & err) override
    {
      valueset = true;
      T tmp;
      if(converter(argv[i], tmp, err))
      {
        refValue.push_back(tmp);
        return true;
      }
      else
      {
        return false;
      }
    }

    ~PositionalMultipleValue()
    {
      if(value)
      {
        delete value;
      }
    }

    virtual void streamOut(std::ostream & ost) const override
    {
      bool first = true;
      for(auto & p : refValue)
      {
        if(first)
        {
          first = false;
        }
        else
        {
          ost << ",";
        }
        ost << p;
      }
    }

    virtual const std::type_info& getTypeInfo() const override
    {
      return typeid(T);
    }

  protected:
    virtual void * valueCastImpl(const std::type_info& ti) const override
    {
      return (void*)(&refValue);
    }

  private:
    vector_type * value;
    vector_type & refValue;
    bool valueset;
    details::Converter<T> converter;
    PositionalMultipleValue(vector_type * _value,
                            vector_type & _refValue,
                            const std::string & _name,
                            const Doc & _doc=Doc("")) :
      Argument('\0',
               _name,
               _doc),
      value(_value),
      refValue(_refValue),
      valueset(false)
    {
    }
  };
}

