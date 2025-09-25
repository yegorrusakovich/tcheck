set(TARGET ${CMAKE_PROJECT_NAME})

add_executable(${TARGET}
    global_error_handler.cpp
    main.cpp
    decls.hpp
    global_error_handler.hpp
    pattern_matcher.hpp
    pattern_matcher.cpp
    matchers.hpp
    matchers.cpp
)

target_link_libraries(${TARGET} PRIVATE tcheck_options tcheck_warnings)

set_target_properties(
  ${TARGET}
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)
