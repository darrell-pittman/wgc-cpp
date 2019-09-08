#ifndef WGC_JOB_THREAD_H
#define WGC_JOB_THREAD_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <memory>

namespace wgc
{
  class JobThread
  {
  public:
    using Task = std::function<void()>;

    JobThread();
    JobThread(const JobThread&) = delete;
    JobThread(JobThread&&) = delete;
    JobThread& operator=(const JobThread&) = delete;
    JobThread& operator=(JobThread&&) = delete;
    ~JobThread();

    void operator()();
    std::future<void> RunJob(Task InJob);

  private:
    bool Alive{true};
    std::mutex Guard;
    std::condition_variable Condition;
    std::queue<std::packaged_task<void()>> Jobs;
    std::unique_ptr<std::thread> Thread;
    const Task StopTask = [this]() { Alive = false; };
  };
} // namespace wgc

#endif
