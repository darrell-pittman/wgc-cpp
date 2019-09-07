#ifndef WGC_FUTURE_H
#define WGC_FUTURE_H

#include <chrono>
#include <future>

namespace wgc
{
  template <typename T>
  bool FutureReady(const std::future<T>& Future)
  {
    return Future.valid() &&
           (Future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready);
  }

  template <typename F, typename... Ts>
  auto Async(F&& Function, Ts&&... Args)
  {
    return std::async(std::launch::async, std::forward<F>(Function), std::forward<Ts>(Args)...);
  }

} // namespace wgc

#endif