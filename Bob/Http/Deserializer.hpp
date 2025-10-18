#ifndef _DESERIALIZER_HPP_
#define _DESERIALIZER_HPP_

#include <string>

namespace Bob
{
namespace Http
{
  class Deserializer
  {
    private:
      std::string _allBuffer; 
    public:
      Deserializer(const std::string& data);
      Deserializer(const char* data);   
      std::string DeserializeMethod();
      std::string DeserializeRoute();
      std::string DeserializeVersion();
      std::string DeseriaizeContentType();
      std::string DeserializeContentLength();
      std::string DeserializeHost();
      std::string DeserializeBody();
  };


}

}

#endif
