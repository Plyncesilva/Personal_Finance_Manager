project(test)
cmake_minimum_required(VERSION 3.5.1)

include_directories(include /usr/include/couchbase)
file(GLOB SOURCES "src/**/*.cpp")

# Specify the path to the directory containing the Couchbase C SDK object files
set(LIBCOUCHBASE_LIBRARY_PATH "/home/mrrobot/projects/couchbase/libcouchbase/lib")

find_library(TRANSACTIONS_CXX transactions_cxx)
message(STATUS "transactions:" ${TRANSACTIONS_CXX})

add_executable(test ${SOURCES})

# Link against all object files in the specified directory
file(GLOB LIBCOUCHBASE_LIBRARIES "${LIBCOUCHBASE_LIBRARY_PATH}/*.so")
target_link_libraries(test ${LIBCOUCHBASE_LIBRARIES} ${TRANSACTIONS_CXX})
