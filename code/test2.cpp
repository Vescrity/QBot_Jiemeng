#include <iostream>
#include <chrono>
#include <functional>
#include <future>
#include <stdexcept>
#include <stdlib.h>
#include <windows.h>
using namespace std;
template <class R, class... Args>
std::future<R> call_with_timeout(std::function<R(Args...)> &&func, int timeout_seconds, Args &&...args)
{
  std::promise<R> promise;
  std::future<R> future_result = promise.get_future();
  std::thread t([&promise, func = std::move(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable
                {
        try {
            std::apply([&promise, &func](auto&&... args) {
                promise.set_value(std::invoke(func, std::forward<decltype(args)>(args)...));
            }, args);
        } catch (...) {
            promise.set_exception(std::current_exception());
        } });
  t.detach(); // Start the thread and detach it
  // Wait for the result or timeout
  if (future_result.wait_for(std::chrono::seconds(timeout_seconds)) == std::future_status::timeout)
  {
    throw std::runtime_error("Function execution timed out.");
  }
  return future_result;
}

std::string my_long_running_function(string x, double y)
{
  Sleep(9999);
  // std::this_thread::sleep_for(std::chrono::seconds(3)); // Do some long running operation
  return "asdf";
}

int main()
{
  try
  {
    auto result = call_with_timeout(std::function<std::string(std::string, double)>(my_long_running_function), 1, "Hello"s, 2.0);
    std::cout << "Result: " << result.get() << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
