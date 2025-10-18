#include "Response.hpp"
#include "Http/Enums.hpp"

Bob::Http::Response::Response(int Code) : haveContent(false)
{
  _RawCode = (Bob::Http::HttpStatusEnum)Code;
}

Bob::Http::Response::Response(Bob::Http::HttpStatusEnum Code) : haveContent(false)
{
  _RawCode = Code;
}

void Bob::Http::Response::_StartHeaders()
{
  _Code = _RawCodeToString();
  _Buf << "HTTP/1.1" + _Code + "\r\n";
  _Buf << "Server: Bob\r\n";
}

void Bob::Http::Response::SetConnection(Bob::Http::HttpConnectionEnum connectionMode)
{
    switch(connectionMode)
    {
        case Http::HttpConnectionEnum::Close:
            _Buf << "Connection: close\r\n";
            break;
        case Http::HttpConnectionEnum::ProxyConnection:
            _Buf << "Connection: proxy-connection\r\n";
            break;
        case Http::HttpConnectionEnum::KeepAlive:
            _Buf << "Connection: keep-alive\r\n";
            break;
        case Http::HttpConnectionEnum::Upgrade:
            _Buf << "Connection: upgrade\r\n";
            break;
        default:
            break;
    }
}

void Bob::Http::Response::SetContentType(Bob::Http::ContentTypeEnum cType)
{
  if(haveContent == true) return;
  haveContent = true;
    switch(cType)
    {
        case ContentTypeEnum::ApplicationJson:
            _Buf << "Content-Type: application/json\r\n";
            break;
        case ContentTypeEnum::ApplicationXml:
            _Buf << "Content-Type: application/xml\r\n";
            break;
        case ContentTypeEnum::ApplicationOctetStream:
            _Buf << "Content-Type: application/octet-stream\r\n";
            break;
        case ContentTypeEnum::ImageJpeg:
            _Buf << "Content-Type: image/jpeg\r\n";
            break;
        case ContentTypeEnum::ImagePng:
            _Buf << "Content-Type: image/png\r\n";
            break;
        case ContentTypeEnum::TextHtml:
            _Buf << "Content-Type: text/html\r\n";
            break;
        default:
            break;
    }
}


std::string Bob::Http::Response::_RawCodeToString()
{
   switch (_RawCode) {
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

        default: return "Unknown";
    }
}


