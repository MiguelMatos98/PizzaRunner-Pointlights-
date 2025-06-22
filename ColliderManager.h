#pragma once
#include <DirectXCollision.h>
#include "SimpleMath.h"
#include "GameObject.h"
#include <vector>

class ColliderManager
{
private:
	static vector<BoundingOrientedBox*> m_Colliders;
public:
	static void Initialize();
	static void Shutdown();
	static void SetCollisionEvent(function<void()> callback);
	static void CheckForCollision();
	static void SetColliderAtributes(size_t colliderindex, Transform transform); // Make this only have a transform 
	static vector<BoundingOrientedBox*>& GetColliders();
	static void SetColliderPosition(const XMVECTOR& newPosition, size_t colliderIndex);
	static void SetColliderRotation(const XMVECTOR& newRotation, size_t colliderIndex);
	static void SetColliderScale(const XMVECTOR& newScale, size_t colliderIndex);
	//void SetColliderToObject(GameObject& object, BoundingOrientedBox* collider = nullptr);
};

