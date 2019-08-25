#ifndef WGC_MEMORY_H
#define WGC_MEMORY_H

#include <memory>

namespace wgc
{
  template<typename Base, typename Derived>
  std::unique_ptr<Base> StaticUniquePtrCast(std::unique_ptr<Derived>&& From)
  {
    return std::unique_ptr<Base>{static_cast<Base*>(From.release())};
  }
}

#endif