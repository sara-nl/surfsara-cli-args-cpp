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
#include <memory>
#include <typeinfo>
#include "cli_argument.h"

namespace Cli
{
  class Flag : public Argument
  {
  public:
    typedef std::shared_ptr<Flag> shared_type;

    static shared_type make(const std::string & _name,
                            const Doc & _doc=Doc(""))
    {
      return shared_type(new Flag('\0', _name, _doc));
    }


    static shared_type make(char _shortname,
                            const Doc & _doc=Doc(""))
    {
      return shared_type(new Flag(_shortname, "", _doc));
    }

    static shared_type make(char _shortname,
                            const std::string & _name,
                            const Doc & _doc=Doc(""))
    {
      return shared_type(new Flag(_shortname, _name, _doc));
    }
                            
    virtual bool isFlag() const override
    {
      return true;
    }

    bool isSet() const override
    {
      return flagset;
    }

    void setValue(bool value)
    {
      flagset = value;
    }

    virtual bool isMultiple() const override
    {
      return false;
    }

    virtual bool isPositional() const override
    {
      return false;
    }

    virtual bool parseArgument(int argc,
                               const char ** argv,
                               int & i,
                               std::vector<std::string> & err) override
    {
      if(flagset && !twice)
      {
        twice = true;
        err.push_back(std::string("flag ") + getFullName() +
                      std::string(" set twice."));
        return false;
      }
      else
      {
        flagset = true;
        return true;
      }
    }

    virtual void streamOut(std::ostream & ost) const override
    {
      ost << flagset;
    }

    virtual const std::type_info& getTypeInfo() const override
    {
      return typeid(bool);
    }

  protected:
    virtual void * valueCastImpl(const std::type_info& ti) const override
    {
      return (void*)(&flagset);
    }

  private:
    bool flagset;
    bool twice;

    Flag(char _shortname,
         const std::string & _name,
         const Doc & _doc=Doc("")) :
      Argument(_shortname, _name, _doc), flagset(false), twice(false)
    {
    }
  };
}
