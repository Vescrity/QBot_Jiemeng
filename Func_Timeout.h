#ifndef FUNC_TIMEOUT
#define FUNC_TIMEOUT
#include <functional>
#include <future>
#include <thread>
#include <tuple>
#include <exception>
/// @brief Run a function in limited time.
/// @tparam R return type
/// @tparam ...Args 
/// @param func the function need to run
/// @param timeout_mseconds the given time (ms)
/// @param ...args arguments
/// @return 
template <class R, class... Args>
std::future<R> call_with_timeout(std::function<R(Args...)> func, int timeout_mseconds, Args... args)
{
  std::promise<R> promise;
  std::future<R> future_result = promise.get_future();
  std::atomic_bool finished{false};

  std::thread worker_thread{
      [&promise, func = std::move(func), args = std::make_tuple(std::forward<Args>(args)...), &finished]() mutable
      {
        try
        {
          std::apply(
              [&promise, &func, &finished](auto &&...args)
              {
                promise.set_value(std::invoke(func, std::forward<decltype(args)>(args)...));
                finished = true;
              },
              args);
        }
        catch (...)
        {
          promise.set_exception(std::current_exception());
          finished = true;
        }
      }};

  if (future_result.wait_for(std::chrono::milliseconds(timeout_mseconds)) == std::future_status::timeout)
  {
    if (!finished.exchange(true))
    {
      worker_thread.join();
    }
    if (!future_result.valid())
    {
      throw std::runtime_error("Function execution timed out.");
    }
  }
  else
  {
    worker_thread.join();
  }

  return future_result;
}
#endif