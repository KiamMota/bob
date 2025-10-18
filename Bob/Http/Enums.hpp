#ifndef _HTTP_ENUMS_HPP
#define _HTTP_ENUMS_HPP


namespace Bob
{
  namespace Http
  {

    enum class HttpMethod 
    {
      GET = 1,
      POST,
      PUT,
      DELETE,
      PATCH,
      UNKNOWN  
    };

    enum class ContentType
    {
      TextPlain = 1,
      TextHtml,
      ApplicationJson,
      ApplicationXml,
      ApplicationOctetStream,
      ImagePng,
      ImageJpeg,
      MultipartFormData,
      UnknownOrEmpty
    };
    
    enum class HttpStatus {
      Continue = 100,
      SwitchingProtocols = 101,

      OK = 200,
      Created = 201,
      Accepted = 202,
      NoContent = 204,

      MovedPermanently = 301,
      Found = 302,
      SeeOther = 303,
      NotModified = 304,

      BadRequest = 400,
      Unauthorized = 401,
      Forbidden = 403,
      NotFound = 404,
      MethodNotAllowed = 405,
      RequestTimeout = 408,

      InternalServerError = 500,
      NotImplemented = 501,
      BadGateway = 502,
      ServiceUnavailable = 503,
      GatewayTimeout = 504
    };
}
}

#endif
