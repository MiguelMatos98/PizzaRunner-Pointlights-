#pragma once
#include "GameObject.h"
#include "Transform.h"

class BoxCollider;

class Cube : public GameObject
{
private:
	BoxCollider* collider{nullptr};
public:
	Transform transform;
	Cube() = default;
	Cube(const DirectX::XMFLOAT3& position, BoxCollider* collider = nullptr);
	Cube(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation, BoxCollider* collider = nullptr);
	void Update() override;
	const BoxCollider* GetCollider();
};

