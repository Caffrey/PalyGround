#pragma once
#include "Core/World/Component.h"
#include "Core/Math/XMath.h"
class TransformComponent : Component
{
public:
    
    Vector3 LocalPosition;
    Quaternion LocalRotation;
    Vector3 LocalScale;    
};
