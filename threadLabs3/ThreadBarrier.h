#pragma once
#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <atomic>
class ThreadBarrier
{
private:
    mutable size_t _threadCount;
    std::atomic<size_t> _threadCountWait;
    size_t _threadCountFree;
    std::condition_variable _cv;
    std::mutex _mutex;
public:
    ThreadBarrier(const size_t& threadCount);
    void wait();
};
ThreadBarrier::ThreadBarrier(const size_t& threadCount) :
_threadCount(threadCount),_threadCountFree(0), _threadCountWait(0) {}
void ThreadBarrier::wait()
{
         std::unique_lock<std::mutex> lock(_mutex); 

        ++_threadCountWait;

        if (_threadCountWait >= _threadCount)
        {
            _threadCountFree = _threadCount - 1;
            _threadCountWait = 0;
            _cv.notify_all();
        }
        else {
            _cv.wait(lock, [this]()
                {
                    return _threadCountWait == 0;
                });

            --_threadCountFree;
        }
    
}


