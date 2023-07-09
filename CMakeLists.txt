cmake_minimum_required(VERSION 3.0.0)
project(galois-lib VERSION 1.0.2)

#
# Global config
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED                             True)

#
# Options
#
option(GALOISLIB_KERNEL_MODE                                "Build galois-lib for kernel mode too (Windows only)."                  OFF)
option(GALOISLIB_GENERATE_DOCS                              "Generate documentation for galois-lib."                                OFF)
option(GALOISLIB_DOCS_ONLY                                  "Generate documentation only (galois-lib binaries will not be built)."  OFF)
option(GALOISLIB_GITHUB_DOCS                                "Generate documentation for GitHub."                                    OFF)
option(GALOISLIB_PRETTY_DOCS                                "Use graphwiz for diagrams."                                            OFF)

#
# Configuration
#
if (WIN32)
    set(GALOISLIB_WINDOWS_BUILD                             ON)
else (WIN32)
    set(GALOISLIB_WINDOWS_BUILD                             OFF)
endif (WIN32)

if (GALOISLIB_GENERATE_DOCS AND GALOISLIB_DOCS_ONLY)

    #
    # Building docs only
    #

    set(GALOISLIB_BUILD_LIB                                 OFF)
    set(GALOISLIB_BUILD_DOCS                                ON)
    set(GALOISLIB_BUILD_GITHUB_DOCS                         ${GALOISLIB_GITHUB_DOCS})
    set(GALOISLIB_BUILD_PRETTY_DOCS                         ${GALOISLIB_PRETTY_DOCS})

    #
    # Just set this to OFF, because no binary is built
    #
    set(GALOISLIB_BUILD_KERNEL_LIB                          OFF)

else (GALOISLIB_GENERATE_DOCS AND GALOISLIB_DOCS_ONLY)

    #
    # Here I assume other flags, but library 
    # will be built always here.
    #

    set(GALOISLIB_BUILD_LIB                                 ON)
    set(GALOISLIB_BUILD_DOCS                                ${GALOISLIB_GENERATE_DOCS})
    set(GALOISLIB_BUILD_GITHUB_DOCS                         ${GALOISLIB_GITHUB_DOCS})
    set(GALOISLIB_BUILD_PRETTY_DOCS                         ${GALOISLIB_PRETTY_DOCS})

    #
    # Kernel mode library is supporten on Windows only
    #
    if (GALOISLIB_WINDOWS_BUILD)

        set(GALOISLIB_BUILD_KERNEL_LIB                      ${GALOISLIB_KERNEL_MODE})
        
    else (GALOISLIB_WINDOWS_BUILD)

        message(STATUS "[${PROJECT_NAME}]: Non-Windows version, GALOISLIB_BUILD_KERNEL_LIB will be set to OFF.")
        set(GALOISLIB_BUILD_KERNEL_LIB                      OFF)

    endif (GALOISLIB_WINDOWS_BUILD)

endif (GALOISLIB_GENERATE_DOCS AND GALOISLIB_DOCS_ONLY)

message("[${PROJECT_NAME}]: GALOISLIB_WINDOWS_BUILD     = ${GALOISLIB_WINDOWS_BUILD}")
message("[${PROJECT_NAME}]: GALOISLIB_BUILD_LIB         = ${GALOISLIB_BUILD_LIB}")
message("[${PROJECT_NAME}]: GALOISLIB_BUILD_KERNEL_LIB  = ${GALOISLIB_BUILD_KERNEL_LIB}")
message("[${PROJECT_NAME}]: GALOISLIB_BUILD_DOCS        = ${GALOISLIB_BUILD_DOCS}")
message("[${PROJECT_NAME}]: GALOISLIB_BUILD_GITHUB_DOCS = ${GALOISLIB_BUILD_GITHUB_DOCS}")
message("[${PROJECT_NAME}]: GALOISLIB_BUILD_PRETTY_DOCS = ${GALOISLIB_BUILD_PRETTY_DOCS}")

