#include "GameObject.h"

GameObject::GameObject(const Transform& transform, ID3D11ShaderResourceView* texture, ObjFileModel* model)
	: m_Transform{ transform }, m_IsPizza{ false }, m_Model{ model }
{
	m_Material.SetMaterialTexture(texture);
}

GameObject::GameObject(const XMVECTOR& position, const XMVECTOR& rotation, ID3D11ShaderResourceView* texture, ObjFileModel* model)
	: m_IsPizza{ false }, m_Model{ model }
{
	m_Transform.position = position;
	m_Transform.rotation = rotation;

	m_Material.SetMaterialTexture(texture);
}

void GameObject::Update(const double delta_Time)
{

}

Transform& GameObject::GetTransform()
{
	return m_Transform;
}

ObjFileModel* GameObject::GetModel()
{
	return m_Model;
}

void GameObject::SetModel(char* meshName, ID3D11Device& device, ID3D11DeviceContext& deviceContext)
{
	// Creatng a model object on the heap
	m_Model = new ObjFileModel(meshName, &device, &deviceContext);
}

void GameObject::SetModel(ObjFileModel* model)
{
	if (model == nullptr)
		return; // Print error
	else
		m_Model = model;
}

Material& GameObject::GetMaterial()
{
	return m_Material;
}


