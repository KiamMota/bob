#include "Http/Request.hpp"
#include "Http/Response.hpp"
#include "Server/BobServer.hpp"
#include <iostream>

Bob::Http::Response funcao(Bob::Http::Request& rq)
{
  std::cout << "corpo da funcao: " << rq.Body() << std::endl;
  Bob::Http::Response resp(200);
  return resp;
}

int main()
{
  Bob::BobServer bob("127.0.0.1", 7192);
  bob.Run();
}
