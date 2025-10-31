#include "RHIBuffer.h"


#define MatchMarco(LayoutName,Flag,OffsetName,Offset)\
    if((Flag & LayoutName)>0)\
        return OffsetName;\
    if((Flag & LayoutName)>0)\
    {\
        OffsetName += Offset;\
    }
#define MatchLengthMarco(LayoutName,Flag,OffsetName,Offset)\
    if((Flag & LayoutName)>0)\
    {\
        OffsetName += Offset;\
    }\
    if((Flag & LayoutName)>0)\
        return OffsetName;\
    
bool RHIVertexStreamComponent::HaveLayout(RHIVertexLayoutType Flag)
{
    return (Layout & Flag) > 0;
}

bool RHIVertexStreamComponent::CalLayoutLength(RHIVertexLayoutType Flag, int& Length)
{
    MatchLengthMarco(RHIVertexLayoutType::Position,Flag,Length,3)
   MatchLengthMarco(RHIVertexLayoutType::Normal,Flag,Length,3)
   MatchLengthMarco(RHIVertexLayoutType::Tangent,Flag,Length,4)
   MatchLengthMarco(RHIVertexLayoutType::Bitangent,Flag,Length,4)
   MatchLengthMarco(RHIVertexLayoutType::Color,Flag,Length,4)
   MatchLengthMarco(RHIVertexLayoutType::TexCoord0,Flag,Length,4)
   MatchLengthMarco(RHIVertexLayoutType::TexCoord1,Flag,Length,4)
   MatchLengthMarco(RHIVertexLayoutType::TexCoord2,Flag,Length,4)
   MatchLengthMarco(RHIVertexLayoutType::TexCoord3,Flag,Length,4)
   MatchLengthMarco(RHIVertexLayoutType::TexCoord4,Flag,Length,4)
   MatchLengthMarco(RHIVertexLayoutType::TexCoord5,Flag,Length,4)
}

bool RHIVertexStreamComponent::GetLayoutOffset(RHIVertexLayoutType Flag, int& Offset)
{
    MatchMarco(RHIVertexLayoutType::Position,Flag,Offset,3)
    MatchMarco(RHIVertexLayoutType::Normal,Flag,Offset,3)
    MatchMarco(RHIVertexLayoutType::Tangent,Flag,Offset,4)
    MatchMarco(RHIVertexLayoutType::Bitangent,Flag,Offset,4)
    MatchMarco(RHIVertexLayoutType::Color,Flag,Offset,4)
    MatchMarco(RHIVertexLayoutType::TexCoord0,Flag,Offset,4)
    MatchMarco(RHIVertexLayoutType::TexCoord1,Flag,Offset,4)
    MatchMarco(RHIVertexLayoutType::TexCoord2,Flag,Offset,4)
    MatchMarco(RHIVertexLayoutType::TexCoord3,Flag,Offset,4)
    MatchMarco(RHIVertexLayoutType::TexCoord4,Flag,Offset,4)
    MatchMarco(RHIVertexLayoutType::TexCoord5,Flag,Offset,4)

    return false;
}

void RHIVertexStreamComponent::GetLayoutPointer(RHIVertexLayoutType Flag, float** StartIndex)
{
    int Offset = 0;
    GetLayoutOffset(Flag,Offset);
    *StartIndex = Data + Offset;
}

RHIVertexStreamComponent::RHIVertexStreamComponent()
{
    Stride = 0;
    VertexCount = 0;
    Data = nullptr;
    Layout = RHIVertexLayoutType::None;
}

RHIVertexStreamComponent::~RHIVertexStreamComponent()
{
    if(Data)
    {
        delete Data;
    }
}

void RHIVertexStreamComponent::UpdateLayout(RHIVertexLayoutType InFlag, int InVertexCount)
{
    if(InFlag == 0)
    {
        throw std::exception("Vertex Type Error");
    }
    
    if((InFlag ^ Layout) > 0)
    {
        //need new array, and reset data
        if(Data)
        {
            delete Data;
        }

        int Length = 0;
        CalLayoutLength(InFlag,Length);
        DataLength = Length * InVertexCount;
        Data = new float[DataLength];
    }
    Layout = InFlag;
}

void RHIVertexStreamComponent::UploadMeshToGPU()
{
}
