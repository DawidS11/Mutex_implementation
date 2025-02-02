#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

class Mutex
{
private:
    std::atomic<bool> flag;

public:
    Mutex() 
        : flag(false) 
    {}

    void lock()
    {
        while (flag.exchange(true, std::memory_order_acquire))
        {
            // Spinlock until the lock is acquired.
        }
    }

    void unlock()
    {
        flag.store(false, std::memory_order_release);
    }
};

int counter = 0;
Mutex mMutex;

void inc(int numIterations)
{
    for (int i = 0; i < numIterations; ++i)
    {
        mMutex.lock();
        ++counter;
        mMutex.unlock();
    }
}

void dec(int numIterations)
{
    for (int i = 0; i < numIterations; ++i)
    {
        mMutex.lock();
        --counter;
        mMutex.unlock();
    }
}

int main() {
    const int numThreads = 5;
    const int numInc = 1000;
    const int numDec = 500;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(inc, numInc);
        threads.emplace_back(dec, numDec);
    }

    for (auto& t : threads)
    {
        t.join();
    }
    
    if (numThreads * (numInc - numDec) == counter)
    {
        std::cout << "OK" << std::endl;
    }

}