set(SOURCE_FILES 
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectEmptyState.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectLoadedState.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectNonExistentState.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectState.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FileObjectUploadInProgressState.cpp)
        
target_sources(eip_scanner PUBLIC ${SOURCE_FILES})