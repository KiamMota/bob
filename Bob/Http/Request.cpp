#include "Request.hpp"
#include <string>

#define SPACE 1

Bob::Http::Request::Request(const std::string& Buffer) : _desserializer(Buffer) {}

Bob::Http::Request::Request(const char* Buffer) : _desserializer(Buffer) {}

void Bob::Http::Request::DeserializeAll()
{

}


