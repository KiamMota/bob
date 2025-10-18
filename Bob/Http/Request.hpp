#ifndef _BOBREQ_HPP_
#define _BOBREQ_HPP_

#include "Enums.hpp"
#include "Http/Deserializer.hpp"
#include <string>

namespace Bob {
namespace Http
{

  class Request
  {
    private:
    Bob::Http::Deserializer _desserializer;
    Bob::Http::HttpMethodEnum _method;
    std::string _route;
    std::string _httpVersion;
    std::string _host;
    Bob::Http::ContentTypeEnum _ContentType;
    long _ContentLength;
    public:
    Request(const std::string& Buffer);
    Request(const char* Buffer);
    /* universal getter */
    void DeserializeAll(); 
};
}

}

#endif
