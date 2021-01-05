//
// Created by Юлия Тимошенко on 21.12.2020.
//

#include "a_sha_worker.hpp"
#include "../third-party/PicoSHA2/picosha2.h"
#include <thread>
#include <ctime>

a_sha_worker::a_sha_worker(size_t n_threads, bool autorun) {
  if(n_threads > std::thread::hardware_concurrency()) {
    throw std::invalid_argument("n_threads specified exceeds hardware concurrency");
  }
  srand(std::time(nullptr));
  if (autorun)
    start();
}
void a_sha_worker::start() {
  for(int i=0;i<n_t;i++){
    pool.emplace_back(&a_sha_worker::perform_checks, this);
  }
}
void a_sha_worker::perform_checks() {
  while(!cancelled){
    std::string s = std::to_string(std::rand());
    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(s.begin(), s.end(), hash.begin(), hash.end());
  }
}
void a_sha_worker::stop() {
  for(auto& t:pool){
    t.join();
  }
  pool.resize(0);
}
