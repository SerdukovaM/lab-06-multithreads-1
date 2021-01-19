// Copyright 2020 Yulia Timoshenko <timoshenkojulie01@gmail.com>

#include "a_sha_worker.hpp"

#include <boost/log/trivial.hpp>
#include <ctime>
#include <iostream>
#include <thread>

#include "picosha2.h"

a_sha_worker::a_sha_worker(size_t n_thr, size_t N, bool autorun) {
  if (n_thr > std::thread::hardware_concurrency() || n_thr <= 0) {
    throw std::invalid_argument(
        "n_threads specified exceeds hardware concurrency or is less than or "
        "equal to zero");
  };
  n_threads = n_thr;
  n_zeros = N;
  cancelled = false;
  srand(std::time(nullptr));
  if (autorun) start();
}
void a_sha_worker::start() {
  for (size_t i = 0; i < n_threads; ++i) {
    pool.emplace_back(&a_sha_worker::perform_checks, this);
  }
}
void a_sha_worker::perform_checks() {
  while (!cancelled) {
    std::string s = std::to_string(std::rand());
    std::string hash;
    picosha2::hash256_hex_string(s, hash);
    BOOST_LOG_TRIVIAL(trace) << " Thread: " << std::this_thread::get_id()
                             << " Data: " << s << " Hash: " << hash;
    bool missed = false;
    for (size_t i = 0; i < n_zeros; ++i) {
      if (hash.c_str()[hash.size() - i - 1] != '0') {
        missed = true;
        break;
      }
    }
    if (!missed) {
      BOOST_LOG_TRIVIAL(info) << hash;
      std::scoped_lock<std::mutex> lock(res_mutex);
      results.push_back(nlohmann::json{
          {"timestamp", std::time(nullptr)}, {"hash", hash}, {"data", s}});
    }
  }
}
void a_sha_worker::await() {
  for (auto& t : pool) {
    t.join();
  }
  pool.resize(0);
}

void a_sha_worker::cancel() { cancelled = true; }

void a_sha_worker::dump_json(const std::string& path) {
  std::ofstream res{path};
  res << results.dump(4);
  res.close();
}
