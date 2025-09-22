#include "TestDrawVulkan.h"
#include <fstream>
#include <iostream>

#include "VulkanUtils.h"

TestDrawVulkan::TestDrawVulkan()
{
    m_VertexFormat = VertexFormat();
}


void TestDrawVulkan::Draw(VkCommandBuffer& cmd)
{
    
    vkCmdBindPipeline(cmd,VK_PIPELINE_BIND_POINT_GRAPHICS,this->GraphicPipeline);

    
    VkBuffer vertexBuffers[] = {NewVertexBuffer->Buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(cmd, static_cast<uint32_t>(Vertex.size()), 1, 0, 0);

}   

void TestDrawVulkan::Init(VulkanContext& Context)
{
    InitMesh(Context);
    InitInputDesc();
    
    InitPipeline(Context);

    
}
uint32_t findMemoryType(VulkanContext& Context, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(Context.PhysicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void TestDrawVulkan::InitMesh(VulkanContext& Context)
{
    //vertex positions
    Vertex.push_back( TestVertexStruct());
    Vertex.push_back( TestVertexStruct());
    Vertex.push_back( TestVertexStruct());

    Vertex[0].pos = {0.5,-0.5, 0};
    Vertex[1].pos = {0.5,0.5, 0};
    Vertex[2].pos = {-0.5,-0.5, 0};

    Vertex[0].color = {0,0, 0};
    Vertex[1].color = { 0.5,0.5,0.5 };
    Vertex[2].color = { 1,0, 0};

    std::vector<int> Offsets;
    Offsets.push_back(offsetof(TestVertexStruct,pos));
    Offsets.push_back(offsetof(TestVertexStruct,normal));
    Offsets.push_back(offsetof(TestVertexStruct,color));

    std::vector<float> VertexFloatData;
    for(auto vertex : Vertex)
    {
        VertexFloatData.push_back(vertex.pos[0]);
        VertexFloatData.push_back(vertex.pos[1]);
        VertexFloatData.push_back(vertex.pos[2]);
        VertexFloatData.push_back(vertex.normal[0]);
        VertexFloatData.push_back(vertex.normal[1]);
        VertexFloatData.push_back(vertex.normal[2]);
        VertexFloatData.push_back(vertex.color[0]);
        VertexFloatData.push_back(vertex.color[1]);
        VertexFloatData.push_back(vertex.color[2]);
    }
        
    m_VertexFormat = VertexFormat(sizeof(TestVertexStruct),Vertex.size(),VertexFloatData,Offsets);
    NewVertexBuffer = VulkanVertexBuffer::CreateVertexBuffer(Context.Allocator,m_VertexFormat.VertexSide,m_VertexFormat.VertexOffset.data(),m_VertexFormat.VertexOffset.size()
        ,m_VertexFormat.VertexData.data(),m_VertexFormat.VertexCount);
    
    //Create Buffer
   //--
    // int VertexSize = sizeof(TestVertexStruct);
    //
    // VkBufferCreateInfo bufferInfo = {};
    // bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    // //this is the total size, in bytes, of the buffer we are allocating
    // bufferInfo.size = Vertex.size() * VertexSize;
    // //this buffer is going to be used as a Vertex Buffer
    // bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    //
    //
    // //let the VMA library know that this data should be writeable by CPU, but also readable by GPU
    // VmaAllocationCreateInfo vmaallocInfo = {};
    // vmaallocInfo.usage =  VMA_MEMORY_USAGE_CPU_TO_GPU;
    //
    // //allocate the buffer
    // vmaCreateBuffer(Context.Allocator, &bufferInfo, &vmaallocInfo,
    //     &vertexBuffer,
    //     &VertexAllocation,
    //     nullptr);
    //
    // void* Data;
    // vmaMapMemory(Context.Allocator,VertexAllocation,&Data);
    // memcpy(Data,Vertex.data(),Vertex.size() * VertexSize);
    // vmaUnmapMemory(Context.Allocator,VertexAllocation);
    // //---

}

void TestDrawVulkan::InitPipeline(VulkanContext& Context)
{
    InitShader(Context);
    VkGraphicsPipelineCreateInfo pipelineInfo = {};

VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = InputBindings.size();
    vertexInputInfo.pVertexBindingDescriptions = InputBindings.data();
    
    vertexInputInfo.pVertexAttributeDescriptions = InputAttributes.data();
    vertexInputInfo.vertexAttributeDescriptionCount = InputAttributes.size();


    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
    multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleInfo.pNext = nullptr;

    multisampleInfo.sampleShadingEnable = VK_FALSE;
    //multisampling defaulted to no multisampling (1 sample per pixel)
    multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleInfo.minSampleShading = 1.0f;
    multisampleInfo.pSampleMask = nullptr;
    multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleInfo.alphaToOneEnable = VK_FALSE;


    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;


    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;

    //empty defaults
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    vkCreatePipelineLayout(Context.Device,&pipelineLayoutInfo,nullptr,&PipelineLayout);


    pipelineInfo.stageCount = 2;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    shaderStages.push_back(
    VulkanUtils::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, VertexShader));

    shaderStages.push_back(
        VulkanUtils::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, PixelShader));
    
    pipelineInfo.pStages = shaderStages.data();
    
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pRasterizationState = &rasterizerInfo;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = PipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    
   auto r = vkCreateGraphicsPipelines(Context.Device,VK_NULL_HANDLE,1,&pipelineInfo,nullptr,&GraphicPipeline);
    std::cout << r;
}


void TestDrawVulkan::InitInputDesc()
{
    // int VertexSize = sizeof(VertexStruct);
    //
    // //vetex bind
  
    // VkVertexInputBindingDescription bindingDescription = {};
    // bindingDescription.binding = 0;
    // bindingDescription.stride =   m_VertexFormat.VertexSide;;
    // bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    InputBindings.push_back(m_VertexFormat.BindingDesc);    
    
    // VkVertexInputAttributeDescription positionDescription = CreateAttributeDesc(offsetof(VertexStruct,pos),0);
    // VkVertexInputAttributeDescription NormalAttributeDesc = CreateAttributeDesc(offsetof(VertexStruct,color),1);
    // VkVertexInputAttributeDescription ColorAttributeDesc = CreateAttributeDesc(offsetof(VertexStruct,normal),2);
    // InputAttributes.push_back(positionDescription);
    // InputAttributes.push_back(NormalAttributeDesc);
    // InputAttributes.push_back(ColorAttributeDesc);

    for(auto desc : m_VertexFormat.AttributeDesc)
    { 
        InputAttributes.push_back(desc);
    }
    
}

void TestDrawVulkan::InitShader(VulkanContext& Context)
{
    load_shader_module(Context, "D:/Engine/PalyGround/Engine/src/Engine/Shader/Vertex.vert",&VertexShader);
    load_shader_module(Context, "D:/Engine/PalyGround/Engine/src/Engine/Shader/Pixel.frag",&PixelShader);
    
}

void TestDrawVulkan::createRenderPass(VulkanContext& Context)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(Context.Device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void TestDrawVulkan::load_shader_module(VulkanContext& Context, const char* FilePath, VkShaderModule* OutModule)
{

    std::ifstream file(FilePath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;

    //codeSize has to be in bytes, so multiply the ints in the buffer by size of int to know the real size of the buffer
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

    //check that the creation goes well.
    auto r= vkCreateShaderModule(Context.Device, &createInfo, nullptr, OutModule );
    std::cout << r;
}

VkVertexInputAttributeDescription TestDrawVulkan::CreateAttributeDesc(int offset, int location)
{
    VkVertexInputAttributeDescription positionDescription = {};
    positionDescription.binding = 0;
    positionDescription.location = location;
    positionDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionDescription.offset = offset;

    return positionDescription;
}
