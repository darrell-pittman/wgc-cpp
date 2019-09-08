#include "wgc/wgc_named_thread.h"

namespace
{
  const std::string NoName = "No Name";
}

namespace wgc
{
  std::map<std::thread::id, std::string> NamedThread::ThreadNames{};

  const std::string& ThisThreadName()
  {
    return NamedThread::_GetName(std::this_thread::get_id());
  }

  NamedThread::NamedThread(NamedThread&& Other)
    : std::thread(static_cast<std::thread&&>(Other))
  {}

  NamedThread::NamedThread(const std::string& Name) noexcept
    : std::thread()
  {
    ThreadNames[get_id()] = Name;
  }

  const std::string& NamedThread::GetName()
  {
    return _GetName(get_id());
  }

  const std::string& NamedThread::_GetName(std::thread::id Id)
  {
    auto Found = ThreadNames.find(Id);
    if(Found != ThreadNames.end())
    {
      return Found->second;
    }
    return NoName;
  }
} // namespace wgc