include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(tcheck_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

endfunction()
