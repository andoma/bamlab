cmake_minimum_required(VERSION 3.20)
project(core CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(core STATIC
    src/stats.cpp
    src/expr.cpp
)

target_include_directories(core PUBLIC include)
target_compile_options(core PRIVATE -Wall -Werror -Wextra)

install(TARGETS core ARCHIVE DESTINATION lib)
install(DIRECTORY include/core DESTINATION include)

# Tests
enable_testing()

add_executable(test_stats test/test_stats.cpp)
target_link_libraries(test_stats PRIVATE core)
target_compile_options(test_stats PRIVATE -Wall -Werror -Wextra)
add_test(NAME stats COMMAND test_stats)

add_executable(test_expr test/test_expr.cpp)
target_link_libraries(test_expr PRIVATE core)
target_compile_options(test_expr PRIVATE -Wall -Werror -Wextra)
add_test(NAME expr COMMAND test_expr)
