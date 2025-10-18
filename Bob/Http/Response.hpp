#ifndef _BOBRS_HPP
#define _BOBRS_HPP

#include "Enums.hpp"

#include <sstream>
#include <string>
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
        Bob::Http::HttpStatusEnum _RawCode;
        std::string _Code;
        void _StartHeaders();
        std::string _RawCodeToString();
        bool haveContent;
      public:
  
        Response(int Code);
        Response(Bob::Http::HttpStatusEnum Code);
        void SetContentType(ContentTypeEnum cType);
        void SetConnection(HttpConnectionEnum connectionMode);
        void SetCookie();
        const char* Send();  
    };
  }
}

#endif
