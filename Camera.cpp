#include "Camera.h"
#include <cmath>
#include "GameManager.h"

using namespace DirectX;

XMMATRIX Camera::GetViewMatrix()
{
	XMVECTOR forward = GetForwardDirection();
	XMVECTOR up = GetUpDirection();
	return XMMatrixLookToLH({ XMVectorGetX(m_Transform.position), XMVectorGetY(m_Transform.position), XMVectorGetZ(m_Transform.position), 0.0f }, forward, up);
}

XMVECTOR Camera::GetForwardDirection()
{
	return { XMVectorSet(sin(m_Yaw) * sin(m_Pitch), cos(m_Pitch), cos(m_Yaw) * sin(m_Pitch), 1.0f) };
}

XMVECTOR Camera::GetRightDirection()
{
	return { XMVectorSet(cos(m_Yaw), 0.0f, -sin(m_Yaw), 1.0f) };
}

XMVECTOR Camera::GetUpDirection()
{
	return XMVector3Cross(GetForwardDirection(), GetRightDirection());
}

void Camera::SetYaw(const float yawValue)
{
	m_Yaw = yawValue;
}

void Camera::SetPitch(const float pitchValue)
{
	m_Pitch = pitchValue;
}

float Camera::GetYaw()
{
	return m_Yaw;
}

float Camera::GetPitch()
{
	return m_Pitch;
}

ObjFileModel& Camera::GetSkyBoxModel()
{
	return *m_SkyboxModel;
}

void Camera::SetSkyboxModel(char* modelType, ID3D11Device& device, ID3D11DeviceContext& deviceContext)
{
}

// Pitch and yaw should be rotation only
Camera::Camera(const XMVECTOR& position, const XMVECTOR& rotation) : GameObject(position, rotation), m_Pitch{ XM_PIDIV2 }, m_Yaw{ 0.0f }, m_SkyboxModel{ nullptr }, m_SkyboxScaleX{ 1000.0f }, m_SkyboxScaleY{ 1000.0f }, m_SkyboxScaleZ{ 1000.0f }
{
	//m_SkyboxModel = new ObjFileModel((char*)"cube.obj", &GameManager::GetInstance().GetRenderer()->GetDevice(), &GameManager::GetInstance().GetRenderer()->GetDeviceContext());

	XMVECTOR skyboxRotation = XMQuaternionIdentity();
	XMVECTOR skyboxScale = XMVectorSet(m_SkyboxScaleX, m_SkyboxScaleY, m_SkyboxScaleZ, 1.0f);

	// Creating a temporary variable to extract the camera's position for initializing the skybox position.
	// This ensures the skybox is positioned based on the camera's current position.	
	XMVECTOR skyboxPosition = { 0.0f, 0.57f, 0.0f, 0.0f };

	m_SkyboxTransform = { skyboxPosition, skyboxRotation, skyboxScale };
}

Camera::~Camera()
{
	if (m_SkyboxModel != nullptr) { delete m_SkyboxModel; m_SkyboxModel = nullptr; }
}

void Camera::Update(const double delta_Time)
{
}

void Camera::SetSkyboxModel(ObjFileModel* skybox_Model)
{
	if (skybox_Model == nullptr)
		return; // Print error
	else
		m_SkyboxModel = skybox_Model;
}

Transform& Camera::GetSkyboxTransform()
{
	return m_SkyboxTransform;
}