cmake_minimum_required(VERSION 3.26)

project(Engine)

set(CMAKE_CXX_STANDARD 20)

##Engine Lib

file(GLOB_RECURSE EngineSourceFiles "src/Engine/*.cpp")
file(GLOB_RECURSE EngineHeaderFiles "src/Engine/*.h")

file(GLOB_RECURSE EditorSourceFiles "src/Editor/*.cpp")
file(GLOB_RECURSE EditorHeaderFiles "src/Editor/*.h")

add_executable(XEngine ${EngineSourceFiles} ${EngineHeaderFiles} ${EditorSourceFiles} ${EditorHeaderFiles} src/main.cpp)
target_include_directories(XEngine PUBLIC src/Editor)
target_include_directories(XEngine PUBLIC src/Engine)
target_include_directories(XEngine PUBLIC src)
#-----------------------------
# #控制好库文件和Exe在同一个目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)    # 可执行文件
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)    # 动态库
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)    # 静态库

# add_custom_command(TARGET XEngine POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_BINARY_DIR}/bin/
#         ${CMAKE_CURRENT_BINARY_DIR}
#     COMMENT "Copying all files from bin to Debug directory"
# )

#添加第三库
add_subdirectory(src/ThirdPart)
