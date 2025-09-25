enable_testing()

add_executable(tchecktest
    PasswordProcessor_test.cpp

# TODO RYS fix
    ../src/password_search.cpp
    ../src/password_search.hpp
)

target_link_libraries(tchecktest
    tcheck_options
    tcheck_warnings
# TODO RYS why target_link_system_libraries don't link here?
    GTest::gtest_main
)

find_package(gtest REQUIRED) # for intro, project_options, ...

include(GoogleTest)
gtest_discover_tests(tchecktest)
