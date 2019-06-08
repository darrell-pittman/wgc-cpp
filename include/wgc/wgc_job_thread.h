#ifndef WGC_JOB_THREAD_H
#define WGC_JOB_THREAD_H

#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <string>

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
    Task Job;
    std::thread Thread;
  };
}

#endif
