#pragma once
#include "Transform.h"
#include "ObjFileModel.h"
#include "GameObject.h"
#include "directxmath.h"

class Camera : public GameObject
{
private:
	float m_SkyboxScaleX;
	float m_SkyboxScaleY;
	float m_SkyboxScaleZ;
	float m_Pitch;
	float m_Yaw;
	ObjFileModel* m_SkyboxModel;
	Transform m_SkyboxTransform;
public:
	Camera(const XMVECTOR& position, const XMVECTOR& rotation);
	~Camera();
	void Update(const double delta_Time) override;
	ObjFileModel& GetSkyBoxModel();
	void SetSkyboxModel(char* modelType, ID3D11Device& device, ID3D11DeviceContext& deviceContext);
	void SetSkyboxModel(ObjFileModel* skyboxModel); // Parameters should be snake_case
	float GetYaw();
	void SetYaw(const float yawValue);
	float GetPitch();
	void SetPitch(const float pitchValue);
	Transform& GetSkyboxTransform();
	XMMATRIX GetViewMatrix();
	XMVECTOR GetForwardDirection();
	XMVECTOR GetRightDirection();
	XMVECTOR GetUpDirection();
};

