#include "Enums.hpp"
#include <cstddef>
#include <cstring>
#include <string>

#define SPACE 1

Bob::Http::Request::Request(const std::string& Buffer)
{
  _AllBuff = Buffer;
}

Bob::Http::Request::Request(const char* Buffer)
{
  _AllBuff = Buffer;
}

void Bob::Http::Request::DeserializeAll()
{
  HeaderDeserializer();
  if(_Method != Http::HttpMethod::GET)
  {
    ContentTypeDeserialize();
    ContentLengthDeserialize();
  }
}

std::string& Bob::Http::Request::operator[](const std::string& getter)
{
  if(getter == "Route") return _Route;
  if(getter == "Host") return _Host;
  if(getter == "Version") return _HttpVersion;
  if(getter == "All") return _AllBuff;
  return _AllBuff;
}

void Bob::Http::Request::MethodDeserializer()
{ 
  char separator = ' ';
  size_t pos = _AllBuff.find(separator);
  if(pos == std::string::npos) return;
  std::string method = _AllBuff.substr(0, pos);
  if(method == "GET")
  {
    _Method = Http::HttpMethod::GET;
    return;
  }
  if(method == "POST")
  {
    _Method = Http::HttpMethod::POST;
    return;
  }
  if(method == "PUT")
  {
    _Method = Http::HttpMethod::PUT;
    return;
  }
  if(method == "PATCH")
  {
    _Method = Http::HttpMethod::PATCH;
    return;
  }
  if(method == "DELETE")
  {
    _Method = Http::HttpMethod::DELETE;
    return;
  }
  _Method = Http::HttpMethod::UNKNOWN;
}

void Bob::Http::Request::RouteDeserializer()
{
  size_t start = _AllBuff.find(' ');
  if(start == std::string::npos) 
  {
    _Route = "Unknown";
    return;
  };
  
  size_t end = _AllBuff.find(' ', start + SPACE);
  if(end == std::string::npos)
  {
    _Route = "Unknown";
    return;
  }

  _Route = _AllBuff.substr(start + SPACE, end - start - SPACE);
}

void Bob::Http::Request::HttpVersionDeserializer()
{
    size_t firstSpace = _AllBuff.find(' ');
    if (firstSpace == std::string::npos) {
        _HttpVersion = "Unknown";
        return;
    }

    size_t secondSpace = _AllBuff.find(' ', firstSpace + 1);
    if (secondSpace == std::string::npos) {
        _HttpVersion = "Unknown";
        return;
    }

    size_t endLine = _AllBuff.find("\r\n", secondSpace);
    if (endLine == std::string::npos) endLine = _AllBuff.size();

    _HttpVersion = _AllBuff.substr(secondSpace + 1, endLine - (secondSpace + 1));
}


void Bob::Http::Request::HeaderDeserializer()
{
  MethodDeserializer();
  RouteDeserializer();
  HostDeserialize();
  HttpVersionDeserializer();
}

Bob::Http::HttpMethod Bob::Http::Request::GetMethod()
{
  return _Method;
}

void Bob::Http::Request::HostDeserialize()
{
  size_t hostPos = _AllBuff.find("Host: ");
  hostPos += strlen("Host: ");
  size_t hostEndPos = _AllBuff.find("\r\n", hostPos);
  if(hostPos == std::string::npos)
  {
    _Host = "Unknown";
    return;
  }
  _Host = _AllBuff.substr(hostPos, hostEndPos - hostPos); 
}

void Bob::Http::Request::ContentTypeDeserialize()
{
  size_t pos = _AllBuff.find("Content-Type");
  if(pos == std::string::npos)
  {
    _ContentType = Http::ContentType::UnknownOrEmpty;
    return;
  }
  std::string contentType = _AllBuff.substr(pos + strlen("Content-Type"));

  if(contentType == "text/plain")
  {
    _ContentType = Http::ContentType::TextPlain;
    return;
  }
  if(contentType == "text/html")
  {
    _ContentType = Http::ContentType::TextHtml;
    return;
  }
  if(contentType == "application/json")
  {
    _ContentType = Http::ContentType::ApplicationJson;
    return;
  }
  if(contentType == "application/xml")
  {
    _ContentType = Http::ContentType::ApplicationXml;
    return;
  }
  if(contentType == "application/octet-stream")
  {
    _ContentType = Http::ContentType::ApplicationOctetStream;
    return;
  }
  if(contentType == "image/png")
  {
    _ContentType = Http::ContentType::ImagePng;
    return;
  }
  if(contentType == "image/jpeg")
  {
    _ContentType = Http::ContentType::ImageJpeg;
    return;
  }
  if(contentType == "multipart/form-data")
  {
    _ContentType = Http::ContentType::MultipartFormData;
    return;
  }
  _ContentType = Http::ContentType::UnknownOrEmpty;
}

void Bob::Http::Request::ContentLengthDeserialize()
{
  size_t pos = _AllBuff.find("Content-Length");
  if(pos == std::string::npos)
  {
    _ContentLength = 0;
    return;
  }
  std::string value = _AllBuff.substr(pos + strlen("Content-Length"));
  _ContentLength = std::stoi(value);
}
