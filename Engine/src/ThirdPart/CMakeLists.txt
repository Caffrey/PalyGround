find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(XEngine PRIVATE SDL3::SDL3)

## install vulkan in your computer
find_package(Vulkan REQUIRED)
target_link_libraries(XEngine PRIVATE Vulkan::Vulkan)

##VMA
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
# add_subdirectory(VulkanMemoryAllocator)
target_link_libraries(XEngine PRIVATE GPUOpen::VulkanMemoryAllocator)

##VK-BOOSTRAP
find_package(vk-bootstrap CONFIG REQUIRED)
target_link_libraries(XEngine PRIVATE vk-bootstrap::vk-bootstrap)
