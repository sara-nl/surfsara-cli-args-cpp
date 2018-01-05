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
#include <string>
#include <string.h>
#include <vector>
#include <memory>
#include <map>

namespace Cli
{
  class Parser
  {
  public:
    std::shared_ptr<Argument> add(std::shared_ptr<Argument> arg)
    {
      arguments.push_back(arg);
      if(!arg->getName().empty())
      {
        name2argument[arg->getName()] = arg;
      }
      if(arg->getShortName())
      {
        shortname2argument[arg->getShortName()] = arg;
      }
      return arg;
    }

    bool hasArgument(const std::string & name) const
    {
      return name2argument.find(name) != name2argument.end();
    }

    bool hasArgument(char shortname) const
    {
      return shortname2argument.find(shortname) != shortname2argument.end();
    }
    
    bool isSet(const std::string & name) const
    {
      auto itr = name2argument.find(name);
      if(itr == name2argument.end())
      {
        return false;
      }
      else
      {
        return itr->second->isSet();
      }
    }

    bool isSet(char shortname) const
    {
      auto itr = shortname2argument.find(shortname);
      if(itr == shortname2argument.end())
      {
        return false;
      }
      else
      {
        return itr->second->isSet();
      }
    }

    bool parse(int argc, const char ** argv) const
    {
      std::vector<std::string> err;
      return parse(argc, argv, err);
    }

    bool parse(int argc, const char ** argv, std::vector<std::string> & err) const
    {
      int i;
      bool ret = true;
      for(i = 1; i < argc; i++)
      {
        std::size_t len = strlen(argv[i]);
        std::map<std::string, std::shared_ptr<Argument> >::const_iterator nitr;
        std::map<char, std::shared_ptr<Argument> >::const_iterator sitr;
        if(len > 1 && argv[i][0] == '-' && argv[i][1] == '-' &&
           (nitr = name2argument.find(argv[i] + 2) ) != name2argument.end())
        {
          ret&= nitr->second->parse(argc, argv, i, err);
        }
        else if(len == 2 && argv[i][0] == '-' && argv[i][1] != '-' &&
                (sitr =  shortname2argument.find(argv[i][1])) != shortname2argument.end())
        {
          ret&= sitr->second->parse(argc, argv, i, err);
        }
        else if(len > 2 && argv[i][0] == '-' && argv[i][1] != '-')
        {
          std::size_t j;
          for(j = 1; j < len; j++)
          {
            if( (sitr = shortname2argument.find(argv[i][j])) != shortname2argument.end())
            {
              if(sitr->second->isFlag())
              {
                ret&= sitr->second->parse(argc, argv, i, err);
              }
              else
              {
                /* error */
                ret = false;
              }
            }
            else
            {
              /* error */
              ret = false;
            }
          }
        }
        else
        {
          /* error */
          ret = false;
        }
      }
      return ret;
    }

    
  private:
    std::vector<std::shared_ptr<Argument> > arguments;
    std::map<std::string, std::shared_ptr<Argument> > name2argument;
    std::map<char, std::shared_ptr<Argument> > shortname2argument;
  };
}

