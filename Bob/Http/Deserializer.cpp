#include "Deserializer.hpp"
#include <cstring>

#define SPACE 1

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
  if(pos == std::string::npos) return "Unknown";
  return _allBuffer.substr(0, pos);
}

std::string Bob::Http::Deserializer::DeserializeRoute()
{

  size_t start = _allBuffer.find(' ');
  if(start == std::string::npos) 
  {
    return "Unknown";
  };
  
  size_t end = _allBuffer.find(' ', start + SPACE);

  return _allBuffer.substr(start + SPACE, end - start - SPACE);
}

std::string Bob::Http::Deserializer::DeserializeVersion()
{

    size_t firstSpace = _allBuffer.find(' ');
    if (firstSpace == std::string::npos) {
        return "Unknown";
    }

    size_t secondSpace = _allBuffer.find(' ', firstSpace + 1);
    if (secondSpace == std::string::npos) {
        return "Unknown";
    }

    size_t endLine = _allBuffer.find("\r\n", secondSpace);
    if (endLine == std::string::npos) endLine = _allBuffer.size();

    return _allBuffer.substr(secondSpace + 1, endLine - (secondSpace + 1));
}

std::string Bob::Http::Deserializer::DeserializeHost()
{

  size_t hostPos = _allBuffer.find("Host: ");
  hostPos += strlen("Host: ");
  size_t hostEndPos = _allBuffer.find("\r\n", hostPos);
  if(hostPos == std::string::npos)
  {
    return "Unknown";
  }
  return _allBuffer.substr(hostPos, hostEndPos - hostPos); 
}

std::string Bob::Http::Deserializer::DeserializeContentLength()
{ 
  size_t pos = _allBuffer.find("Content-Length");
  if(pos == std::string::npos)
  {
    return "0";
  }
  return _allBuffer.substr(pos + strlen("Content-Length"));
}

std::string Bob::Http::Deserializer::DeseriaizeContentType()
{

  size_t pos = _allBuffer.find("Content-Type");
  if(pos == std::string::npos)
  {
    return "0";
  }
  return _allBuffer.substr(pos + strlen("Content-Type"));
}
