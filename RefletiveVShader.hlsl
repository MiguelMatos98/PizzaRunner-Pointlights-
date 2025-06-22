///////////////////////////////////////////////////////////
// Reflective Vertex Shader
///////////////////////////////////////////////////////////

cbuffer TransformBuffer : register(b0)
{
    matrix Matrix_WVP; // World-View-Projection matrix
    matrix Matrix_W; // World matrix for object transformation
    float2 pading;
};

struct VIn
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VOut
{
    float4 PositionCS : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD0;
    float3 WorldPosition : TEXCOORD1;
    float3 Normal : TEXCOORD2;
};

VOut main(VIn input)
{
    VOut output;

    // Transform position
    float4 worldPos = mul(Matrix_W, float4(input.Position, 1));
    output.WorldPosition = worldPos.xyz; // Pass world position
    
    output.PositionCS = mul(Matrix_WVP, float4(input.Position, 1));

     // Pass UVs
    output.UV = input.UV;
    
    // Calculate world-space normal
    float3 worldNormal = normalize(mul((float3x3) Matrix_W, input.Normal));
    output.Normal = worldNormal;

    // Combine lighting
    output.Color = input.Color;

    return output;
}
