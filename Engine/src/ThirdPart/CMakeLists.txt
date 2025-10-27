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

##bgfx
find_package(bgfx CONFIG REQUIRED)
target_link_libraries(XEngine PRIVATE bgfx::bgfx)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(XEngine PRIVATE imgui)

find_package(tomlplusplus CONFIG REQUIRED)
target_link_libraries(XEngine PRIVATE tomlplusplus::tomlplusplus)

find_package(RTTR CONFIG REQUIRED Core)
target_link_libraries(XEngine RTTR::Core)

find_package(Taskflow CONFIG REQUIRED)
target_link_libraries(XEngine Taskflow::Taskflow)