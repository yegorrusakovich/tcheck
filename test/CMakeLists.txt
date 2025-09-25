enable_testing()

find_package(gtest REQUIRED) # for intro, project_options, ...
include(GoogleTest)

add_executable(password
    PasswordMatcher_test.cpp

# TODO RYS fix
    ../src/pattern_matcher.cpp
    ../src/pattern_matcher.hpp
    ../src/matchers.cpp
    ../src/matchers.hpp
)

target_link_libraries(password
    tcheck_options
    tcheck_warnings
# TODO RYS why target_link_system_libraries don't link here?
    GTest::gtest_main
)

gtest_discover_tests(password)

add_executable(uri
    UriMatcher_test.cpp

# TODO RYS fix
    ../src/pattern_matcher.cpp
    ../src/pattern_matcher.hpp
    ../src/matchers.cpp
    ../src/matchers.hpp
)

target_link_libraries(uri
    tcheck_options
    tcheck_warnings
# TODO RYS why target_link_system_libraries don't link here?
    GTest::gtest_main
)

gtest_discover_tests(uri)
