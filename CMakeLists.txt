# 最低CMake版本要求
cmake_minimum_required(VERSION 3.21)

# 项目名称
project(FileBrowser)

include(my_tools/common.cmake)

# set INCLUDE_DIRECTORIES in every subdirectorys
add_subdirectory(my_tools)

link_directories("D:/Files/3rdparty/lib")

# Uncomment the following lines if you want to use Freetype for GUI Font functions
# set(USING_FREETYPE ON CACHE BOOL "Enable freetype" FORCE)
# set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES}
# 	"D:/Files/3rdparty/include/freetype"
# )

add_subdirectory(imgui_base)

include_directories(${INCLUDE_DIRECTORIES})

aux_source_directory(src SRC_LIST)

add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})

if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
endif()

target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
