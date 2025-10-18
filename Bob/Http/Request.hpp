#ifndef _BOBREQ_HPP_
#define _BOBREQ_HPP_

#include "Enums.hpp"
#include "Http/Deserializer.hpp"
#include <cstdint>
#include <string>

namespace Bob {
namespace Http
{

  class Request
  {
    private:
    Bob::Http::Deserializer _desserializer;
    Bob::Http::HttpMethodEnum _method;
    Bob::Http::ContentTypeEnum _contentType;
    uint64_t _contentLength;
    std::string _methodStr;
    Bob::Http::HttpMethodEnum _MethodMapper();
    std::string _ContentTypeStr;
    Bob::Http::ContentTypeEnum _ContentTypeMapper();
    std::string _route;
    std::string _httpVersion;
    std::string _host;
    std::string _body;   
     

    public:
    Request(const std::string& Buffer);
    Request(const char* Buffer);
    std::string Body();
    Http::HttpMethodEnum Method();
    std::string Host();
    std::string Version();
    std::string Route();
    Bob::Http::ContentTypeEnum ContentType();
    uint64_t ContentLength();
    void DeserializeAll(); 
};
}

}

#endif
