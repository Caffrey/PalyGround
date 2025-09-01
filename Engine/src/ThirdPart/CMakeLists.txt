add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
target_link_libraries(XEngine PRIVATE SDL3::SDL3)


file(GLOB_RECURSE gladSource "glad/*c")
file(GLOB_RECURSE gladHeader "glad/*h")
add_library(gladLib STATIC ${gladSource} ${gladHeader})
target_include_directories(gladLib PUBLIC glad/include)
target_include_directories(gladLib PUBLIC glad/src)
target_link_libraries(XEngine PUBLIC gladLib)