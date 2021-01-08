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
#include <boost/log/expressions.hpp>



a_sha_worker* gp_worker;

void handle(int s) {
  if (s==SIGINT) {
    gp_worker->cancel();
  }
}

int main(int argc, char* argv[]) {
  if ((argc!=2) and (argc!=3)) {
    throw std::invalid_argument{"./worker <path_to_json> <number of threads>"};
  }
  boost::shared_ptr<boost::log::sinks::text_file_backend> be = boost::make_shared<boost::log::sinks::text_file_backend>(
        boost::log::keywords::file_name = "async_worker_%5N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::format ="[%TimeStamp%]: %Message%"
      );
  boost::log::core::get()->add_sink(boost::shared_ptr<boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend>>(new boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend>(be)));
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