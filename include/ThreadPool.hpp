#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:

    ThreadPool(size_t numThreads) : m_stop(false) {

        for(size_t i = 0; i < numThreads; ++i) {


            m_workers.emplace_back([this] {


                while(true) {
                    std::function<void()> task;


                    {

                        std::unique_lock<std::mutex> lock(this->m_queueMutex);

                        this->m_condition.wait(lock, [this]{
                            return this->m_stop || !this->m_tasks.empty();
                        });

                        if(this->m_stop && this->m_tasks.empty()) {
                            return;
                        }


                        task = std::move(this->m_tasks.front());
                        this->m_tasks.pop();
                    }

                    task();
                }
            });
        }
    }


    ~ThreadPool() {

        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_stop = true;
        }


        m_condition.notify_all();

        for(std::thread &worker : m_workers) {
            worker.join();
        }
    }


    void enqueue(std::function<void()> task) {

        {
            std::unique_lock<std::mutex> lock(m_queueMutex);

            if(m_stop) {
                return;
            }

            m_tasks.emplace(std::move(task));
        }

        m_condition.notify_one();
    }


private:
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    bool m_stop;
};