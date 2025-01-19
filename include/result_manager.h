#pragma once

#include "quadratic_equation.h"

#include <queue>
#include <mutex>

class ResultManager
{
public:
    // Store next result in an appropriate order
    void push(QuadraticEquation::Result result);

private:
    // Process already stored results in order
    void process();

private:
    // Priority queue to order results by sequence number
    using ResultQueue = std::priority_queue<
        QuadraticEquation::Result,
        std::vector<QuadraticEquation::Result>,
        // Custom comparator for reverse ordering (smallest number first)
        std::greater<QuadraticEquation::Result>>;

    // Provides ordering results for consistent output
    ResultQueue results;
    // Synchronizes the results queue
    std::mutex m;
    // Tracks which result we should output next
    size_t next_num{};
};
