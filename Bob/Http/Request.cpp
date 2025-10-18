#include "Request.hpp"
#include "Http/Enums.hpp"
#include <cstdint>
#include <string>

#define SPACE 1

Bob::Http::Request::Request(const std::string& Buffer) : _desserializer(Buffer) {}

Bob::Http::Request::Request(const char* Buffer) : _desserializer(Buffer) {}

void Bob::Http::Request::DeserializeAll()
{
  _methodStr = _desserializer.DeserializeMethod();
  _route = _desserializer.DeserializeRoute();
  _httpVersion = _desserializer.DeserializeVersion();
  _ContentTypeStr = _desserializer.DeseriaizeContentType();
  _contentLength = std::stoi(_desserializer.DeserializeContentLength());
  _host = _desserializer.DeserializeHost();
}

std::string Bob::Http::Request::Version()
{
  return _httpVersion;
}

std::string Bob::Http::Request::Body()
{
  return _body;
}

std::string Bob::Http::Request::Method()
{
  return _route;
}

std::string Bob::Http::Request::Route()
{
  return _route;
}

uint64_t Bob::Http::Request::ContentLength()
{
  return _contentLength;
}

Bob::Http::ContentTypeEnum Bob::Http::Request::ContentType()
{
  return _contentType;
}


