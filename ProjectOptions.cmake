include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


include(CheckCXXSourceCompiles)


macro(tcheck_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)

    message(STATUS "Sanity checking UndefinedBehaviorSanitizer, it should be supported on this platform")
    set(TEST_PROGRAM "int main() { return 0; }")

    # Check if UndefinedBehaviorSanitizer works at link time
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=undefined")
    check_cxx_source_compiles("${TEST_PROGRAM}" HAS_UBSAN_LINK_SUPPORT)

    if(HAS_UBSAN_LINK_SUPPORT)
      message(STATUS "UndefinedBehaviorSanitizer is supported at both compile and link time.")
      set(SUPPORTS_UBSAN ON)
    else()
      message(WARNING "UndefinedBehaviorSanitizer is NOT supported at link time.")
      set(SUPPORTS_UBSAN OFF)
    endif()
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    if (NOT WIN32)
      message(STATUS "Sanity checking AddressSanitizer, it should be supported on this platform")
      set(TEST_PROGRAM "int main() { return 0; }")

      # Check if AddressSanitizer works at link time
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
      set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
      check_cxx_source_compiles("${TEST_PROGRAM}" HAS_ASAN_LINK_SUPPORT)

      if(HAS_ASAN_LINK_SUPPORT)
        message(STATUS "AddressSanitizer is supported at both compile and link time.")
        set(SUPPORTS_ASAN ON)
      else()
        message(WARNING "AddressSanitizer is NOT supported at link time.")
        set(SUPPORTS_ASAN OFF)
      endif()
    else()
      set(SUPPORTS_ASAN ON)
    endif()
  endif()
endmacro()

macro(tcheck_setup_options)
  option(tcheck_ENABLE_HARDENING "Enable hardening" ON)
  option(tcheck_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    tcheck_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    tcheck_ENABLE_HARDENING
    OFF)

  tcheck_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR tcheck_PACKAGING_MAINTAINER_MODE)
    option(tcheck_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(tcheck_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(tcheck_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(tcheck_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(tcheck_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(tcheck_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(tcheck_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(tcheck_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(tcheck_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(tcheck_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(tcheck_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(tcheck_ENABLE_PCH "Enable precompiled headers" OFF)
    option(tcheck_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(tcheck_ENABLE_IPO "Enable IPO/LTO" ON)
    option(tcheck_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(tcheck_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(tcheck_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(tcheck_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(tcheck_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(tcheck_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(tcheck_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(tcheck_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(tcheck_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(tcheck_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(tcheck_ENABLE_PCH "Enable precompiled headers" OFF)
    option(tcheck_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      tcheck_ENABLE_IPO
      tcheck_WARNINGS_AS_ERRORS
      tcheck_ENABLE_USER_LINKER
      tcheck_ENABLE_SANITIZER_ADDRESS
      tcheck_ENABLE_SANITIZER_LEAK
      tcheck_ENABLE_SANITIZER_UNDEFINED
      tcheck_ENABLE_SANITIZER_THREAD
      tcheck_ENABLE_SANITIZER_MEMORY
      tcheck_ENABLE_UNITY_BUILD
      tcheck_ENABLE_CLANG_TIDY
      tcheck_ENABLE_CPPCHECK
      tcheck_ENABLE_COVERAGE
      tcheck_ENABLE_PCH
      tcheck_ENABLE_CACHE)
  endif()

  tcheck_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (tcheck_ENABLE_SANITIZER_ADDRESS OR tcheck_ENABLE_SANITIZER_THREAD OR tcheck_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(tcheck_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(tcheck_global_options)
  if(tcheck_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    tcheck_enable_ipo()
  endif()

  tcheck_supports_sanitizers()

  if(tcheck_ENABLE_HARDENING AND tcheck_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR tcheck_ENABLE_SANITIZER_UNDEFINED
       OR tcheck_ENABLE_SANITIZER_ADDRESS
       OR tcheck_ENABLE_SANITIZER_THREAD
       OR tcheck_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${tcheck_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${tcheck_ENABLE_SANITIZER_UNDEFINED}")
    tcheck_enable_hardening(tcheck_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(tcheck_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(tcheck_warnings INTERFACE)
  add_library(tcheck_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  tcheck_set_project_warnings(
    tcheck_warnings
    ${tcheck_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(tcheck_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    tcheck_configure_linker(tcheck_options)
  endif()

  include(cmake/Sanitizers.cmake)
  tcheck_enable_sanitizers(
    tcheck_options
    ${tcheck_ENABLE_SANITIZER_ADDRESS}
    ${tcheck_ENABLE_SANITIZER_LEAK}
    ${tcheck_ENABLE_SANITIZER_UNDEFINED}
    ${tcheck_ENABLE_SANITIZER_THREAD}
    ${tcheck_ENABLE_SANITIZER_MEMORY})

  set_target_properties(tcheck_options PROPERTIES UNITY_BUILD ${tcheck_ENABLE_UNITY_BUILD})

  if(tcheck_ENABLE_PCH)
    target_precompile_headers(
      tcheck_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(tcheck_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    tcheck_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(tcheck_ENABLE_CLANG_TIDY)
    tcheck_enable_clang_tidy(tcheck_options ${tcheck_WARNINGS_AS_ERRORS})
  endif()

  if(tcheck_ENABLE_CPPCHECK)
    tcheck_enable_cppcheck(${tcheck_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(tcheck_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    tcheck_enable_coverage(tcheck_options)
  endif()

  if(tcheck_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(tcheck_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(tcheck_ENABLE_HARDENING AND NOT tcheck_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR tcheck_ENABLE_SANITIZER_UNDEFINED
       OR tcheck_ENABLE_SANITIZER_ADDRESS
       OR tcheck_ENABLE_SANITIZER_THREAD
       OR tcheck_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    tcheck_enable_hardening(tcheck_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
