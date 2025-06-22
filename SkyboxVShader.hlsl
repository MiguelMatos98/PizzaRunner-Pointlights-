///////////////////////////////////////////////////////////
// Optimized Skybox Vertex Shader
///////////////////////////////////////////////////////////
cbuffer SkyboxTransformBuffer : register(b1)
{
    matrix WVP; // View-Projection matrix (combined)
};

struct VIn
{
    float3 position : POSITION; // Vertex position in local space
    float4 colour : COLOR; // Optional color tint or fade
    float2 uv : TEXCOORD;
    float3 norm : NORMAL;
};

struct VOut
{
    float4 position : SV_Position; // Clip-space position
    float4 colour : COLOR; // Optional color tint
    float3 uvw : TEXCOORD0; // Direction for cubemap sampling
};

VOut main(VIn input)
{
    VOut output;
    
    // Position
    output.position = mul(WVP, float4(input.position, 1.0f));
    
    // Texture coords 
    output.uvw = normalize(input.position.xyz);
    
    output.colour = input.colour;
    
    return output;
}


