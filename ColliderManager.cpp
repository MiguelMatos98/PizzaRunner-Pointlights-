#include "ColliderManager.h"
#include "GameManager.h"

vector<BoundingOrientedBox*> ColliderManager::m_Colliders{};

void ColliderManager::Initialize()
{
}

void ColliderManager::Shutdown()
{
	for (size_t i = 0; i < m_Colliders.size(); i++)
	{
		delete m_Colliders[i];
		m_Colliders[i] = nullptr;
	}
	m_Colliders.clear();
}

void ColliderManager::SetCollisionEvent(function<void()> callback)
{
	callback();
}

void ColliderManager::CheckForCollision() // Isolate this function for two meshes only 153
{
	static vector <size_t> colliderIndexes = {};
	constexpr int playerCollider = 0; // Player bring this index
	for (size_t otherColliderIndex = playerCollider + 1; otherColliderIndex < m_Colliders.size(); otherColliderIndex++)
	{
		// Check if the player intersects with the other collider and hasn't already been handled
		if (m_Colliders[playerCollider]->Intersects(*m_Colliders[otherColliderIndex]))
		{
			std::cout << "Player has intersected with Collider number(index): " << playerCollider << " - At the position: " << m_Colliders[otherColliderIndex]->Center.x << ", " << m_Colliders[otherColliderIndex]->Center.y << ", " << m_Colliders[otherColliderIndex]->Center.z << "\n";

			SetCollisionEvent([&otherColliderIndex]() { GameManager::UnregisterGameObject(otherColliderIndex); });
			SetCollisionEvent([]() { GameManager::GetPlayer().IncreaseScore(1); });
			colliderIndexes.push_back(otherColliderIndex);
		}

		// Optionally, you could include a reverse check for when the other collider intersects with the player (if needed)
		// In this case, it would typically be handled by the first check, but you could leave it for debugging.
		if (m_Colliders[otherColliderIndex]->Intersects(*m_Colliders[playerCollider]))
		{
			std::cout << "Collider number(index): " << otherColliderIndex << " has intersected with the Player" << "\n";
		}
	}

	if (!colliderIndexes.empty())
	{
		for (size_t i = 0; i < colliderIndexes.size(); i++)
		{
			m_Colliders.erase(m_Colliders.begin() + colliderIndexes[i]);
		}
		colliderIndexes.clear();
	}

}

void ColliderManager::SetColliderAtributes(size_t colliderindex, Transform transform)
{
	size_t currentColliderIndex = colliderindex;

	// Get rotation axis from transform
	XMVECTOR yawQuat = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), XMVectorGetY(transform.rotation));
	XMVECTOR pitchQuat = XMQuaternionRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), XMVectorGetX(transform.rotation));
	XMVECTOR rollQuat = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), XMVectorGetZ(transform.rotation));

	XMVECTOR combinedQuat = XMQuaternionMultiply(yawQuat, XMQuaternionMultiply(pitchQuat, rollQuat));

	// For no repetive indexes 
	if (currentColliderIndex >= 0 && currentColliderIndex <= m_Colliders.size())
		m_Colliders.push_back(new BoundingOrientedBox);

	SetColliderPosition(transform.position, currentColliderIndex);
	SetColliderRotation(XMQuaternionNormalize(combinedQuat), currentColliderIndex);
	SetColliderScale(transform.scale, currentColliderIndex);

	std::cout << "Collider number(index): " << colliderindex << " - At the position: " << m_Colliders[colliderindex]->Extents.x << ", " << m_Colliders[colliderindex]->Extents.y << ", " << m_Colliders[colliderindex]->Extents.z << "\n";
}

vector<BoundingOrientedBox*>& ColliderManager::GetColliders()
{
	return m_Colliders;
}

void ColliderManager::SetColliderPosition(const XMVECTOR& newPosition, size_t colliderIndex)
{
	if (colliderIndex > 0 && colliderIndex < m_Colliders.size())
		cout << "Valid index for setting position" << endl;

	XMStoreFloat3(&m_Colliders[colliderIndex]->Center, newPosition);
}

void ColliderManager::SetColliderRotation(const XMVECTOR& newRotation, size_t colliderIndex)
{
	if (colliderIndex > 0 && colliderIndex < m_Colliders.size())
		cout << "Valid index for setting position" << endl;

	XMStoreFloat4(&m_Colliders[colliderIndex]->Orientation, newRotation);
}

void ColliderManager::SetColliderScale(const XMVECTOR& newScale, size_t colliderIndex)
{
	if (colliderIndex > 0 && colliderIndex < m_Colliders.size())
		cout << "Valid index for setting position" << endl;

	XMStoreFloat3(&m_Colliders[colliderIndex]->Extents, newScale);
}
