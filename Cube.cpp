#include "Cube.h"


Cube::Cube(const DirectX::XMFLOAT3& position, BoxCollider* collider)
{
	/*if (collider == nullptr)
		throw std::invalid_argument("Valid collider has to be provided");*/

	this->transform.position = position;
	this->collider = collider;
}

Cube::Cube(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation, BoxCollider* collider)
{
	/*if (collider == nullptr)
		throw std::invalid_argument("Valid collider has to be provided");*/

	this->transform.position = position;
	this->transform.rotation = rotation;
	this->collider = collider;
}

void Cube::Update()
{
}

const BoxCollider* Cube::GetCollider()
{
	return collider;
}
