//
// Created by Юлия Тимошенко on 21.12.2020.
//

#ifndef TEMPLATE_A_SHA_WORKER_HPP
#define TEMPLATE_A_SHA_WORKER_HPP

#include <string>
#include <atomic>
#include <thread>
#include <nlohmann/json.hpp>
class a_sha_worker {
 public:
  a_sha_worker(size_t n_threads, size_t N=4, bool autorun=false) : n_z{N};
  void start();
  void stop();
  void dump_json(const std::string&);
 private:
  std::vector<std::thread> pool{};
  int n_t, n_z;
  void perform_checks();
  nlohmann::json results = nlohmann::json::array();
  std::mutex r_m;
  std::atomic_bool cancelled;
};

#endif  // TEMPLATE_A_SHA_WORKER_HPP
