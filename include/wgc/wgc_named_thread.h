#ifndef WGC_NAMED_THREAD_H
#define WGC_NAMED_THREAD_H

#include <map>
#include <string>
#include <thread>
#include <utility>

namespace wgc
{
  extern const std::string& ThisThreadName();

  class NamedThread : public std::thread
  {
  public:
    NamedThread(const std::string& Name) noexcept;
    NamedThread(NamedThread&) = delete;
    NamedThread(const NamedThread&) = delete;
    NamedThread(const NamedThread&&) = delete;
    NamedThread(NamedThread&& Other);

    template <typename F, typename... Ts>
    explicit NamedThread(const std::string& Name, F&& Func, Ts&&... Args)
      : std::thread(std::forward<F>(Func), std::forward<Ts>(Args)...)
    {
      ThreadNames[get_id()] = Name;
    }

    const std::string& GetName();

  private:
    friend const std::string& ThisThreadName();
    static const std::string& _GetName(std::thread::id);
    static std::map<std::thread::id, std::string> ThreadNames;
  };

} // namespace wgc

#endif