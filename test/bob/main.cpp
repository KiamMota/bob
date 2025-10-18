#include "Http/Request.hpp"
#include "Http/Response.hpp"
#include "Server/BobServer.hpp"

Bob::Http::Response funcao(Bob::Http::Request& rq)
{
  Bob::Http::Response resp(200);
  return resp;
}

int main()
{
  Bob::BobServer bob("127.0.0.1", 7192);
  bob.SetIdleTimeout(4000);
  bob.Run();
}
