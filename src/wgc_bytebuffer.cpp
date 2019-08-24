#include "wgc/wgc_bytebuffer.h"

namespace wgc
{
  std::vector<uint8_t> ByteBuffer::GetBytes()
  {
    std::streamsize bof = Buffer.tellg();
    Buffer.seekg(0, std::ios::end);
    size_t Size = static_cast<size_t>(Buffer.tellg() - bof);
    Buffer.seekg(0, std::ios::beg);

    std::vector<uint8_t> Bytes(Size);
    Buffer.read(reinterpret_cast<char*>(Bytes.data()), static_cast<std::streamsize>(Bytes.size()));
    return Bytes;
  }

  std::stringstream& ByteBuffer::AsStringStream()
  {
    return Buffer;
  }

  void ByteBuffer::Write(const char* Data)
  {
    Buffer.write(Data, strlen(Data));
  }

  void ByteBuffer::Write(const std::string& Data)
  {
    Write(Data.c_str());
  }

  ByteBuffer& ByteBuffer::operator<<(const char* Data)
  {
    Write(Data);
    return *this;
  }

  ByteBuffer& ByteBuffer::operator<<(const std::string& Data)
  {
    Write(Data);
    return *this;
  }

}