#
# Just sanity check if we are going well
#
if (NOT GALOISLIB_BUILD_LIB AND GALOISLIB_BUILD_TESTS)
    message(FATAL_ERROR "[${PROJECT_NAME}]: cannot build tests without building galois-lib itself")
endif (NOT GALOISLIB_BUILD_LIB AND GALOISLIB_BUILD_TESTS)

#
# Set global directories
#
set(GALOISLIB_ROOT		                                    ${CMAKE_CURRENT_LIST_DIR})
set(GALOISLIB_SOURCES_ROOT				                    ${GALOISLIB_ROOT}/src)
set(GALOISLIB_INCLUDE_ROOT                                  ${GALOISLIB_ROOT}/include)
set(GALOISLIB_INCLUDE_DIRECTORIES		                    ${GALOISLIB_INCLUDE_ROOT})

#
# Set output directories for all targets
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY                          ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY                          ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY                          ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE                  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE                  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE                  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG                    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG                    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG                    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

#
# Now let's build targets according to the flags set before
#
if (GALOISLIB_BUILD_LIB)
    
    #
    # FindWDK module
    #
    list(APPEND CMAKE_MODULE_PATH	                        ${CMAKE_SOURCE_DIR}/third-party/FindWDK/cmake)
    
    #
    # Preprocessor definitions
    #
    add_compile_definitions(UNICODE)

    #
    # Implementation directories
    #
    set(GALOISLIB_INTERNAL_SOURCES_DIR                      ${GALOISLIB_SOURCES_ROOT}/details)
    set(GALOISLIB_INTERNAL_INCLUDE_DIR                      ${GALOISLIB_INCLUDE_ROOT}/details)

    #
    # Source files
    #
    set(GALOISLIB_SOURCE_FILES			                    ${GALOISLIB_INTERNAL_SOURCES_DIR}/gf128.c
                                                            ${GALOISLIB_INTERNAL_SOURCES_DIR}/gf8.c)

    set(GALOISLIB_HEADER_FILES			                    ${GALOISLIB_INTERNAL_INCLUDE_DIR}/gf128.h
                                                            ${GALOISLIB_INTERNAL_INCLUDE_DIR}/gf8.h)

    set(GALOISLIB_SOURCES				                    ${GALOISLIB_SOURCE_FILES}
                                                            ${GALOISLIB_HEADER_FILES})

    #
    # Library itself (may be built for user mode as 
    # well as for kernel mode)
    #
    add_library(galois-lib					                ${GALOISLIB_SOURCES})

    if (GALOISLIB_BUILD_KERNEL_LIB)
        message("[${PROJECT_NAME}]: Building additional target for kernel mode")

        find_package(WDK REQUIRED)
        wdk_add_library(galois-lib-km		                ${GALOISLIB_SOURCES})
    endif (GALOISLIB_BUILD_KERNEL_LIB)


    #
    # Include directories
    #
    target_include_directories(galois-lib PRIVATE           ${GALOISLIB_INCLUDE_DIRECTORIES})

    if (GALOISLIB_BUILD_KERNEL_LIB)
        target_include_directories(galois-lib-km PRIVATE    ${GALOISLIB_INCLUDE_DIRECTORIES})
    endif (GALOISLIB_BUILD_KERNEL_LIB)


    #
    # On non-Windows builds it is necessary to add pclmul support
    #
    if (NOT GALOISLIB_WINDOWS_BUILD)
        target_compile_options(galois-lib PRIVATE           -mpclmul)
    endif (NOT GALOISLIB_WINDOWS_BUILD)

endif (GALOISLIB_BUILD_LIB)

if (GALOISLIB_BUILD_DOCS)

    #
    # Docs target
    #
    add_subdirectory(docs)

endif (GALOISLIB_BUILD_DOCS)
