#pragma once

#include<iostream>
#include<vector>
#include<queue>
#include<mutex>
#include<future>
#include<functional>
#include<thread>

//template<typename T>
class Pool
{
    using Task = std::function<void(void)>;

    std::vector<std::thread> pool;

    std::queue<Task> tasks;

    std::mutex m_lock;

    std::condition_variable cv_task;

    std::atomic<bool> close;

    std::atomic<int> threading_num;
public:
    Pool(int size = 10) : close(false)
    {
        threading_num = size < 1 ? 1 :size;
        for(int iter_size = 0; iter_size < threading_num; ++iter_size)
        {
            //std::cout << "thre:  " << threading_num << std::endl;
            pool.emplace_back(
                [this]
                {
                    while(!this->close)
                    {
                        std::function<void(void)> task;
                        {
                            std::unique_lock<std::mutex> lock { this->m_lock };

                            this->cv_task.wait(lock, [this] { return this->close.load() || !this->tasks.empty(); } );

                            if(this->close.load() && this->tasks.empty())
                                return ;
                            
                            task = std::move(this->tasks.front());
                            tasks.pop();
                        }
                        threading_num--;
                        task();
                        threading_num++;
                    }
                }
            );
        }
    }
    ~Pool()
    {
        close.store(true);
        cv_task.notify_all();
        for(auto& thread : pool)
            if(thread.joinable())
                thread.join();
    }

    template<typename F, typename... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        if(close.load())
            throw std::runtime_error("it is close");
        using RetType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<RetType()>> (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<RetType> future = task->get_future();
        {
            std::lock_guard<std::mutex> lock { m_lock };
            tasks.emplace(
                [task]() { (*task)(); }
            );
        }
        cv_task.notify_one();

        return future;
    }

    int get_id() { return threading_num; }

};
