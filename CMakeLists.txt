cmake_minimum_required(VERSION 3.16)
 
set(EIPSCANNER_MAJOR_VERSION 2)
set(EIPSCANNER_MINOR_VERSION 0)
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
 
# Add LTTng-UST check
option(ENABLE_LTTNG "Enable LTTng-UST tracing support if available" ON)
find_library(LTTNG_LIBRARY NAMES lttng-ust)
if(LTTNG_LIBRARY AND ENABLE_LTTNG)
    set(LTTNG_FOUND TRUE)
    message(STATUS "LTTng-UST found, enabling tracing")
else()
    message(STATUS "LTTng-UST not found, disabling tracing")
endif()

include(GNUInstallDirs)

# Configure and install CMake package files
include(CMakePackageConfigHelpers)

# Generate the config file that includes the exports
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/eip_scannerConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/eip_scannerConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/eip_scanner"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

# Generate the version file for the config file
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/eip_scannerConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMinorVersion
)

# Install the configuration files
install(FILES
    "${PROJECT_BINARY_DIR}/eip_scannerConfig.cmake"
    "${PROJECT_BINARY_DIR}/eip_scannerConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/eip_scanner"
)

# Install the export set for use with the install-tree
install(EXPORT eip_scannerTargets
    FILE eip_scannerTargets.cmake
    NAMESPACE eip_scanner::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/eip_scanner"
)

# Setup the include structure so that every file is based off of the root of the project
include_directories(src)
 
# Actually add the source files for the project
add_subdirectory(src/${CMAKE_PROJECT_NAME})
 
if (EXAMPLE_ENABLED)
  add_subdirectory(examples)
endif()
 
if (TEST_ENABLED)
    add_subdirectory(test)
endif() 