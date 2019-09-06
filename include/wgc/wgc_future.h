#ifndef WGC_FUTURE_H
#define WGC_FUTURE_H

#include <future>
#include <chrono>

namespace wgc
{
  template<typename T>
  bool FutureReady(const std::future<T>& Future)
  {
    return 
      Future.valid() 
      && (Future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready);
  }

}

#endif