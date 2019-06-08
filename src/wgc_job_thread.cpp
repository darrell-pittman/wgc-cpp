#include "wgc/wgc_job_thread.h"

namespace wgc
{
  void JobThread::operator()()
  {
    while(Alive)
    {
      std::unique_lock<std::mutex> Lock(Guard);
      Condition.wait(Lock, [this]{return !Alive || Executing;});
      if(Alive)
      {
          Job();
      }
      Executing = false;
      Condition.notify_one();      
    }
  }

  void JobThread::RunJob(Task InJob)
  {
    std::unique_lock<std::mutex> Lock(Guard);
    Job = InJob;
    if(!Alive)
    {
      Alive = true;
      Thread = std::thread(std::ref(*this));
    }
    Executing = true;
    Condition.notify_one();
  }

  void JobThread::Wait()
  {
    std::unique_lock<std::mutex> Lock(Guard);
    Condition.wait(Lock, [this]{return !Alive || !Executing;});
    Condition.notify_one();    
  }

  void JobThread::Join()
  {
    Thread.join();
  }

  void JobThread::Stop(const bool BlockUntilStopped)
  {
      Wait();
      {
        std::lock_guard<std::mutex> Lock(Guard);
        Alive = false;
        Condition.notify_one();
      }
      if(BlockUntilStopped)
      {
        Join();
      }
  }

  JobThread::~JobThread()
  {
    Stop(true);
  }
}
