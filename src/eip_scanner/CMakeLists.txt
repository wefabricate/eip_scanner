include_directories(${CMAKE_CURRENT_LIST_DIR})

# Add LTTng-UST check
option(ENABLE_LTTNG "Enable LTTng-UST tracing support if available" ON)
find_library(LTTNG_LIBRARY NAMES lttng-ust)
if(LTTNG_LIBRARY AND ENABLE_LTTNG)
    set(LTTNG_FOUND TRUE)
    message(STATUS "LTTng-UST found, enabling tracing")
else()
    message(STATUS "LTTng-UST not found, disabling tracing")
endif()


set(SOURCE_FILES
        cip/connectionManager/ForwardCloseRequest.cpp
        cip/connectionManager/ForwardOpenRequest.cpp
        cip/connectionManager/LargeForwardOpenRequest.cpp
        cip/connectionManager/ForwardOpenResponse.cpp
        cip/connectionManager/NetworkConnectionParametersBuilder.cpp

        cip/CipRevision.cpp
        cip/EPath.cpp
        cip/MessageRouterRequest.cpp
        cip/MessageRouterResponse.cpp
        cip/CipString.cpp

        eip/CommonPacket.cpp
        eip/CommonPacketItem.cpp
        eip/CommonPacketItemFactory.cpp
        eip/EncapsPacket.cpp
        eip/EncapsPacketFactory.cpp

        fileObject/FileObjectEmptyState.cpp
        fileObject/FileObjectLoadedState.cpp
        fileObject/FileObjectNonExistentState.cpp
        fileObject/FileObjectState.cpp
        fileObject/FileObjectUploadInProgressState.cpp

        sockets/BaseSocket.cpp
        sockets/EndPoint.cpp
        sockets/TCPSocket.cpp
        sockets/UDPBoundSocket.cpp
        sockets/UDPSocket.cpp
        sockets/Platform.cpp

        utils/Logger.cpp
        utils/Buffer.cpp

        BaseObject.cpp
        ConnectionManager.cpp
        DiscoveryManager.cpp
        FileObject.cpp
        IdentityObject.cpp
        IOConnection.cpp
        MessageRouter.cpp
        ParameterObject.cpp
        SessionInfo.cpp)

# if vendor scripts are enabled
if(ENABLE_VENDOR_SRC)
    add_subdirectory(vendor)
endif()

if(LTTNG_FOUND)
    list(APPEND SOURCE_FILES tracing/eip_tracepoint.c)
endif()

add_library(eip_scanner ${SOURCE_FILES} ${VENDOR_FILES})

if (WIN32)
    list(APPEND LIBRARY_DEPENDENCIES ws2_32)
    if (BUILD_SHARED_LIBS)
        set_target_properties(eip_scanner PROPERTIES OUTPUT_NAME "eip_scanner")
    endif()
endif()

if(LTTNG_FOUND)
    list(APPEND LIBRARY_DEPENDENCIES lttng-ust)
    target_compile_definitions(eip_scanner PRIVATE -DLTTNG_ENABLED)
endif()

if (${LIBRARY_DEPENDENCIES})
    target_link_libraries(eip_scanner ${LIBRARY_DEPENDENCIES})
endif()

set_target_properties(
        eip_scanner
        PROPERTIES
        VERSION ${EIPSCANNER_FULL_VERSION}
        SOVERSION ${EIPSCANNER_MAJOR_VERSION})

install(TARGETS eip_scanner
        LIBRARY
            DESTINATION "${INSTALL_LIB_DIR}"
        ARCHIVE
            DESTINATION "${INSTALL_LIB_DIR}")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/src/"
        DESTINATION "${INSTALL_INCLUDE_DIR}"
        FILES_MATCHING PATTERN "*.h*")