include(GenerateExportHeader)

set(TARGET mtch)

add_library(${TARGET}
    pattern_matcher.cpp
    matchers.cpp
)

add_library(tcheck::mtch ALIAS mtch)

target_include_directories(mtch ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_link_libraries(${TARGET} PRIVATE tcheck_options tcheck_warnings)

set_target_properties(
  ${TARGET}
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(${TARGET} EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/myproject/sample_library_export.hpp)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${TARGET} PUBLIC ${TARGET}_STATIC_DEFINE)
endif()

