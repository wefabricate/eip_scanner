set(SOURCE_FILES 
        ${CMAKE_CURRENT_LIST_DIR}/CommonPacket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CommonPacketItem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CommonPacketItemFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/EncapsPacket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/EncapsPacketFactory.cpp)

target_sources(eip_scanner PUBLIC ${SOURCE_FILES})
target_sources(eip_scannerS PUBLIC ${SOURCE_FILES})