enable_testing()

include(GoogleTest)

add_executable(password
    PasswordMatcher_test.cpp
)

target_link_libraries(password
    tcheck_options
    tcheck_warnings
# TODO RYS why target_link_system_libraries don't link here?
    tcheck::mtch
    GTest::gtest_main
)

gtest_discover_tests(password)

add_executable(uri
    UriMatcher_test.cpp
)

target_link_libraries(uri
    tcheck_options
    tcheck_warnings
# TODO RYS why target_link_system_libraries don't link here?
    tcheck::mtch
    GTest::gtest_main
)

gtest_discover_tests(uri)

add_executable(date
  DateMatcher_test.cpp
)

target_link_libraries(date
    tcheck_options
    tcheck_warnings
# TODO RYS why target_link_system_libraries don't link here?
    tcheck::mtch
    GTest::gtest_main
)

gtest_discover_tests(date)
