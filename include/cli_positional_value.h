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
#include "cli_converter.h"
#include "cli_argument.h"
#include <string>
#include <sstream>
#include <typeinfo>
#include <typeindex>
#include <memory>



namespace Cli
{
  template<typename T>
  class PositionalValue : public Argument
  {
  public:
    typedef PositionalValue<T> self_type;
    typedef std::shared_ptr<self_type> shared_type;

    static shared_type make(const std::string & _name = "ARG",
                            const Doc & _doc=Doc(""))
    {
      T * value = new T();
      return shared_type(new self_type(value, *value, _name, _doc));
    }

    static shared_type make(T & _refValue,
                            const std::string & _name = "ARG",
                            const Doc & _doc=Doc(""))
    {
      return shared_type(new self_type(nullptr, _refValue, _name, _doc));
    }
    
    virtual bool isFlag() const override
    {
      return false;
    }

    bool isSet() const override
    {
      return valueset;
    }

    T & getValue()
    {
      return refValue;
    }

    const T & getValue() const
    {
      return refValue;
    }

    void setValue(const T & value)
    {
      refValue = value;
    }


    virtual bool isMultiple() const override
    {
      return false;
    }

    virtual bool isPositional() const override
    {
      return true;
    }

    virtual bool parseArgument(int argc, const char ** argv,
                               int & i, std::vector<std::string> & err) override
    {
      if(valueset && !twice)
      {
        twice = true;
        err.push_back(std::string("value ") + getFullName() +
                      std::string(" set twice."));
        return false;
      }
      valueset = true;
      return converter(argv[i], refValue, err);
    }

    ~PositionalValue()
    {
      if(value)
      {
        delete value;
      }
    }

    virtual void streamOut(std::ostream & ost) const override
    {
      ost << refValue;
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
    T * value;
    T & refValue;
    bool valueset;
    bool twice;
    details::Converter<T> converter;

    PositionalValue(T * _value,
                    T & _refValue,
                    const std::string & _name,
                    const Doc & _doc=Doc("")) :
      Argument('\0', _name, _doc),
      value(_value),
      refValue(_refValue),
      valueset(false),
      twice(false)
    {
    }
  };
}

