#include "processor.h"

#include <iostream>

using namespace std;

// Save current output precision
QuadraticEquationsProcessor::QuadraticEquationsProcessor() : prev_precision(cout.precision())
{
    // Set increased output precision
    cout.precision(PRECISION);
    // Use hardware concurrency, but leave one core for the producer
    // and main thread. Ensure at least one worker thread.
    size_t num_threads = max(1u,
                             thread::hardware_concurrency() > 2 ? thread::hardware_concurrency() - 2 : 1);

    // Create worker threads
    for (size_t i = 0; i < num_threads; ++i)
    {
        worker_threads.emplace_back(&QuadraticEquationsProcessor::worker, this);
    }
}

QuadraticEquationsProcessor::~QuadraticEquationsProcessor()
{
    waitCompletion();
    // Restore output precision
    cout.precision(prev_precision);
}

void QuadraticEquationsProcessor::run(Tasks &&tasks)
{
    // Solve the equations
    size_t eq_num = 0;
    for (auto task : tasks)
    {
        try
        {
            task.number = eq_num++;
            tasks_queue.push(task);
        }
        catch (const invalid_argument &e)
        {
            cerr << "Error: " << e.what() << "\n";
            continue;
        }
        catch (const exception &e)
        {
            cerr << "Unexpected error: " << e.what() << "\n";
            continue;
        }
    }
}

void QuadraticEquationsProcessor::worker()
{
    Task task{.a = 0, .b = 0, .c = 0, .number = 0};

    while (tasks_queue.pop(task))
    {
        try
        {
            QuadraticEquation eq(task.a, task.b, task.c);
            // Solve the equation and store result with its sequence number
            auto result = eq.solve();
            // Assign the task number to the result for correct output ordering
            result.number = task.number;
            // Send result to ordered output
            result_manager.push(std::move(result));
        }
        catch (const exception &e)
        {
            cerr << "Error processing equation #" << task.number
                 << ": " << e.what() << "\n";
        }
    }
}

void QuadraticEquationsProcessor::waitCompletion()
{
    // Shutdown the queue
    tasks_queue.shutdown();

    // Wait for all worker threads completion
    for (auto &thread : worker_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
