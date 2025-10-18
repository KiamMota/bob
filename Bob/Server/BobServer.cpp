#include "BobServer.hpp"
#include "Http/Enums.hpp"
#include "Http/Request.hpp"
#include "Http/Response.hpp"
#include "uv.h"
#include "uv/unix.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <uv.h>

Bob::BobServer::BobServer(const char* ip, ushort port)
{
  _Ip = ip;
  _Port = port;
  _messageNotAllowed = "HTTP/1.1 405 Method Not Allowed\r\n Server: Bob\r\n";
}

int Bob::BobServer::Run()
{
  InitLoop();
  InfoListening();
  PrepareServer();
  return uv_run(_mainLoop, UV_RUN_DEFAULT);
}

Bob::BobServer& Bob::BobServer::AddController(const char* route, Bob::Http::HttpMethod Method, std::function<Bob::Http::Response(Bob::Http::Request&)> callback)
{
  _RouteMap[route][Method] = callback; 
  return *this;
}

std::string Bob::BobServer::GetIp()
{
  return _Ip;
}

ushort Bob::BobServer::GetPort()
{
  return _Port;
}

std::string Bob::BobServer::GetEndpoint()
{
  return _Ip + ":" + std::to_string(_Port);
}

void Bob::BobServer::InfoListening()
{
  std::cout << "Bob's here!" << std::endl;
  std::cout << "[info] : escutando em " << _Ip << "::" << _Port << std::endl;
}

void Bob::BobServer::PrepareServer()
{
  _serverHandle = new uv_tcp_t;
  uv_tcp_init(_mainLoop, _serverHandle);
  uv_ip4_addr(_Ip.c_str(), _Port, &address);
  uv_tcp_bind(_serverHandle, (const struct sockaddr*)&address, 0);
  uv_listen((uv_stream_t*)_serverHandle, 128, DefaultCallbackConnection);
  _serverHandle->data = this;
}

void Bob::BobServer::InitLoop()
{
  _mainLoop = uv_default_loop();
}

void Bob::BobServer::AllocBufferCb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buffer)
{
    if(suggested_size <= 0)
    {
      std::cout << "No Data";
      suggested_size = 1024;
    }
    std::cout << "* Allocated Buffer " << std::endl;
    buffer->len = suggested_size;
    buffer->base = new char[buffer->len];
}

Bob::BobServer& Bob::BobServer::SetIdleTimeout(int ms)
{
 _msTolerance = ms; 
  return *this;
}

void Bob::BobServer::WriteCb(uv_write_t* write, int status)
{
  if(status < 0) std::cout << "error in write:" << uv_strerror(status); 
  delete write;
}

void Bob::BobServer::ReadBufferCb(uv_stream_t* server, ssize_t nread, const uv_buf_t* buffer) {
    
    std::cout << "* Reading Data..." << std::endl;  

    BobServer* self = (BobServer*)server->data;

    if(!self)
    {
      std::cout << "Failed To Alocate self in stream->data" << std::endl;
      delete[] buffer->base;
      return;
    };

    if(nread <= 0) {
        if(nread == UV_EOF) std::cout << "* Stream Disconnected\n";
        else std::cout << "* No Data\n";
        if(buffer->base) delete[] buffer->base;
        if(nread == UV_EOF && !uv_is_closing((uv_handle_t*)server))
            uv_close((uv_handle_t*)server, nullptr);
        return;
    }
    delete[] buffer->base;
}


void Bob::BobServer::DefaultCallbackConnection(uv_stream_t* stream, int status)
{
    BobServer* self = static_cast<BobServer*>(stream->data);
    std::cout << "* Stream Connected : " << std::flush;

    uv_tcp_t* client_handle = new uv_tcp_t;
    uv_tcp_init(self->_mainLoop, client_handle);

    if (uv_accept(stream, (uv_stream_t*)client_handle) != 0) {
        uv_close((uv_handle_t*)client_handle, [](uv_handle_t* h){ delete (uv_tcp_t*)h; });
        std::cout << "* Not Accepted Connection! " << std::endl;
        return;
    }

    // *** CORREÇÃO CRÍTICA ***
    client_handle->data = self; // agora ReadBufferCb pode acessar 'self'

    uv_timer_t* timeoutTimer = new uv_timer_t;
    uv_timer_init(self->_mainLoop, timeoutTimer);
    timeoutTimer->data = client_handle;

    uv_read_start((uv_stream_t*)client_handle, self->AllocBufferCb, self->ReadBufferCb);

    uv_timer_start(timeoutTimer, [](uv_timer_t* t) {
        uv_tcp_t* client = (uv_tcp_t*)t->data;
        if (!uv_is_closing((uv_handle_t*)client))
            uv_close((uv_handle_t*)client, nullptr);
        std::cout << "* Timeout! Connection Closed By Server.  " << std::endl;
        uv_close((uv_handle_t*)t, [](uv_handle_t* h){ delete (uv_timer_t*)h; });
    }, self->_msTolerance, 0);
}

