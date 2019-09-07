#ifndef WGC_JOB_THREAD_H
#define WGC_JOB_THREAD_H

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace wgc
{
  class JobThread
  {
  public:
    using Task = std::function<void()>;
    JobThread(){};
    JobThread(const JobThread&) = delete;
    JobThread(JobThread&&) = delete;
    JobThread& operator=(const JobThread&) = delete;
    JobThread& operator=(JobThread&&) = delete;

    void operator()();
    void RunJob(Task InJob);
    void Wait();
    void Stop(bool BlockUnitlStopped = true);
    void Join();
    virtual ~JobThread();

  private:
    bool Alive = false;
    std::mutex Guard;
    std::condition_variable Condition;
    bool Executing = false;
    std::queue<Task> Jobs;
    std::thread Thread;
  };
} // namespace wgc

#endif
