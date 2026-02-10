# Skip install test when coverage is enabled (coverage-instrumented libraries
# can't be linked by non-coverage builds, causing undefined gcov symbols)
if(NOT CMAKE_CXX_FLAGS MATCHES "--coverage")
    add_subdirectory(test_install)
endif()

function(create_test testName)
    add_executable(${testName} ${testName}.cpp)
    target_link_libraries(${testName} PRIVATE gtest_main ${PROJECT_NAME}::twbase)
    add_test(NAME twbase.${testName} COMMAND ${testName})
    
    # Register for coverage dependencies
    set_property(GLOBAL APPEND PROPERTY ${PROJECT_NAME}_COVERAGE_TESTS ${testName})
endfunction()

create_test(assertion_tests)
create_test(lib_test)

