#include "Request.hpp"
#include "Http/Enums.hpp"
#include "Server/BobServer.hpp"
#include <cstdint>
#include <string>

#define SPACE 1

Bob::Http::Request::Request(const std::string& Buffer) : _desserializer(Buffer) {}

Bob::Http::Request::Request(const char* Buffer) : _desserializer(Buffer) {}

void Bob::Http::Request::DeserializeAll()
{
  _methodStr = _desserializer.DeserializeMethod();
  _method = _MethodMapper();
  _route = _desserializer.DeserializeRoute();
  _httpVersion = _desserializer.DeserializeVersion();
  _ContentTypeStr = _desserializer.DeseriaizeContentType();
  _contentLength = std::stoi(_desserializer.DeserializeContentLength());
  _host = _desserializer.DeserializeHost();
}

Bob::Http::HttpMethodEnum Bob::Http::Request::_MethodMapper()
{
  if(_methodStr == "GET") return HttpMethodEnum::GET;
  if(_methodStr == "POST") return HttpMethodEnum::POST;
  if(_methodStr == "PUT") return HttpMethodEnum::PUT;
  if(_methodStr == "DELETE") return HttpMethodEnum::DELETE;
  if(_methodStr == "PATCH") return HttpMethodEnum::PATCH;
  return HttpMethodEnum::UNKNOWN;
}

Bob::Http::ContentTypeEnum Bob::Http::Request::_ContentTypeMapper() {
    if (_ContentTypeStr == "application/json") return ContentTypeEnum::ApplicationJson;
    if (_ContentTypeStr == "application/xml") return ContentTypeEnum::ApplicationXml;
    if (_ContentTypeStr == "application/octet-stream") return ContentTypeEnum::ApplicationOctetStream;
    if (_ContentTypeStr == "image/jpeg") return ContentTypeEnum::ImageJpeg;
    return ContentTypeEnum::UnknownOrEmpty;
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


