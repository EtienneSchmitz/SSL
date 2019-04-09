#include "execution_manager.h"

#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>

rhoban_ssl::Task::~Task()
{
}

rhoban_ssl::ExecutionManager rhoban_ssl::ExecutionManager::execution_manager_singleton_;

rhoban_ssl::ExecutionManager::ExecutionManager()
{
}

rhoban_ssl::ExecutionManager& rhoban_ssl::ExecutionManager::getManager()
{
  return ExecutionManager::execution_manager_singleton_;
}

void rhoban_ssl::ExecutionManager::addTask(rhoban_ssl::Task* t)
{
  add_buffer_.push_back(t);
}

void rhoban_ssl::ExecutionManager::run(double min_loop_duration)
{
  using std::chrono::high_resolution_clock;
  std::vector<Task*> to_remove;
  high_resolution_clock::time_point start;
  long min_loop_d = long(min_loop_duration * 1e9);
  do
  {
    start = high_resolution_clock::now();
    for (auto i : add_buffer_)
    {
      tasks_.push_back(i);
    }
    add_buffer_.clear();
    to_remove.clear();
    for (auto i : tasks_)
    {
      if (i->runTask() == false)
      {
        to_remove.push_back(i);
      }
    }
    for (auto i : to_remove)
    {
      tasks_.remove(i);
      delete i;
    }

    long loop_duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(high_resolution_clock::now() - start).count();
    long d = min_loop_d - loop_duration;
    ;
    if (d > 0)
    {
      // std::cout << d << std::endl;
      std::this_thread::sleep_for(std::chrono::nanoseconds(d));
    }
  } while (tasks_.size() > 0);
}