#include "wgc/wgc_job_thread.h"
#include "wgc/wgc_named_thread.h"

namespace wgc
{
  JobThread::JobThread()
  {
    Thread = std::make_unique<std::thread>(std::ref(*this));
  }

  void JobThread::operator()()
  {
    while(Alive)
    {
      std::packaged_task<void()> Job;
      std::unique_lock<std::mutex> Lock(Guard);
      Condition.wait(Lock, [this] { return !(Alive && Jobs.empty()); });
      if(Jobs.empty())
      {
        continue;
      }
      Job = std::move(Jobs.front());
      Jobs.pop();
      Lock.unlock();
      Job();
      Condition.notify_one();
    }
  }

  std::future<void> JobThread::RunJob(Task InJob)
  {
    std::packaged_task<void()> Job(InJob);
    std::future<void> Future = Job.get_future();
    std::unique_lock<std::mutex> Lock(Guard);
    Jobs.push(std::move(Job));
    Lock.unlock();
    Condition.notify_one();
    return Future;
  }

  JobThread::~JobThread()
  {
    RunJob(StopTask);
    if(Thread->joinable())
    {
      Thread->join();
    }
  }
} // namespace wgc
