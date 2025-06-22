#pragma once
#include <directxmath.h>

struct Transform
{
	DirectX::XMVECTOR position = DirectX::XMVectorZero();
    DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
	DirectX::XMVECTOR scale = DirectX::XMVectorSet( 1.0f, 1.0f, 1.0f, 1.0f );

    DirectX::XMMATRIX GetWorldMatrix()
    {
        // Apply scaling, rotation, and translation
        DirectX::XMMATRIX Scale = DirectX::XMMatrixScalingFromVector(scale);
        DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationQuaternion(rotation);
        DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslationFromVector(position);

        // Combine into a world matrix
        return Scale * Rotation * Translation;
    }

    // Local position, rotation and scale can also be implemented (Use worldToLocalMatrix and localToWorldMatrix)

    // Funtions for possible further accessiblity of the class
    /* 
        Transform& operator=(const Transform& other);

        const Transform& operator=(const Transform& other);

        Transform& operator=(Transform& other);

        // Delete move constructor and move assignment
        Transform(Transform&&) = delete;
        Transform& operator=(Transform&&) = delete;

        // Delete operator& to prevent referencing
        Transform* operator&() = delete;
        const Transform* operator&() const = delete;
    */
};

