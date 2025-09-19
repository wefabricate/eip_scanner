cmake_minimum_required(VERSION 3.16)

enable_testing()
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/test")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

set(TEST_SOURCE_FILES 
        cip/TestCipRevision.cpp
        cip/TestCipString.cpp
        cip/TestEPath.cpp
        cip/TestMessageRouterResponse.cpp

        eip/TestCommonPacket.cpp
        eip/TestCommonPacketItem.cpp
        eip/TestCommonPacketItemFactory.cpp
        eip/TestEncapsPacket.cpp
        eip/TestEncapsPacketFactory.cpp

        fileObject/TestFileObjectLoadedState.cpp
        fileObject/TestFileObjectUploadInProgressState.cpp

        sockets/TestEndPoint.cpp

        utils/TestBuffer.cpp

        TestDiscoveryManager.cpp
        TestIdentityObject.cpp
        TestMessageRouter.cpp
        TestParameterObject.cpp
        test.cpp
        )
        
if(ENABLE_VENDOR_SRC)
        list(APPEND TEST_SOURCE_FILES
                vendor/ra/powerFlex525/TestDPIFaultManager.cpp
                vendor/ra/powerFlex525/TestDPIFaultObject.cpp
                vendor/ra/powerFlex525/TestDPIFaultParameter.cpp
        )
endif()

add_executable(test_eipscanner
        ${TEST_SOURCE_FILES}
        )

message(STATUS ${GTEST_BOTH_LIBRARIES})
message(STATUS ${GMOCK_BOTH_LIBRARIES})

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

target_link_libraries(test_eipscanner
        ${GTEST_BOTH_LIBRARIES}
        ${GMOCK_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        eip_scanner)

if(LTTNG_FOUND)
    target_link_libraries(test_eipscanner lttng-ust)
endif()

add_test(TestEipScanner test_eipscanner)
