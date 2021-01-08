//
// Created by mikhail on 1/8/21.
//

#ifndef TEMPLATE_WORKER_CPP
#define TEMPLATE_WORKER_CPP

#include <csignal>
#include "a_sha_worker.cpp"
#include <boost/log/core.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions.hpp>



a_sha_worker* gp_worker;

void handle(int s) {
  if (s==SIGINT) {
    gp_worker->cancel();
  }
}

int main(int argc, char* argv[]) {
  if ((argc!=2) and (argc!=3)) {
    std::cout << "worker <json results path> <number of threads>";
    return 0;
  }
  boost::log::add_file_log(
        boost::log::keywords::file_name = "async_worker_%5N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::format = "[%TimeStamp%][%Severity%]: %Message%"
      );
  boost::log::add_console_log(std::cout,
        boost::log::keywords::format = "[%TimeStamp%][%Severity%]: %Message%"
      );
  size_t n_zeros = 4;
  size_t n_threads = std::thread::hardware_concurrency();
  std::string json_path = argv[1];
  if (argc == 3) {
    n_threads = std::atoi(argv[2]);
  }
  a_sha_worker w{n_threads, n_zeros, false};
  gp_worker = &w;
  std::signal(SIGINT, handle);
  w.start();
  w.await();
  w.dump_json(json_path);
  delete gp_worker;
  return 0;
}

#endif