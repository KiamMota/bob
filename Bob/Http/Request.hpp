#ifndef _BOBREQ_HPP_
#define _BOBREQ_HPP_

#include "Enums.hpp"
#include <string>

namespace Bob
{
namespace Http
{

  class Request
  {
    private:

    std::string _AllBuff;
    Bob::Http::HttpMethod _Method;
    std::string _Route;
    std::string _HttpVersion;
    std::string _Host;
    Bob::Http::ContentType _ContentType;
    long _ContentLength;
    public:
    Request(const std::string& Buffer);
    Request(const char* Buffer);
    std::string& operator[](const std::string& getter);
    Http::HttpMethod GetMethod();
    Http::ContentType GetContentType();
    void HeaderDeserializer();
    void MethodDeserializer();
    void RouteDeserializer();
    void HttpVersionDeserializer();
    void ContentTypeDeserialize();
    void ContentLengthDeserialize();
    void HostDeserialize();
    void DeserializeAll();
    
};
}

}

#endif
