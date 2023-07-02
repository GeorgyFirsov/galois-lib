#
# Directories
#
set(GALOISLIB_SOURCES_ROOT				                ${GALOISLIB_LIB_ROOT}/src)
set(GALOISLIB_INCLUDE_ROOT				                ${GALOISLIB_LIB_ROOT}/include)

set(GALOISLIB_INCLUDE_DIRECTORIES		                ${GALOISLIB_INCLUDE_ROOT})

#
# Implementation directories
#
set(GALOISLIB_SOURCES_DIR                               ${GALOISLIB_SOURCES_ROOT}/details)
set(GALOISLIB_INCLUDE_DIR                               ${GALOISLIB_INCLUDE_ROOT}/details)

#
# Source files
#
set(GALOISLIB_SOURCE_FILES			                    ${GALOISLIB_SOURCES_DIR}/gf128.c
									                    ${GALOISLIB_SOURCES_DIR}/gf8.c)

set(GALOISLIB_HEADER_FILES			                    ${GALOISLIB_INCLUDE_DIR}/gf128.h
									                    ${GALOISLIB_INCLUDE_DIR}/gf8.h)

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
