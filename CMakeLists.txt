cmake_minimum_required(VERSION 3.5.1)

project(test)

# Find GTKmm using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtkmm-3.0)

# Specify the path to the directory containing the Couchbase C SDK object files
set(LIBCOUCHBASE_LIBRARY_PATH "/home/mrrobot/projects/couchbase/libcouchbase/lib")

# Find the transactions_cxx library (adjust the name if necessary)
find_library(TRANSACTIONS_CXX transactions_cxx)

# Include directories for Couchbase and GTKmm
include_directories(include /usr/include/couchbase ${GTK_INCLUDE_DIRS})

# Collect source files
file(GLOB SOURCES "src/**/*.cpp")

# Specify the target executable
add_executable(test ${SOURCES})

# Link against all object files in the specified directory
file(GLOB LIBCOUCHBASE_LIBRARIES "${LIBCOUCHBASE_LIBRARY_PATH}/*.so")

# Link against GTKmm libraries, transactions_cxx, and Couchbase libraries
target_link_libraries(test PRIVATE ${LIBCOUCHBASE_LIBRARIES} ${TRANSACTIONS_CXX} ${GTK_LIBRARIES})

# Add compile options for GTKmm
target_compile_options(test PRIVATE ${GTKMM_CFLAGS_OTHER})
