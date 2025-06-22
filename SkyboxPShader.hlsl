///////////////////////////////////////////////////////////
// Optimized Skybox Pixel Shader
///////////////////////////////////////////////////////////
TextureCube skyboxTexture : register(t2); // The cubemap representing the skybox
SamplerState sampler0 : register(s1); // Sampler for the cubemap

struct VOut
{
    float4 position : SV_Position; // Clip-space position
    float4 colour : COLOR; // Optional color tint
    float3 uvw : TEXCOORD0; // Direction for sampling the cubemap
};

float4 main(VOut input) : SV_Target
{
    // Sample the cubemap using the uvw direction
    float4 sampledSky = skyboxTexture.Sample(sampler0, input.uvw);

    // Combine the sampled skybox color with the optional tint
    return saturate(sampledSky * input.colour);
}

