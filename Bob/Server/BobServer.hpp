#ifndef _BOBSERVER_HPP_
#define _BOBSERVER_HPP_

#include "Http/Enums.hpp"
#include "Http/Request.hpp"
#include "Http/Response.hpp"
#include "uv.h"
#include <functional>
#include <netinet/in.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace Bob
{
  class BobServer
  { 
    private:
      int _msTolerance;
      std::string _Ip;
      ushort _Port;
      uv_timer_t* _timerHandle;
      uv_tcp_t* _serverHandle;
      uv_loop_t* _mainLoop;

      const char* _messageNotAllowed;
      std::pmr::unordered_map<std::string /* route */, 
          std::pmr::unordered_map<Http::HttpMethod, std::function<Http::Response(Http::Request&)>>> _RouteMap;

      struct sockaddr_in address;
  
      static void AllocBufferCb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buffer);
      static void ReadBufferCb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buffer);
      static void WriteCb(uv_write_t* req, int status);    
      static void DefaultCallbackConnection(uv_stream_t* stream, int result);

      void PrepareServer();
      void InfoListening();
      void InitLoop();
    public:
      BobServer(const char* ip, ushort port); 
      int Run(); 
      
      BobServer& AddController(const char* route, Bob::Http::HttpMethod Method, std::function<Bob::Http::Response(Bob::Http::Request&)> callback);
      
      BobServer& SetIdleTimeout(int ms);
      ushort GetPort();
      std::string GetIp();
      std::string GetEndpoint();
      std::vector<Bob::Http::Response(Bob::Http::Request&)> _Callbacks();
        
  };
}

#endif
