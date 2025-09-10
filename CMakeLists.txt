cmake_minimum_required(VERSION 3.5)
 
set(EIPSCANNER_MAJOR_VERSION 1)
set(EIPSCANNER_MINOR_VERSION 1)
set(EIPSCANNER_PATCH_VERSION 0)
set(EIPSCANNER_FULL_VERSION ${EIPSCANNER_MAJOR_VERSION}.${EIPSCANNER_MINOR_VERSION}.${EIPSCANNER_PATCH_VERSION})
 
project(eip_scanner VERSION ${EIPSCANNER_FULL_VERSION})
 
set(CMAKE_CXX_STANDARD 20)
option(ENABLE_VENDOR_SRC "Enable vendor source" OFF)
option(TEST_ENABLED "Enable unit test" OFF)
option(EXAMPLE_ENABLED "Build examples" OFF)
 
if (WIN32)
  # Needed on MinGW with GCC 10 or lower
  add_compile_definitions(_WIN32_WINNT=0x0600)
 
  # Prevent std::numeric_limits::max collision
  if (MSVC)
    add_compile_options(/W1 /IGNORE:warning[4267,4244])
    add_compile_definitions(NOMINMAX)
  endif()
endif()
 
set(INSTALL_CMAKE_DIR share/eip_scanner/cmake CACHE PATH
    "Installation directory for CMake files")
# ... for the build tree
set(INSTALL_INCLUDE_DIR include CACHE PATH
      "Installation directory for header files")
# ... for the build tree
set(INSTALL_LIB_DIR lib CACHE PATH
      "Installation directory for header files")
# Make relative paths absolute (needed later on)
# foreach(p LIB BIN INCLUDE CMAKE)
foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()
 
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
       "${INSTALL_INCLUDE_DIR}")
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(eip_scannerConfig.cmake.in
  "${PROJECT_BINARY_DIR}/eip_scannerConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${eip_scanner_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(eip_scannerConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/eip_scannerConfig.cmake" @ONLY)
# ... for both
configure_file(eip_scannerConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/eip_scannerConfigVersion.cmake" @ONLY)
# Install the eip_scannerConfig.cmake and eip_scannerConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/eip_scannerConfig.cmake"
  "${PROJECT_BINARY_DIR}/eip_scannerConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Setup the include structure so that every file is based off of the root of the project
include_directories(src)
 
# Actually add the source files for the project
add_subdirectory(src/${CMAKE_PROJECT_NAME})
 
if (EXAMPLE_ENABLED)
  set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
 
  # Create the build directory if it doesn't exist
  file(MAKE_DIRECTORY ${EXAMPLES_DIR}/build)
 
  # Run the other CMakeLists.txt file without any dependencies on this file
  execute_process(COMMAND ${CMAKE_COMMAND} ${EXAMPLES_DIR} WORKING_DIRECTORY ${EXAMPLES_DIR}/build)
 
  # Execute code during a `cmake --build` command
  add_custom_target(EXAMPLES_TARGETS ALL COMMAND ${CMAKE_COMMAND} --build ${EXAMPLES_DIR}/build WORKING_DIRECTORY ${EXAMPLES_DIR}/build COMMENT "Building the examples sub-project")
endif()
 
if (TEST_ENABLED)
    add_subdirectory(test)
endif()
 
include(GNUInstallDirs)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/package.xml
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})
 