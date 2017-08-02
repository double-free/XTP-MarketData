#pragma once

#include<vector>
#include<queue>
#include<thread>
#include<functional>
#include<mutex>
#include<condition_variable>


class ThreadPool {
public:
  explicit ThreadPool (int thread_num);
  ~ThreadPool();
  void doJob(std::function<void (void)> f);
protected:
  void threadEntry(int i);
private:
  bool shutdown_;
  std::vector<std::thread> threads_;
  // template <class Ret, class... Args> class function<Ret(Args...)>;
  std::queue<std::function <void (void)> > tasks_;  // return void, paras void

  std::mutex mu_;
  std::condition_variable cond_;
};
