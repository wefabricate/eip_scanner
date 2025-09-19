set(SOURCE_FILES 
        ${CMAKE_CURRENT_LIST_DIR}/CipRevision.cpp
        ${CMAKE_CURRENT_LIST_DIR}/EPath.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MessageRouterRequest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MessageRouterResponse.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CipString.cpp)


target_sources(eip_scanner PUBLIC ${SOURCE_FILES})

add_subdirectory(connectionManager)