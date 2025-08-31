cmake_minimum_required(VERSION 3.21)

project(FileBrowser)

include(my_tools/common.cmake)

add_subdirectory(my_tools)

set(USING_FREETYPE ON CACHE BOOL "Enable freetype" FORCE)
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES}
    ${CMAKE_SOURCE_DIR}/3rdLibrary/freetype/include
)

add_subdirectory(imgui_base)

include_directories(${INCLUDE_DIRECTORIES})

aux_source_directory(src SRC_LIST)

add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rdLibrary/freetype/lib/win)

target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

set(RUNTIME_DLLS
    ${CMAKE_SOURCE_DIR}/3rdLibrary/freetype/bin/freetype.dll
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(FILES ${RUNTIME_DLLS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(RUNTIME_DLLS
    ${CMAKE_SOURCE_DIR}/3rdLibrary/freetype/bin/freetype.dll
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(FILES ${RUNTIME_DLLS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
