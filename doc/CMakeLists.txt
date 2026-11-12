# ============================================================================
# External requirements
# ============================================================================

find_package (Doxygen OPTIONAL_COMPONENTS dot)
if(NOT DOXYGEN_FOUND)
	return()
else()
	message("Doxygen found : ${DOXYGEN_EXECUTABLE}")
endif()

find_package (Sphinx)
if(NOT SPHINX_FOUND)
	return()
else()
	message("Sphinx found : ${Sphinx_EXECUTABLE}")
endif()


# ============================================================================
# Targets
# ============================================================================
#Doxyfile
configure_file (Doxyfile.in Doxyfile @ONLY)
#Configure sphinx configuration file
configure_file (conf.py.in conf.py @ONLY)

#call doxygen to produce xml files
add_custom_target (doc_dox
	COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	COMMENT "Generating documentation with Doxygen" VERBATIM)

#call sphinx to generate html documentation
add_custom_target (doc
    COMMAND ${Sphinx_EXECUTABLE} -E -c ${CMAKE_CURRENT_BINARY_DIR} -b html ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/html
    DEPENDS doc_dox
	COMMENT "Generating documentation with Sphinx" VERBATIM)

# ============================================================================
# Install configuration
# ============================================================================
install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/
	DESTINATION share/doc
    OPTIONAL )

