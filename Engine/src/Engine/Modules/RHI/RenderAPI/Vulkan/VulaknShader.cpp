#include "VulaknShader.h"



void VulaknShader::CreateShader(VulaknContext* Context, const std::vector<char>& code)
{

    VkShaderModuleCreateInfo ShaderModuleCreateInfo = {};
    ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    ShaderModuleCreateInfo.codeSize = code.size();
    ShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());



    VulaknShader* Shader = new VulaknShader();



    vkCreateShaderModule(Context->Device,&ShaderModuleCreateInfo,nullptr,&Shader->shaderModule);
    
}

void VulaknShader::UseShader()
{
    //vkCmdBindPipeline()
}
