#ifndef WGC_SEQUENCE_H
#define WGC_SEQUENCE_H

#include "wgc/wgc_memory.h"

#include <type_traits>
#include <limits>
#include <memory>
#include <mutex>

namespace wgc
{
  template<class T, typename std::enable_if_t<std::is_integral_v<T>, T>* = nullptr>
  class Sequence
  {
  private:
    template<class U, typename std::enable_if_t<std::is_integral_v<U>, U>* = nullptr>
    class SyncSequence;
    
  public:
    Sequence(T InitialValue = 0, T MaxValue = std::numeric_limits<T>::max())
      : Init{InitialValue}
      , Max{MaxValue}
      , Current{InitialValue}
    {}
    virtual T Next()
    {
      T Result = Current;
      Current = (Current == Max) ? Init : Current + 1;
      return Result;
    }

    std::unique_ptr<Sequence<T>> Synchronized()
    {
      auto Seq = StaticUniquePtrCast<Sequence<T>>(std::make_unique<SyncSequence<T>>(Init, Max));
      Seq->Current = Current;
      return Seq;
    }

  private:
    T Init;
    T Max;
    T Current;

    template<class U, typename std::enable_if_t<std::is_integral_v<U>, U>*>
    class SyncSequence : public Sequence<U>
    {
    public:
      SyncSequence(U InitialValue = 0, U MaxValue = std::numeric_limits<T>::max())
        : Sequence<U>(InitialValue, MaxValue)
      {} 

      U Next() override
      {
        std::unique_lock<std::mutex> Lock(Guard);
        return Sequence<U>::Next();
      }
    private:
      std::mutex Guard;
    };
  };
}

#endif