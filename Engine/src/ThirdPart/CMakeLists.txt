find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(XEngine SDL3::SDL3)

## install vulkan in your computer
find_package(Vulkan REQUIRED)
target_link_libraries(XEngine Vulkan::Vulkan)

##VMA
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
# add_subdirectory(VulkanMemoryAllocator)
target_link_libraries(XEngine GPUOpen::VulkanMemoryAllocator)

##VK-BOOSTRAP
find_package(vk-bootstrap CONFIG REQUIRED)
target_link_libraries(XEngine vk-bootstrap::vk-bootstrap)

##bgfx
find_package(bgfx CONFIG REQUIRED)
target_link_libraries(XEngine bgfx::bgfx)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(XEngine imgui)

find_package(tomlplusplus CONFIG REQUIRED)
target_link_libraries(XEngine tomlplusplus::tomlplusplus)

find_package(RTTR CONFIG REQUIRED Core)
target_link_libraries(XEngine RTTR::Core)

find_package(Taskflow CONFIG REQUIRED)
target_link_libraries(XEngine Taskflow::Taskflow)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(XEngine spdlog::spdlog)



target_link_libraries(XTestCase SDL3::SDL3)
target_link_libraries(XTestCase Vulkan::Vulkan)
target_link_libraries(XTestCase GPUOpen::VulkanMemoryAllocator)
target_link_libraries(XTestCase vk-bootstrap::vk-bootstrap)
target_link_libraries(XTestCase bgfx::bgfx)
target_link_libraries(XTestCase imgui)
target_link_libraries(XTestCase tomlplusplus::tomlplusplus)
target_link_libraries(XTestCase RTTR::Core)
target_link_libraries(XTestCase Taskflow::Taskflow)
target_link_libraries(XTestCase spdlog::spdlog)