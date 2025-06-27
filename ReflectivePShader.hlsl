///////////////////////////////////////////////////////////
// Reflective Pixel Shader (Lighting + Reflections)
///////////////////////////////////////////////////////////
TextureCube skyboxTexture : register(t0); // Skybox texture for reflections
Texture2D texture0 : register(t1); // Diffuse texture
SamplerState sampler0 : register(s0); // Sampler

#define MAX_POINT_LIGHTS 8

struct PointLight
{
    float3 position; // 12 bytes
    float padding; // 4 bytes
    
    float3 colour; // 12 bytes
    float strength; // 4 bytes

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
    float opacity; // Opacity
    
    // Transperant shader variables
    float refractionIndex; // Refraction index
    float metallic; // Metallic factor

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
    // Sample diffuse texture
    float4 sampled = texture0.Sample(sampler0, input.uv);

    // Normalize normals and calculate view direction
    float3 Fragmentnormal = normalize(input.normal);
    float3 viewDirection = normalize(CameraPositionWS.xyz - input.worldPosition);

    // Sample environment reflection
    float4 reflectedSampled = skyboxTexture.Sample(sampler0, reflect(-viewDirection, Fragmentnormal));

    // Directional light calculations
    float3 lightDirection = normalize(directionLightDir.rgb);
    float diffuseMask = saturate(dot(Fragmentnormal, lightDirection));

    // Diffuse component with directional light color and diffuse texture
    float4 diffuseComponent = diffuseMask * diffuseColor * sampled;

    // Blinn-Phong half-vector for specular
    float3 halfVector = normalize(viewDirection + lightDirection);
    float NoH = saturate(dot(Fragmentnormal, halfVector));

    // Roughness to shininess conversion (roughness=0 => shininess=64, roughness=1 => shininess=1)
    float shininess = lerp(64.0f, 1.0f, roughness);

    // Specular calculation (fixed syntax errors)
    float rough = roughness;
    half LoH = half(saturate(dot(lightDirection, halfVector)));
    float d = (NoH * NoH) * (rough * rough - 1.0f) + 1.00001f;
    half LoH2 = LoH * LoH;
    half specularTerm = rough * rough / ((d * d) * max(0.1h, LoH2) * (rough * 4.0 + 2.0));

    // Specular component modulated by diffuseMask and reflectionStrength
    float4 reflectionComponent = diffuseMask * min(specularTerm, 10.0f) / 10.0f * reflectionStrength * float4(1.0f, 1.0f, 1.0f, 1.0f);

    // Fresnel term
    float FersenelMask = pow(1.0f - saturate(dot(Fragmentnormal, viewDirection)), 5.0f);

    // Accumulate point light contributions
    float3 pointFinal = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        float3 pointLightDir = pointLights[i].position - input.worldPosition;
        float pointLightDistance = length(pointLightDir);
        float3 pointLightDirNorm = normalize(pointLightDir);

        // Inverse square attenuation
        float pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);

        // Lambert diffuse
        float pointAmount = saturate(dot(Fragmentnormal, pointLightDirNorm)) * pointLightAttenuation;

        // Accumulate point light color
        pointFinal += pointLights[i].colour * pointAmount;

        // (Optional: Add point light specular here if needed)
    }

    // Specular highlight using Blinn-Phong model
    float3 specularHighlight = pow(max(dot(input.normal, halfVector), 0.0f), shininess) * specularColor.rgb;
    reflectionComponent.rgb += specularHighlight;

    // Final color combining diffuse, reflection, ambient, emission, and point lights
    float4 color = saturate(lerp(saturate(reflectionComponent + diffuseComponent + (1.0f - diffuseMask) * ambientLightCol * diffuseColor * sampled), reflectedSampled, FersenelMask * reflectionStrength) * (1.0f - emision) + float4(emision * diffuseColor.rgb * sampled.rgb, sampled.a) + float4(pointFinal * (1.0f - emision), 0.0f));

    return color;
}