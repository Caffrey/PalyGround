#include "TestDrawVulkan.h"
#include <fstream>
void TestDrawVulkan::Draw(VkCommandBuffer& cmd)
{
    vkCmdBindPipeline(cmd,VK_PIPELINE_BIND_POINT_GRAPHICS,this->GraphicPipeline);
    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffer, &offset);
    vkCmdDraw(cmd,3,1,0,0);
}

void TestDrawVulkan::Init(VulkanContext& Context)
{
    InitMesh(Context);
    InitInputDesc();
    InitShader(Context);
    InitPipeline(Context);
   
}

void TestDrawVulkan::InitMesh(VulkanContext& Context)
{
    //vertex positions
    Vertex.push_back( { 1.f, 1.f, 0.0f });
    Vertex.push_back( {-1.f, 1.f, 0.0f });
    Vertex.push_back( { 0.f,-1.f, 0.0f });

    //vertex colors, all green
    Colors.push_back( { 0.f, 1.f, 0.0f });
    Colors.push_back( { 0.f, 1.f, 0.0f });
    Colors.push_back( { 0.f, 1.f, 0.0f });

 
    
    //Create Buffer
    int BufferSize = Vertex.size();
    for (int i = 0 ; i< BufferSize;i++)
    {
        Buffers.push_back(Vertex[i]);
        Buffers.push_back(Colors[i]);
    }
    int VertexSize = sizeof(Vector3)*2;
    
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //this is the total size, in bytes, of the buffer we are allocating
    bufferInfo.size = BufferSize * VertexSize;
    //this buffer is going to be used as a Vertex Buffer
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


    //let the VMA library know that this data should be writeable by CPU, but also readable by GPU
    VmaAllocationCreateInfo vmaallocInfo = {};
    vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    //allocate the buffer
    vmaCreateBuffer(Context.Allocator, &bufferInfo, &vmaallocInfo,
        &vertexBuffer,
        &VertexAllocation,
        nullptr);

    void* Data;
    vmaMapMemory(Context.Allocator,VertexAllocation,&Data);
    memcpy(Data,Buffers.data(),BufferSize*VertexSize);
    vmaUnmapMemory(Context.Allocator,VertexAllocation);
}

void TestDrawVulkan::InitPipeline(VulkanContext& Context)
{
    VkGraphicsPipelineCreateInfo pipelineInfo = {};

VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = InputBindings.size();
    vertexInputInfo.vertexAttributeDescriptionCount = InputAttributes.size();
    vertexInputInfo.pVertexBindingDescriptions = InputBindings.data();
    vertexInputInfo.pVertexAttributeDescriptions = InputAttributes.data();


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
    //pipelineInfo.pStages
    
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pRasterizationState = &rasterizerInfo;
    pipelineInfo.pMultisampleState = &multisampleInfo;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = PipelineLayout;

    
    vkCreateGraphicsPipelines(Context.Device,VK_NULL_HANDLE,1,&pipelineInfo,nullptr,&GraphicPipeline);
}


void TestDrawVulkan::InitInputDesc()
{
    int VertexSize = sizeof(Vector3)*3;
    
    //vetex bind
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = VertexSize;
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    InputBindings.push_back(bindingDescription);    

    VkVertexInputAttributeDescription positionDescription = CreateAttributeDesc(0,0);
    VkVertexInputAttributeDescription NormalAttributeDesc = CreateAttributeDesc(sizeof(Vector3),1);
    VkVertexInputAttributeDescription ColorAttributeDesc = CreateAttributeDesc(sizeof(Vector3),2);
    InputAttributes.push_back(positionDescription);
    InputAttributes.push_back(NormalAttributeDesc);
    InputAttributes.push_back(ColorAttributeDesc);
    
}

void TestDrawVulkan::InitShader(VulkanContext& Context)
{
    load_shader_module(Context, "./Vertex.spv",&VertexShader);
    load_shader_module(Context, "./Pixel.spv",&PixelShader);
    
}

void TestDrawVulkan::load_shader_module(VulkanContext& Context, const char* FilePath, VkShaderModule* OutModule)
{
    std::ifstream file(FilePath,std::ios::ate | std::ios::binary);

    size_t fileSize = (size_t)file.tellg();

    //spirv expects the buffer to be on uint32, so make sure to reserve an int vector big enough for the entire file
    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

    //put file cursor at beginning
    file.seekg(0);

    //load the entire file into the buffer
    file.read((char*)buffer.data(), fileSize);

    //now that the file is loaded into the buffer, we can close it
    file.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;

    //codeSize has to be in bytes, so multiply the ints in the buffer by size of int to know the real size of the buffer
    createInfo.codeSize = buffer.size() * sizeof(uint32_t);
    createInfo.pCode = buffer.data();

    //check that the creation goes well.
    VkShaderModule shaderModule;
    vkCreateShaderModule(Context.Device, &createInfo, nullptr, &shaderModule);
    *OutModule = shaderModule;
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
