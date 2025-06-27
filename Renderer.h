#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <iterator>
#include "ReadData.h"
#include "text2D.h"
#include "objFileModel.h"
#include <Mouse.h>
#include <Keyboard.h>
#include "Camera.h"

#define MAX_POINT_LIGHTS 8
#define SCREEN_WIDTH  1300
#define SCREEN_HEIGHT 940

class Renderer
{
private:
	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
		XMFLOAT2 UV;
		XMFLOAT3 Normal;
	}; // Usually apart of the mesh class and then it's passed to the renderer 

	struct PointLight
	{
		DirectX::XMFLOAT3 position = { 0.f, 0.f, 0.f }; // 12 bytes
		float strength = 1.0f;                          // 4 bytes

		DirectX::XMFLOAT3 colour = { 1.f, 1.f, 1.f };  // 12 bytes
		float padding = 0.f;                            // 4 bytes (to align to 16 bytes)
	};

	struct SkyboxTransformBuffer
	{
		XMMATRIX WVP = XMMatrixIdentity(); // World-View-Projection matrix
	};

	struct TransformBuffer
	{
		XMMATRIX WVP = XMMatrixIdentity(); // World-View-Projection matrix (For Camera Clip Space)
		XMMATRIX WorldMatrix = XMMatrixIdentity(); // World-Matrix 

		float padding[2];					// 8 bytes (align to 16 bytes)
	};

	struct MaterialLightingBuffer
	{
		XMVECTOR CameraPosition = XMVectorZero(); // Camera position

		XMVECTOR ambientLightCol = XMVectorZero(); // 16 bytes
		XMVECTOR directionalLightCol = XMVectorZero(); // 16 bytes
		XMVECTOR directionLightDir = XMVectorZero(); // 16 bytes
		PointLight pointLights[MAX_POINT_LIGHTS] = {};  // Array of point lights
		// Material properties
		XMVECTOR diffuseColor = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // Diffuse color
		XMVECTOR specularColor = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // Specular color
		float emission = 0.0f; // Shininess
		float opacity = 0.0f; // Opacity
		float reflectionStrength = 0.0f; // Reflection strength
		float metallic = 0.0f; // Metallic factor
		float roughness = 0.0f; // Roughness factor
		float refractionIndex = 0.0f; // Refraction index

		float padding[2];                    // 8 bytes (align to 16 bytes)
	};

	XMVECTOR ambientLightColour;
	XMVECTOR directionalLightShinesFrom;
	XMVECTOR directionaLightColour;
	const wchar_t* windowName;// Potentially, have a function that sets this 
	HWND windowHandler;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* objectTransformBuffer;
	ID3D11Buffer* materialLightingBuffer;
	ID3D11Buffer* skyboxTransformBuffer;
	ID3D11SamplerState* sampler;
	ID3D11RasterizerState* rasterSolid;
	ID3D11RasterizerState* rasterSkybox;
	ID3D11DepthStencilState* depthWriteSolidState;
	ID3D11DepthStencilState* depthWriteSkyboxState;
	ID3D11DepthStencilState* depthZBufferState;
	ID3D11VertexShader* skyboxVertexShader;
	ID3D11PixelShader* skyboxPixelShader;
	ID3D11InputLayout* skyboxInputLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBuffer;
	ID3D11DepthStencilView* depthZBuffer;
	ID3D11BlendState* alphaBlendSateEnable;
	ID3D11BlendState* alphaBlendSateDisable;
	ID3D11ShaderReflection* vertexShaderReflection;
	TransformBuffer TransformData;
	SkyboxTransformBuffer SkyboxTransformData;
	MaterialLightingBuffer MaterialLightingData;
public:
	Renderer();
	PointLight pointLights[MAX_POINT_LIGHTS];
	void InitializeGraphics();
	HRESULT InitializeWindow(const HINSTANCE instance_Handle, const int Cmd_Show);
	HRESULT InitializeD3D();
	HRESULT LoadPixelShader(const wchar_t* const file_Name, ID3D11PixelShader** pixel_Shader);
	HRESULT LoadVertexShader(const wchar_t* file_Name, ID3D11VertexShader** vertex_Shader, ID3D11InputLayout** input_Layout);
	HRESULT InitializePipeline();
	void DrawSkyBox(Transform& transform, XMMATRIX& view, XMMATRIX& projection);
	void RenderFrame();
	ID3D11Device& GetDevice();
	ID3D11DeviceContext& GetDeviceContext();
	void RenderUI();
	HWND& GetWindowHandler();
	void Clean3D();
};
