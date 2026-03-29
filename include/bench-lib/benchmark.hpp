#ifndef BENCH_LIB_BENCHMARK_HPP
#define BENCH_LIB_BENCHMARK_HPP

#include <vector>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <filesystem>
#include "benchmark_builder.hpp"

namespace BenchLib
{
  class Benchmark
  {
  public:
    template <typename F, typename... Args>
    BenchmarkBuilder add(const std::string &explanation, F &&f, Args &&...args) {
      auto args_tuple = std::make_tuple(std::forward<Args>(args)...);
      BenchmarkCase bc;
      bc.explanation = explanation;
      bc.func = [func = std::forward<F>(f), args_tuple]() mutable -> std::any {
        if constexpr (std::is_void_v<std::invoke_result_t<F, Args...>>) {
          std::apply(func, args_tuple);
          return std::any{};
        }
        else {
          return std::any(std::apply(func, args_tuple));
        }
      };
      tasks.push_back(std::move(bc));
      return BenchmarkBuilder(tasks.back());
    }

    template <typename T>
    T get_result(size_t index) const {
      return std::any_cast<T>(tasks[index].result);
    }

    void run(int iterations = 128, int cache_warmup = 64);
    bool write_csv(std::string csv_name);
    bool append_csv(std::string csv_name);

  private:
    std::vector<BenchmarkCase> tasks;
  };
}

#endif