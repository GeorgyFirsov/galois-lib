#
# Configuration flags
#
if (GALOISLIB_BUILD_PRETTY_DOCS)
    set(GALOISLIB_DOXYGEN_HAVE_DOT              YES)
else (GALOISLIB_BUILD_PRETTY_DOCS)
    set(GALOISLIB_DOXYGEN_HAVE_DOT              NO)
endif (GALOISLIB_BUILD_PRETTY_DOCS)

#
# Setup paths
#
set(GALOISLIB_DOCS_VERSION                      ${CMAKE_PROJECT_VERSION})
set(GALOISLIB_DOCS_ROOT                         ${GALOISLIB_ROOT}/docs)
set(GALOISLIB_DOCS_DOXYGEN_AWESOME_ROOT         ${GALOISLIB_DOCS_ROOT}/doxygen-awesome-css)
set(GALOISLIB_DOCS_DOXYGEN_IN                   ${GALOISLIB_DOCS_ROOT}/Doxyfile.in)
set(GALOISLIB_DOCS_DOXYGEN_OUT                  ${GALOISLIB_DOCS_ROOT}/Doxyfile)

#
# And here we go...
#
message("[${PROJECT_NAME}]: Configuring doxygen...")
configure_file(${GALOISLIB_DOCS_DOXYGEN_IN}     ${GALOISLIB_DOCS_DOXYGEN_OUT} @ONLY)

#
# In case of GitHub docs we only need a configuration file to be 
# generated, so we quit here in this case
#
if (GALOISLIB_BUILD_GITHUB_DOCS)
    message(NOTICE "[${PROJECT_NAME}]: Doxyfile is generated, but GALOISLIB_BUILD_GITHUB_DOCS is set, nothing else will be performed")
    message(NOTICE "[${PROJECT_NAME}]: Doxyfile is located here: '${GALOISLIB_DOCS_DOXYGEN_OUT}'")
    return()
endif (GALOISLIB_BUILD_GITHUB_DOCS)

#
# Otherwise we need to find Doxygen
#
find_package(Doxygen)

#
# Check if we have Doxygen installed
#
if (NOT DOXYGEN_FOUND )
    message(WARNING "[${PROJECT_NAME}]: Doxygen need to be installed to generate the doxygen documentation")
    return()
endif() 

#
# Custom target for building documentation
#
add_custom_target(galois-lib-docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${GALOISLIB_DOCS_DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM)
    