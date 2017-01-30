cmake_minimum_required (VERSION 2.6)
project (rs)

add_executable(rs 
    main.c
)


include(CTest)
enable_testing()

add_executable(gf_test
    gf_test.c
)

add_executable(util_test
    util_test.c
)

add_test(gf_test gf_test)
add_test(util_test util_test)
