project(test)
include_directories(include)
file(GLOB SOURCES "src/**/*.cpp")
add_executable(test ${SOURCES})