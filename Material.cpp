#include "Material.h"

Material::Material()
    : m_Texture{ nullptr },
    m_SamplerState{ nullptr },
    opacity{ 1.0f },              // Fully opaque by default
    refractionIndex{ 1.0f },   // Typical default refractive index for air (~1.0)
    m_DiffuseColor{ 1.0f, 1.0f, 1.0f, 1.0f },
    specularColor{ 1.0f, 1.0f, 1.0f, 1.0f },
    shininess{ 0.9f },
    metallic{ 0.0f },
    roughness{ 0.5f },          // Some roughness to avoid perfect mirror by default
    glossiness{ 0.0f },
    reflectionStrength{ 0.0f }  // No reflection by default (adjust if needed)
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
