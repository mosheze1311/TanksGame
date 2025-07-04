#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>


class ThreadPool
{
private:
    // === Attributes === //
    std::vector<std::thread> work_threads;
    std::queue<std::function<void()>> tasks;

    std::mutex tasks_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop = false;

    // === Thread Adding === //
    void appendThread()
    {
        work_threads.emplace_back(
            [this]()
            {
            while (true) {
                std::function<void()> task;
    
                { // scope for locking
                    std::unique_lock<std::mutex> lock(this->tasks_mutex);
                    this->condition.wait(lock, [this]() {
                        return this->stop || !this->tasks.empty(); // wake conditions
                    });
    
                    if (this->stop && this->tasks.empty())
                        return;
    
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();  // Execute the task
            } });
    }

public:
    // === Constructor === //
    explicit ThreadPool(int num_threads)
    {
        for (int i = 0; i < num_threads; ++i)
            appendThread();
    }

    // === Destructor === //
    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(tasks_mutex);
            stop = true;
        }
        condition.notify_all();

        for (std::thread &worker : work_threads)
            if (worker.joinable())
                worker.join();
    }

    // === Copy and Move Constructors === //
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    // === Enqueue === //
    void enqueueTask(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> lock(tasks_mutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }
};
