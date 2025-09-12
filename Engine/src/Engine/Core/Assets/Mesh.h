#pragma once

#include "Core/Object.h"
#include "Modules/RHI/RHIBuffer.h"
#include <vector>
#include "Core/Math/Math.h"

struct XSubMesh
{
    int IndexStart;
    int IndexEnd;
};

class XMesh : public Object
{
    
protected:
    std::vector<Vector3> Vertices;
    std::vector<int> Indices;

    std::vector<Vector3> Normal;
    std::vector<Vector4> Tangent;
    std::vector<Vector4> Color;

    std::vector<Vector4> Uv;
    std::vector<Vector4> Uv1;
    std::vector<Vector4> Uv2;
    std::vector<Vector4> Uv3;
    std::vector<Vector4> Uv4;
    std::vector<Vector4> Uv5;
    std::vector<Vector4> Uv6;
    std::vector<Vector4> Uv7;
    std::vector<Vector4> Uv8;

    std::vector<XSubMesh> SubMesh;
public:
    RHIVertexBuffer* VertexBuffer;
};
