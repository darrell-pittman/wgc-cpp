#ifndef WGC_BYTEBUFFER_H
#define WGC_BYTEBUFFER_H

#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace wgc
{
  template <typename T>
  std::vector<uint8_t> GetVectorBytes(const std::vector<T> Data)
  {
    size_t Size = Data.size() * sizeof(typename std::vector<T>::value_type);
    std::vector<uint8_t> Bytes(Size);
    memcpy(Bytes.data(), Data.data(), Size);
    return Bytes;
  }

  class ByteBuffer
  {
  public:
    std::vector<uint8_t> GetBytes();
    std::stringstream& AsStringStream();

    template <class T,
              typename std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, T>* = nullptr>
    void Write(const T& Data)
    {
      Buffer.write(reinterpret_cast<const char*>(&Data), sizeof(Data));
    }

    void Write(const char* Data);
    void Write(const std::string& Data);

    template <class T,
              typename std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, T>* = nullptr>
    void Write(const std::vector<T>& Data)
    {
      std::vector<uint8_t> Bytes = GetVectorBytes(Data);
      Buffer.write(reinterpret_cast<const char*>(Bytes.data()),
                   static_cast<std::streamsize>(Bytes.size()));
    }

    template <class T,
              typename std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, T>* = nullptr>
    ByteBuffer& operator<<(const T& Data)
    {
      Write(Data);
      return *this;
    }

    ByteBuffer& operator<<(const char* Data);
    ByteBuffer& operator<<(const std::string& Data);

    template <class T,
              typename std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, T>* = nullptr>
    ByteBuffer& operator<<(const std::vector<T>& Data)
    {
      Write(Data);
      return *this;
    }

  private:
    std::stringstream Buffer;
  };
} // namespace wgc

#endif