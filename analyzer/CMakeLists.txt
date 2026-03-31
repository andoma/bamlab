cmake_minimum_required(VERSION 3.20)
project(analyzer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# libcore is pre-installed into the rootfs
find_library(CORE_LIB core PATHS /usr/local/lib REQUIRED)

add_executable(analyzer src/main.cpp)
target_include_directories(analyzer PRIVATE /usr/local/include)
target_link_libraries(analyzer PRIVATE ${CORE_LIB})
target_compile_options(analyzer PRIVATE -Wall -Werror -Wextra)
