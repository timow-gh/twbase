cmake_minimum_required(VERSION 3.28)

project(use_installdir_proj
        VERSION 0.0.0
        LANGUAGES CXX)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(${PROJECT_SOURCE_DIR}/cmake/project_preamble.cmake)
project_preamble()

# Find the installed package
find_package(twbase CONFIG REQUIRED)

# Create executable
add_executable(use_installdir main.cpp)
target_link_libraries(use_installdir PRIVATE twbase::twbase)

set_target_properties(use_installdir
        PROPERTIES
        DEBUG_POSTFIX d
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

# Install executable
install(TARGETS use_installdir
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Install the twbase shared library that the executable depends on
if(MSVC)
    # On Windows, DLLs are in the bin directory
    file(GLOB twbase_DLLS "${CMAKE_PREFIX_PATH}/bin/twbase*.dll")
    if(twbase_DLLS)
        install(FILES ${twbase_DLLS}
                DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
else()
    # On Unix-like systems, shared libraries are in lib directory
    file(GLOB twbase_LIBS "${CMAKE_PREFIX_PATH}/lib/libtwbase*.so*")
    if(twbase_LIBS)
        install(FILES ${twbase_LIBS}
                DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endif()
endif()