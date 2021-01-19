// Copyright 2020 Yulia Timoshenko <timoshenkojulie01@gmail.com>

#ifndef A_SHA_WORKER_HPP
#define A_SHA_WORKER_HPP

#include <atomic>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

class a_sha_worker {
 public:
  a_sha_worker(size_t n_thr = std::thread::hardware_concurrency(), size_t N = 4,
               bool autorun = true);
  void start();
  void await();
  void cancel();
  void dump_json(const std::string&);

 private:
  std::vector<std::thread> pool{};
  size_t n_threads, n_zeros;
  void perform_checks();
  nlohmann::json results = nlohmann::json::array();
  std::mutex res_mutex;
  std::atomic_bool cancelled;
};

#endif  // A_SHA_WORKER_HPP
