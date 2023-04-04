#ifndef FUNC_TIMEOUT
#define FUNC_TIMEOUT
#include <chrono>
#include <future>
#include <stdexcept>
#include <functional>
template <class R, class... Args>
std::future<R> call_with_timeout(std::function<R(Args...)> &&func, int timeout_mseconds, Args &&...args)
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
  if (future_result.wait_for(std::chrono::milliseconds(timeout_mseconds)) == std::future_status::timeout)
  {
    throw std::runtime_error("Function execution timed out.");
  }
  return future_result;
}
#endif