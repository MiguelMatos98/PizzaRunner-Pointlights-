#pragma once
#include "d3d11.h"
#include "DirectXMath.h"
class Material
{
private:
    ID3D11ShaderResourceView* m_Texture;
    ID3D11SamplerState* m_SamplerState;
    DirectX::XMVECTOR m_DiffuseColor;
    DirectX::XMVECTOR specularColor;
    float shininess;
    float opacity;
    float reflectionStrength;
    float metallic;
    float roughness;
    float glossiness;
    float refractionIndex;
public:
    Material();
    inline DirectX::XMVECTOR GetDiffuseColor() const { return m_DiffuseColor; }
    inline DirectX::XMVECTOR GetSpecularColor() const { return specularColor;  }
    inline float GetShininess() const { return shininess; }
    inline float GetOpacity() const { return opacity; }
    inline float GetReflectionStrength() const { return reflectionStrength; }
    inline float GetMetallic() const { return metallic; }
    inline float GetRoughness() const { return roughness; }
    inline float GetGlossiness() const { return glossiness; }
    inline float GetRefractionIndex() const { return refractionIndex; }
    inline ID3D11ShaderResourceView* GetMaterialTexture() { return m_Texture; }
    void SetMaterialTexture(ID3D11ShaderResourceView* testure_);
    ID3D11SamplerState* GetMaterialSamplerState();
    void SetMaterialSamplerState(ID3D11SamplerState* samplerState);
};

