cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
        HUNTER_CACHE_SERVERS
        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
        CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.271.tar.gz"
        SHA1 "2d6686024529b471e0cc3a4fc07d4d64b62a53ee"
)


# TODO: rename project and delete this comment
project(template)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

hunter_add_package(Boost COMPONENTS filesystem log system thread)
find_package(Boost CONFIG REQUIRED filesystem log log_setup system thread)

hunter_add_package(Boost COMPONENTS log thread system)
find_package(Boost CONFIG REQUIRED log thread system)

hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

# TODO: change template word to project name and delete this comment

add_executable(tests
        # TODO: enum your files and delete this comment
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
        )

add_executable(worker
        # TODO: enum your files and delete this comment
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/worker.cpp
        )

# TODO: change template word to project name and delete this comment
target_include_directories(worker
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/PicoSHA2
        )
target_include_directories(tests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/PicoSHA2
        )
# TODO: change template word to project name and delete this comment

target_link_libraries(
        worker
        Boost::log_setup
        Boost::log
        Boost::thread
        Boost::filesystem
        Boost::system
        nlohmann_json::nlohmann_json
)
target_link_libraries(
        tests
        template
        GTest::gtest_main
)
enable_testing()
add_test(NAME unit_tests COMMAND tests)

if (BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)
  # TODO: change template word to project name and delete this comment
  add_coverage(template)
  add_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif ()