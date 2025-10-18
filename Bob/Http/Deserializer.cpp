#include "Deserializer.hpp"
#include <cstring>
#include <string>

#define SPACE 1
#define UNK "0"
#define HOST "Host: "
#define CONTENT_LENGTH "Content-Length"
#define CONTENT_TYPE "Content-Type"

Bob::Http::Deserializer::Deserializer(const std::string& data)
{
  _allBuffer = data;
}

Bob::Http::Deserializer::Deserializer(const char* data)
{
  _allBuffer = data;
}

std::string Bob::Http::Deserializer::DeserializeMethod()
{

  char separator = ' ';
  size_t pos = _allBuffer.find(separator);
  if(pos == std::string::npos) return UNK;
  return _allBuffer.substr(0, pos);
}

std::string Bob::Http::Deserializer::DeserializeRoute()
{

  size_t start = _allBuffer.find(' ');
  if(start == std::string::npos) 
  {
    return UNK;
  };
  
  size_t end = _allBuffer.find(' ', start + SPACE);

  return _allBuffer.substr(start + SPACE, end - start - SPACE);
}

std::string Bob::Http::Deserializer::DeserializeVersion()
{

    size_t firstSpace = _allBuffer.find(' ');
    if (firstSpace == std::string::npos) {
        return UNK;
    }

    size_t secondSpace = _allBuffer.find(' ', firstSpace + 1);
    if (secondSpace == std::string::npos) {
        return UNK;
    }

    size_t endLine = _allBuffer.find("\r\n", secondSpace);
    if (endLine == std::string::npos) endLine = _allBuffer.size();

    return _allBuffer.substr(secondSpace + 1, endLine - (secondSpace + 1));
}

std::string Bob::Http::Deserializer::DeserializeHost()
{

  size_t hostPos = _allBuffer.find(HOST);
  hostPos += strlen(HOST);
  size_t hostEndPos = _allBuffer.find("\r\n", hostPos);
  if(hostPos == std::string::npos)
  {
    return UNK;
  }
  return _allBuffer.substr(hostPos, hostEndPos - hostPos); 
}

std::string Bob::Http::Deserializer::DeserializeContentLength()
{ 
  size_t pos = _allBuffer.find(CONTENT_LENGTH);
  if(pos == std::string::npos)
  {
    return "0";
  }
  return _allBuffer.substr(pos + strlen(CONTENT_LENGTH));
}

std::string Bob::Http::Deserializer::DeseriaizeContentType()
{

  size_t pos = _allBuffer.find(CONTENT_TYPE);
  if(pos == std::string::npos)
  {
    return "0";
  }
  return _allBuffer.substr(pos + strlen(CONTENT_TYPE));
}

std::string Bob::Http::Deserializer::DeserializeBody()
{
  auto pos = _allBuffer.find("\r\n\r\n");
  if (pos != std::string::npos)
    return _allBuffer.substr(pos + 4);
  return UNK;
}
