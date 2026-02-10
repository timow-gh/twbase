function(create_test testName)
    add_executable(${testName} ${testName}.cpp)
    target_link_libraries(${testName} PRIVATE gtest_main ${PROJECT_NAME}::twbase)
    add_test(NAME twbase.${testName} COMMAND ${testName})
    
    # Register for coverage dependencies
    set_property(GLOBAL APPEND PROPERTY ${PROJECT_NAME}_COVERAGE_TESTS ${testName})
endfunction()

create_test(google_test_usage)
