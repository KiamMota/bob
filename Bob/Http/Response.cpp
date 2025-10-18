#include "Response.hpp"

Bob::Http::Response::Response(int Code)
{
  _RawCode = (Bob::Http::HttpStatus)Code;
}

Bob::Http::Response::Response(Bob::Http::HttpStatus Code)
{
  _RawCode = Code;
}

void Bob::Http::Response::_StartHeaders()
{
  _Code = _RawCodeToString();
  _Buf << "HTTP/1.1" + _Code + "\r\n";
  _Buf << "Server: Bob\r\n";
}

std::string Bob::Http::Response::_RawCodeToString()
{
   switch (_RawCode) {
    case Http::HttpStatus::Continue: return "100 Continue";
    case Http::HttpStatus::SwitchingProtocols: return "101 Switching Protocols";

    case Http::HttpStatus::OK: return "200 OK";
    case Http::HttpStatus::Created: return "201 Created";
    case Http::HttpStatus::Accepted: return "202 Accepted";
    case Http::HttpStatus::NoContent: return "204 No Content";

    case Http::HttpStatus::MovedPermanently: return "301 Moved Permanently";
    case Http::HttpStatus::Found: return "302 Found";
    case Http::HttpStatus::SeeOther: return "303 See Other";
    case Http::HttpStatus::NotModified: return "304 Not Modified";

        /* 4xx client */
    case Http::HttpStatus::BadRequest: return "400 Bad Request";
    case Http::HttpStatus::Unauthorized: return "401 Unauthorized";
    case Http::HttpStatus::Forbidden: return "403 Forbidden";
    case Http::HttpStatus::NotFound: return "404 Not Found";
    case Http::HttpStatus::MethodNotAllowed: return "405 Method Not Allowed";
    case Http::HttpStatus::RequestTimeout: return "408 Request Timeout";

        /* 5xx server error */
    case Http::HttpStatus::InternalServerError: return "500 Internal Server Error";
    case Http::HttpStatus::NotImplemented: return "501 Not Implemented";
    case Http::HttpStatus::BadGateway: return "502 Bad Gateway";
    case Http::HttpStatus::ServiceUnavailable: return "503 Service Unavailable";
    case Http::HttpStatus::GatewayTimeout: return "504 Gateway Timeout";

        default: return "Unknown";
    }

}

