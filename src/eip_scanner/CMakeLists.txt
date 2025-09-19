include_directories(${CMAKE_CURRENT_LIST_DIR})

set(LIB_SOURCE_FILES
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
    list(APPEND LIB_SOURCE_FILES tracing/eip_tracepoint.c)
endif()

add_library(eip_scanner ${LIB_SOURCE_FILES} ${VENDOR_FILES})

if(LTTNG_FOUND)
    target_link_libraries(eip_scanner PUBLIC lttng-ust dl)
    target_compile_definitions(eip_scanner PUBLIC -DLTTNG_ENABLED)
endif()

# Add Windows-specific DLL configurations
if(WIN32)
    target_link_libraries(eip_scanner PRIVATE ws2_32)
    
    # Set proper Windows DLL properties
    if(BUILD_SHARED_LIBS)
        set_target_properties(eip_scanner PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
            VERSION ${EIPSCANNER_FULL_VERSION}
            SOVERSION ${EIPSCANNER_MAJOR_VERSION}
            DEBUG_POSTFIX "d"
        )
    endif()
endif()


set_target_properties(
        eip_scanner
        PROPERTIES
        VERSION ${EIPSCANNER_FULL_VERSION}
        SOVERSION ${EIPSCANNER_MAJOR_VERSION})

# Install the target with proper export
install(TARGETS eip_scanner
    EXPORT eip_scannerTargets
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/src/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h*")