#pragma once

class Object
{
public:
    Object() = default;
    virtual ~Object()= default;
    virtual void Serialization(){};
    virtual void Deserialization(){};
};
