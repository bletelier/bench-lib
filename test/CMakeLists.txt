add_executable(test_bench test_main.cpp)
target_link_libraries(test_bench PRIVATE bench-lib)
add_test(NAME bench_basic_test COMMAND test_bench)
target_compile_options(test_bench PRIVATE -O3)
