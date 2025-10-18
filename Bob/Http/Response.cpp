#include "Response.hpp"
#include "Http/Enums.hpp"
#include <iostream>
#include <string>

Bob::Http::Response::Response(int Code) : _haveBody(false) 
{
  _RawCode = (Bob::Http::HttpStatusEnum)Code;
  _StartHeaders();
}

Bob::Http::Response::Response(Bob::Http::HttpStatusEnum Code) : _haveBody(false)  
{
  _RawCode = Code;
  _StartHeaders();
}

void Bob::Http::Response::_StartHeaders()
{
  _firstHeader = "HTTP/1.1 " + _RawCodeToString() + "\r\n";
  _firstHeader += "Server: Bob\r\n";
}

void Bob::Http::Response::SetConnection(Bob::Http::HttpConnectionEnum connectionMode)
{
    switch(connectionMode)
    {
        case Http::HttpConnectionEnum::Close:
            _connection = "Connection: close\r\n";
            break;
        case Http::HttpConnectionEnum::ProxyConnection:
            _connection = "Connection: proxy-connection\r\n";
            break;
        case Http::HttpConnectionEnum::KeepAlive:
            _connection = "Connection: keep-alive\r\n";
            break;
        case Http::HttpConnectionEnum::Upgrade:
            _connection = "Connection: upgrade\r\n";
            break;
        default:
            break;
    }
}

void Bob::Http::Response::SetContentType(Bob::Http::ContentTypeEnum cType)
{
    switch(cType)
    {
        case ContentTypeEnum::ApplicationJson:
            _contentType = "Content-Type: application/json\r\n";
            break;
        case ContentTypeEnum::ApplicationXml:
            _contentType = "Content-Type: application/xml\r\n";
            break;
        case ContentTypeEnum::ApplicationOctetStream:
            _contentType = "Content-Type: application/octet-stream\r\n";
            break;
        case ContentTypeEnum::ImageJpeg:
            _contentType = "Content-Type: image/jpeg\r\n";
            break;
        case ContentTypeEnum::ImagePng:
            _contentType = "Content-Type: image/png\r\n";
            break;
        case ContentTypeEnum::TextHtml:
            _contentType = "Content-Type: text/html\r\n";
            break;
        default:
            break;
    }
}

std::string Bob::Http::Response::_RawCodeToString()
{
   switch (_RawCode) {
    default: return "Unknown";
    case Http::HttpStatusEnum::Continue: return "100 Continue";
    case Http::HttpStatusEnum::SwitchingProtocols: return "101 Switching Protocols";

    case Http::HttpStatusEnum::OK: return "200 OK";
    case Http::HttpStatusEnum::Created: return "201 Created";
    case Http::HttpStatusEnum::Accepted: return "202 Accepted";
    case Http::HttpStatusEnum::NoContent: return "204 No Content";

    case Http::HttpStatusEnum::MovedPermanently: return "301 Moved Permanently";
    case Http::HttpStatusEnum::Found: return "302 Found";
    case Http::HttpStatusEnum::SeeOther: return "303 See Other";
    case Http::HttpStatusEnum::NotModified: return "304 Not Modified";

        /* 4xx client */
    case Http::HttpStatusEnum::BadRequest: return "400 Bad Request";
    case Http::HttpStatusEnum::Unauthorized: return "401 Unauthorized";
    case Http::HttpStatusEnum::Forbidden: return "403 Forbidden";
    case Http::HttpStatusEnum::NotFound: return "404 Not Found";
    case Http::HttpStatusEnum::MethodNotAllowed: return "405 Method Not Allowed";
    case Http::HttpStatusEnum::RequestTimeout: return "408 Request Timeout";

        /* 5xx server error */
    case Http::HttpStatusEnum::InternalServerError: return "500 Internal Server Error";
    case Http::HttpStatusEnum::NotImplemented: return "501 Not Implemented";
    case Http::HttpStatusEnum::BadGateway: return "502 Bad Gateway";
    case Http::HttpStatusEnum::ServiceUnavailable: return "503 Service Unavailable";
    case Http::HttpStatusEnum::GatewayTimeout: return "504 Gateway Timeout";

    }
}

void Bob::Http::Response::SetBody(std::string body)
{
  if(_haveBody) return;
  _haveBody = true;
  _body = std::string(body);
  _contentType = "Content-Length: " + std::to_string(_body.size());
}

std::string Bob::Http::Response::Send()
{
  std::string allBuffer;
  allBuffer = _firstHeader;
  allBuffer += _connection;
  allBuffer += _contentType;
  allBuffer += "Content-Length: " + std::to_string(_body.size()); 
  allBuffer += "\r\n\r\n";
  allBuffer += _body;
  return allBuffer;
}

