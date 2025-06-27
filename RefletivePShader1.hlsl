///////////////////////////////////////////////////////////
// Reflective Pixel Shader (Lighting + Reflections)
///////////////////////////////////////////////////////////
TextureCube skyboxTexture : register(t0); // Skybox texture for reflections
Texture2D texture0 : register(t1); // Diffuse texture
SamplerState sampler0 : register(s0); // Sampler

#define MAX_POINT_LIGHTS 8

struct PointLight
{
    float4 position; // 16 bytes
    float4 colour; // 16 bytes
};

cbuffer MaterialLightingBuffer : register(b1)
{
    float4 CameraPositionWS; // Camera position
    
    float4 ambientLightCol; // 16 bytes
    float4 directionLightDir; // 16 bytes
    float4 directionalLightCol; // 16 bytes
    
    PointLight pointLights[MAX_POINT_LIGHTS]; // 384 bytes
    
    float4 diffuseColor; // Diffuse material color
    float4 specularColor; // Specular material color
    float emision; // Shininess factor
    float reflectionStrength; // Strength of reflections
    float roughness; // Roughness factor
    
    
    float metallic; // Metallic factor
    // Transperant shader variables
    float opacity; // Opacity
    float refractionIndex; // Refraction index
    
    float2 padding; // 8 bytes
};

struct VIn
{
    float4 positionCS : SV_Position;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float3 normal : TEXCOORD2;
};

float4 main(VIn input) : SV_Target
{
    // Sample textures
    float4 sampled = texture0.Sample(sampler0, input.uv);

    float reflectionFactor = reflectionStrength;

    // Compute diffuse and reflection components
    float3 Fragmentnormal = normalize(input.normal);
    
    float3 viewDirection = normalize(CameraPositionWS.xyz - input.worldPosition.xyz);
    
    float4 reflectedSampled = skyboxTexture.Sample(sampler0, reflect(-viewDirection, Fragmentnormal));
    float3 lightDirection = normalize(directionLightDir.rgb); // Direction light direction
    float diffuseMask = saturate(dot(Fragmentnormal, lightDirection));
    float4 diffuseComponent = diffuseMask * diffuseColor * sampled * float4(1.0f, 1.0f, 1.0f, 1.0f); // Direction light color variable
    
    float3 halfVector = normalize(viewDirection + lightDirection);
    
    float rough = 0.5f;
    
    float NoH = saturate(dot(float3(Fragmentnormal), halfVector));
    half LoH = half(saturate(dot(lightDirection, halfVector)));
    float d = (NoH * NoH) * (rough * rough - 1) + 1.00001f;

    half LoH2 = LoH * LoH;
    
    half specularTerm = rough * rough / ((d * d) * max(0.1h, LoH2) * (rough * 4.0 + 2.0));
    
    float4 reflectionComponent = diffuseMask * min(specularTerm, 10.0f) / 10.0f * reflectionStrength * float4(1.0f, 1.0f, 1.0f, 1.0f); //  Specular color variable
    
   // return reflectionComponent;
    
    float FersenelMask = pow(1 - saturate(dot(Fragmentnormal, viewDirection)), 5.0f); // Fersnel term effect // Temp reflection var
    
   // float4 reflectionComponent = reflectedSampled * reflectionFactor;

   // // Point lights...
    float3 pointFinal = float3(0, 0, 0);
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        float3 pointLightDir = normalize(pointLights[i].position.xyz - input.worldPosition.xyz);
        float pointLightDistance = length(pointLights[i].position.xyz - input.worldPosition.xyz);
       // float pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);
        float pointAmount = saturate(dot(input.normal, pointLightDir)); //* pointLightAttenuation;

        pointFinal += pointLights[i].colour.rgb * pointAmount * 5.0f;
    }
   //// Compute specular highlights using world-space viewDir
    //float3 halfVector = normalize(viewDirection + input.normal); // Blinn-Phong half-vector
    float3 specularHighlight = pow(max(dot(input.normal, halfVector), 0.0f), 1.0) * specularColor.rgb;
    reflectionComponent.rgb += specularHighlight;

                                                                                                                                                                    // Relection streng not being passed properly
    return saturate(lerp(saturate(reflectionComponent + diffuseComponent + (1 - diffuseMask) * ambientLightCol * diffuseColor * sampled), reflectedSampled, FersenelMask * reflectionStrength) * (1 - emision) + float4(emision * diffuseColor * sampled.rgb, sampled.a) + float4(pointFinal * (1 - emision), 0.0f));

   // return saturate(float4(pointFinal, 1.0f));
}


