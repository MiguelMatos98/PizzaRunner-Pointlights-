#pragma once
#include "Transform.h"
#include "ObjFileModel.h"
#include <DirectXCollision.h>
#include <iostream>
#include "Material.h"

class GameObject
{
private:
protected:
	virtual void Update(const double delta_Time);
	ObjFileModel* m_Model;
	Transform m_Transform;
	Material m_Material;
public:
	bool m_IsPizza;
	GameObject(const Transform& transform, ID3D11ShaderResourceView* texture = nullptr, ObjFileModel* model = nullptr);
	GameObject(const XMVECTOR& position, const XMVECTOR& rotation, ID3D11ShaderResourceView* texture = nullptr, ObjFileModel* model = nullptr);
	ObjFileModel* GetModel();
	void SetModel(char* modelType, ID3D11Device& device, ID3D11DeviceContext& deviceContext);
	void SetModel(ObjFileModel* model);
	Transform& GetTransform();
	Material& GetMaterial();
};