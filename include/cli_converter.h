#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

namespace Cli
{
  namespace details
  {
    template<typename T>
    class Converter
    {
    public:
      bool operator()(const char * str, T & refValue, std::vector<std::string> & err)
      {
        std::stringstream stream(str);
        T value;
        stream >> value;
        if(stream.fail() || !stream.eof()) 
        {
          err.push_back(std::string(str) +
                        " is not a valid " +
                        typeid(T).name());
          return false;
        }
        refValue = value;
        return true;
      }
    };

    template<>
    class Converter<std::string>
    {
    public:
      bool operator()(const char * str, std::string & refValue, std::vector<std::string> & err)
      {
        refValue = std::string(str);
        return true;
      }
    };

  }
};
