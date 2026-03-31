cmake_minimum_required(VERSION 3.20)
project(calculator CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# libcore is pre-installed into the rootfs
find_library(CORE_LIB core PATHS /usr/local/lib REQUIRED)

add_executable(calculator src/main.cpp)
target_include_directories(calculator PRIVATE /usr/local/include)
target_link_libraries(calculator PRIVATE ${CORE_LIB})
target_compile_options(calculator PRIVATE -Wall -Werror -Wextra)
