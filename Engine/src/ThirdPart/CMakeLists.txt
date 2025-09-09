add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
target_link_libraries(XEngine PRIVATE SDL3::SDL3)


file(GLOB_RECURSE gladSource "glad/*c")
file(GLOB_RECURSE gladHeader "glad/*h")
add_library(gladLib STATIC ${gladSource} ${gladHeader})
target_include_directories(gladLib PUBLIC glad/include)
target_include_directories(gladLib PUBLIC glad/src)
target_link_libraries(XEngine PUBLIC gladLib)


## install vulkan in your computer
find_package(Vulkan REQUIRED)
target_link_libraries(XEngine PRIVATE Vulkan::Vulkan)

##VMA
add_subdirectory(VulkanMemoryAllocator)
target_link_libraries(XEngine PRIVATE GPUOpen::VulkanMemoryAllocator)

##VK-BOOSTRAP
add_subdirectory(vk-bootstrap)
target_link_libraries(XEngine PRIVATE vk-bootstrap::vk-bootstrap)
