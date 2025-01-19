#include <iostream>

#include "result_manager.h"

void ResultManager::push(QuadraticEquation::Result result)
{
    std::lock_guard lock(m);
    // Store the result in the priority queue
    results.push(std::move(result));
    // Try to output any results that are ready
    process();
}

void ResultManager::process()
{
    // Keep outputting results as long as we have the next one in sequence
    while (!results.empty() && results.top().number == next_num)
    {
        std::cout << results.top() << '\n';
        results.pop();
        ++next_num;
    }
}
