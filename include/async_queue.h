#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

// A thread-safe asynchronous queue
// (asynchronously links Producer and Consumer agents)
template <typename T>
class AsyncQueue
{
private:
    // Underlying FIFO container adaptor (not thread-safe)
    std::queue<T> queue;
    // Mutex for the underlying queue
    mutable std::mutex m;
    // Condition to monitor the queue's not empty state
    std::condition_variable not_empty;
    // Flag indicating that no more items will be added
    bool finished{};

public:
    // Add new item to the queue
    void push(T item)
    {
        // Protect the internal queue
        std::unique_lock lock(m);
        // Add the specified item to the end of the underlying queue
        queue.push(std::move(item));
        // Spare threads of potential redundant lock/unlock cycles
        lock.unlock();
        // Notify one of the waiting threads of a new task in the queue
        not_empty.notify_one();
    }

    // Retrieves an item from the queue
    // Returns:
    //   true if an item was successfully retrieved
    //   false if the queue has been shut down
    bool pop(T &item)
    {
        // Protect the internal queue
        std::unique_lock lock(m);
        // Wait until queue has items or is finished
        not_empty.wait(lock, [this]()
                       { return !queue.empty() || finished; });

        if (queue.empty())
            // Shutdown detected after processing all messages
            return false;

        // Extract and return the first item
        item = std::move(queue.front());
        queue.pop();

        return true;
    }

    // Mark the queue as finished - no more items will be added
    void shutdown()
    {
        std::lock_guard lock(m);
        finished = true;
        not_empty.notify_all();
    }
};
