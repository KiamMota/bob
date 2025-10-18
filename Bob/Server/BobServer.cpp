#include "BobServer.hpp"
#include "Http/Enums.hpp"
#include "Http/Request.hpp"
#include "Http/Response.hpp"
#include "uv.h"
#include "uv/unix.h"
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <uv.h>
#include <uv/unix.h>

Bob::BobServer::BobServer(const char* ip, ushort port)
{
  _Ip = ip;
  _Port = port;
  _messageNotAllowed = "HTTP/1.1 405 Method Not Allowed\r\nServer: Bob\r\n";
}

int Bob::BobServer::Run()
{
  InitLoop();
  InfoListening();
  PrepareServer();
  return uv_run(_mainLoop, UV_RUN_DEFAULT);
}

Bob::BobServer& Bob::BobServer::AddController(const char* route, Bob::Http::HttpMethodEnum Method, std::function<Bob::Http::Response(Bob::Http::Request&)> callback)
{
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

void Bob::BobServer::DefaultCallbackConnection(uv_stream_t* stream, int status)
{
    /* data to get the object */
    BobServer* self = (BobServer*)stream->data;
    std::cout << "* Client Connected" << std::endl;

    /* represents the client */
    uv_tcp_t* client_handle = new uv_tcp_t;
    
    uv_tcp_init(self->_mainLoop, client_handle);
  
    client_handle->data = self;
    /* if connection is not accepted */
    if (uv_accept(stream, (uv_stream_t*)client_handle) != 0) {
        std::cout << "* Not Accepted Connection! " << std::endl;
        uv_close((uv_handle_t*)client_handle, [](uv_handle_t* h){ delete (uv_tcp_t*)h; });
        return;
    }
   
    uv_read_start((uv_stream_t*)client_handle, self->AllocBufferCb, self->ReadBufferCb);
}

void Bob::BobServer::AllocBufferCb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buffer)
{
    if(suggested_size <= 0)
    {
      std::cout << "* No Data " << std::endl;
      suggested_size = 1024;
      std::cout << "* Adjusted Size " << std::endl;
    }
    std::cout << "Alocated Buffer" << std::endl;
    buffer->len = suggested_size;
    buffer->base = new char[buffer->len];
}

void Bob::BobServer::WriteCb(uv_write_t* write, int status)
{
  if(status < 0) std::cout << "error:" << uv_strerror(status); 
  delete write;
}

void Bob::BobServer::ReadBufferCb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buffer) {
    
    if(nread == UV_EOF && !uv_is_closing((uv_handle_t*)client))
    {
      std::cout << "* Closing Client Connection " << std::endl;
      uv_close((uv_handle_t*)client, nullptr);
      delete[] buffer->base;
      return;
    }

    std::cout << "* Received Data" << std::endl;
    SendResponse(client, buffer);  
    delete[] buffer->base;
}

void Bob::BobServer::SendResponse(uv_stream_t* client, const uv_buf_t* buffer)
{
    /* inits the self object */
    BobServer* self = (BobServer*)client->data;
    std::cout << "* Sending Response" << std::endl;
    uv_write_t* writer = new uv_write_t;
    uv_buf_t responseBuf;
    
    if(!self)
    {
      std::cout << "Failed To Alocate self in stream->data" << std::endl;
      delete self;
      delete writer;
      return;
    };
    Http::Response resp(Http::HttpStatusEnum::Accepted);
    resp.SetConnection(Http::HttpConnectionEnum::Close);
    resp.SetContentType(Http::ContentTypeEnum::ApplicationJson);
    resp.SetBody("{ \"message\": \"hello, friends!\"}");
    resp.Send();
    
    char* messageNotAllowed = strdup(self->_messageNotAllowed);
    responseBuf = uv_buf_init(messageNotAllowed, strlen(messageNotAllowed));
    uv_write(writer, client, &responseBuf, 1, WriteCb);
    uv_close((uv_handle_t*)client, NULL);
}


