#include "Material.h"

Material::Material()
    : m_Texture{ nullptr },
    m_SamplerState{ nullptr },
    opacity{0.0f},
    refractionIndex{ 1.0f },
    m_DiffuseColor{ 1.0f, 1.0f, 1.0f, 1.0f },
    specularColor{ 1.0f, 1.0f, 1.0f, 1.0f }, // Brighter specular emmision
    shininess{ 0.9f }, // Higher shininess
    metallic{ 0.0f },
    roughness{ 0.f },
    glossiness{ 0.0f },
    reflectionStrength{ 1.0f }
{
}

void Material::SetMaterialTexture(ID3D11ShaderResourceView* testure_)
{
	m_Texture = testure_;
}

ID3D11SamplerState* Material::GetMaterialSamplerState()
{
    return m_SamplerState;
}

void Material::SetMaterialSamplerState(ID3D11SamplerState* samplerState)
{
    m_SamplerState = samplerState;
}
