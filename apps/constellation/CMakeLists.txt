#
# F E T C H   C O N S T E L L A T I O N
#
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(fetch-constellation)

setup_compiler()

add_executable(constellation
               bootstrap_monitor.cpp
               bootstrap_monitor.hpp
               constellation.cpp
               constellation.hpp
               health_check_http_module.hpp
               logging_http_module.hpp
               main.cpp
               telemetry_http_module.hpp)
target_link_libraries(constellation PRIVATE fetch-ledger fetch-miner fetch-dkg)
target_include_directories(constellation PRIVATE ${FETCH_ROOT_DIR}/libs/python/include)
