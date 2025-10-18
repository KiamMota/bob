#ifndef _BOBRS_HPP
#define _BOBRS_HPP

#include "Enums.hpp"

#include <sstream>
#include <string>
#include <vector>
namespace Bob
{
  namespace Http
  {
    class Response
    {
      private:
        std::string _ServerName;
        std::string _Date;
        std::ostringstream _Buf;
        std::vector<std::string> _Lines;
        Bob::Http::HttpStatusEnum _RawCode;
        std::string _Code;
        void _StartHeaders();
        std::string _RawCodeToString();
        std::string _firstHeader;
        std::string _contentType;
        std::string _connection;
        std::string _body;
        std::string _contentLength;
        bool _haveBody;     

      public:
  
        Response(int Code);
        Response(Bob::Http::HttpStatusEnum Code);
        void SetContentType(ContentTypeEnum cType);
        void SetConnection(HttpConnectionEnum connectionMode);
        void SetBody(std::string body);
        std::string Send();  
    };
  }
}

#endif
