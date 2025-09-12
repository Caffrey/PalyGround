#pragma once
#include <string>

#include "Core/Assets/Material.h"
#include "Core/Assets/Mesh.h"

struct XMaterialSlot
{
public:
    int SubMeshIndex;
    std::vector<XMaterial> Materials;
};

class BaseMeshComponent
{
public:
    XMesh* Mesh;
    std::map<std::string,XMaterialSlot> MaterialSlotsMap;
};
