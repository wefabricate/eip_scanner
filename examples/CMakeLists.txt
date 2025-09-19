cmake_minimum_required(VERSION 3.5)

# Remember to update the LIBRARY_PATH and LD_LIBRARY_PATH environment variables
#   * export LIBRARY_PATH=/usr/local/include/
#   * export LD_LIBRARY_PATH=/usr/local/lib/

find_package(eip_scanner REQUIRED)

add_executable(explicit_messaging ExplicitMessagingExample.cpp)
target_link_libraries(explicit_messaging eip_scanner::eip_scanner)

add_executable(file_object_example FileObjectExample.cpp)
target_link_libraries(file_object_example eip_scanner::eip_scanner)

add_executable(identity_object_example IdentityObjectExample.cpp)
target_link_libraries(identity_object_example eip_scanner::eip_scanner)

add_executable(implicit_messaging ImplicitMessagingExample.cpp)
target_link_libraries(implicit_messaging eip_scanner::eip_scanner)

add_executable(parameter_object_example ParameterObjectExample.cpp)
target_link_libraries(parameter_object_example eip_scanner::eip_scanner)

add_executable(discovery_example DiscoveryManagerExample.cpp)
target_link_libraries(discovery_example eip_scanner::eip_scanner)

if (ENABLE_VENDOR_SRC)
  add_executable(yaskawa_assembly_object_example vendors/yaskawa/mp3300iec/Yaskawa_AssemblyObjectExample.cpp)
  target_link_libraries(yaskawa_assembly_object_example eip_scanner::eip_scanner)
endif()

if(WIN32)
  target_link_libraries(explicit_messaging ws2_32)
  target_link_libraries(file_object_example ws2_32)
  target_link_libraries(identity_object_example ws2_32)
  target_link_libraries(implicit_messaging ws2_32)
  target_link_libraries(parameter_object_example ws2_32)
  target_link_libraries(discovery_example ws2_32)
  if (ENABLE_VENDOR_SRC)
    target_link_libraries(yaskawa_assembly_object_example ws2_32)
  endif()
endif()

if(LTTNG_FOUND)
  target_link_libraries(explicit_messaging lttng-ust dl)
  target_link_libraries(file_object_example lttng-ust dl)
  target_link_libraries(identity_object_example lttng-ust dl)
  target_link_libraries(implicit_messaging lttng-ust dl)
  target_link_libraries(parameter_object_example lttng-ust dl)
  target_link_libraries(discovery_example lttng-ust dl)
  if (ENABLE_VENDOR_SRC)
    target_link_libraries(yaskawa_assembly_object_example lttng-ust dl)
  endif()
endif()
