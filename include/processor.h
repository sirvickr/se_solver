#pragma once

#include "async_queue.h"
#include "quadratic_equation.h"
#include "result_manager.h"
#include "task.h"

#include <ios>
#include <mutex>
#include <thread>
#include <vector>

// Asynchronously processes a sequence of quadratic equations solver tasks
class QuadraticEquationsProcessor
{
public:
    QuadraticEquationsProcessor();
    ~QuadraticEquationsProcessor();
    // Solve the equations and print the results
    void run(Tasks &&tasks);

private:
    // Output precision
    static constexpr std::streamsize PRECISION = 15;
    const std::streamsize prev_precision;

    // A thread-safe tasks queue
    AsyncQueue<Task> tasks_queue;
    // Pool of worker threads to serve tasks from the queue
    std::vector<std::thread> worker_threads;

    // Arranges asynchronous results in the original order
    ResultManager result_manager;

private:
    // Worker thread
    void worker();

    // Wait for all tasks to complete
    void waitCompletion();
};
