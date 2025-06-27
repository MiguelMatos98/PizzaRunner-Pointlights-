#include "Renderer.h"
#include <Windows.h>
#include <DDSTextureLoader.h>
#include <dxgidebug.h>
#include <DirectXColors.h>
#include "WICTextureLoader.h"
#include "GameManager.h"
#include "ColliderManager.h"
#include "Material.h"
#include "SpriteFont.h"
#include <wrl/client.h>
#include <sstream>

Renderer::Renderer() : ambientLightColour{ 0.1f, 0.1f, 0.1f, 1.0f }, directionalLightShinesFrom{ 0.0f, 1.0f, 0.0f }, directionaLightColour{ 1.0f, 1.0f, 1.0f, 1.0f }, windowName{ nullptr }, windowHandler{ nullptr }, device{ nullptr }, deviceContext{ nullptr }, vertexBuffer{ nullptr }, indexBuffer{ nullptr }, objectTransformBuffer{ nullptr }, materialLightingBuffer{ nullptr }, skyboxTransformBuffer{ nullptr }, sampler{ nullptr }, rasterSolid{ nullptr }, rasterSkybox{ nullptr }, depthWriteSolidState{ nullptr }, depthWriteSkyboxState{ nullptr }, depthZBufferState{ nullptr }, skyboxVertexShader{ nullptr }, skyboxPixelShader{ nullptr }, skyboxInputLayout{ nullptr }, vertexShader{ nullptr }, pixelShader{ nullptr }, layout{ nullptr }, swapChain{ nullptr }, backBuffer{ nullptr }, depthZBuffer{ nullptr }, alphaBlendSateEnable{ nullptr }, alphaBlendSateDisable{ nullptr }, vertexShaderReflection{ nullptr }, TransformData{}, MaterialLightingData{}
{
}

void Renderer::InitializeGraphics()
{
	// ---------------------- Wooden Cube Vertices ----------------------
	//									 x	    y	   z			   r	 g	   b	 a				 u	   v			   Nx	  Ny     Nz
	Vertex vertices[] = {
		// Front face (normals pointing forward: { 0.0f, 0.0f, -1.0f })
		XMFLOAT3{-0.5f, -0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{ 0.0f,  0.0f, -1.0f}, // Front Bottom Left
		XMFLOAT3{ 0.5f, -0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{ 0.0f,  0.0f, -1.0f}, // Front Bottom Right
		XMFLOAT3{ 0.5f,  0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{ 0.0f,  0.0f, -1.0f}, // Front Top Right
		XMFLOAT3{-0.5f,  0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{ 0.0f,  0.0f, -1.0f}, // Front Top Left

		// Back face (normals pointing backward: { 0.0f, 0.0f, 1.0f })
		XMFLOAT3{-0.5f, -0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{ 0.0f,  0.0f,  1.0f}, // Back Bottom Left
		XMFLOAT3{ 0.5f, -0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{ 0.0f,  0.0f,  1.0f}, // Back Bottom Right
		XMFLOAT3{ 0.5f,  0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{ 0.0f,  0.0f,  1.0f}, // Back Top Right
		XMFLOAT3{-0.5f,  0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{ 0.0f,  0.0f,  1.0f}, // Back Top Left

		// Left face (normals pointing left: { -1.0f, 0.0f, 0.0f })
		XMFLOAT3{-0.5f, -0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{-1.0f,  0.0f,  0.0f}, // Left Bottom Front
		XMFLOAT3{-0.5f,  0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{-1.0f,  0.0f,  0.0f}, // Left Top Front
		XMFLOAT3{-0.5f,  0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{-1.0f,  0.0f,  0.0f}, // Left Top Back
		XMFLOAT3{-0.5f, -0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{-1.0f,  0.0f,  0.0f}, // Left Bottom Back

		// Right face (normals pointing right: { 1.0f, 0.0f, 0.0f })
		XMFLOAT3{ 0.5f, -0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{ 1.0f,  0.0f,  0.0f}, // Right Bottom Front
		XMFLOAT3{ 0.5f,  0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{ 1.0f,  0.0f,  0.0f}, // Right Top Front
		XMFLOAT3{ 0.5f,  0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{ 1.0f,  0.0f,  0.0f}, // Right Top Back
		XMFLOAT3{ 0.5f, -0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{ 1.0f,  0.0f,  0.0f}, // Right Bottom Back

		// Top face (normals pointing up: { 0.0f, 1.0f, 0.0f })
		XMFLOAT3{-0.5f,  0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{ 0.0f,  1.0f,  0.0f}, // Top Front Left
		XMFLOAT3{ 0.5f,  0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{ 0.0f,  1.0f,  0.0f}, // Top Front Right
		XMFLOAT3{ 0.5f,  0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{ 0.0f,  1.0f,  0.0f}, // Top Back Right
		XMFLOAT3{-0.5f,  0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{ 0.0f,  1.0f,  0.0f}, // Top Back Left

		// Bottom face (normals pointing down: { 0.0f, -1.0f, 0.0f })
		XMFLOAT3{-0.5f, -0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 1.0f}, XMFLOAT3{ 0.0f, -1.0f,  0.0f}, // Bottom Front Left
		XMFLOAT3{ 0.5f, -0.5f, -0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 1.0f}, XMFLOAT3{ 0.0f, -1.0f,  0.0f}, // Bottom Front Right
		XMFLOAT3{ 0.5f, -0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{1.0f, 0.0f}, XMFLOAT3{ 0.0f, -1.0f,  0.0f}, // Bottom Back Right
		XMFLOAT3{-0.5f, -0.5f,  0.5f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.0f}, XMFLOAT3{ 0.0f, -1.0f,  0.0f}, // Bottom Back Left
	};

	// Create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DYNAMIC; // Dynamic allows CPU-Write and GPU-read
	bd.ByteWidth = sizeof(vertices); // Alternativelly can also be this for simplicity
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use of vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow CPU to write in buffer 

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA initData = { };
	initData.pSysMem = vertices;

	if (FAILED(device->CreateBuffer(&bd, &initData, &vertexBuffer)))
	{
		OutputDebugString(L"Failed to create vertex buffer");
		return;
	}

	// Create an index buffer
	unsigned int indices[] =
	{
		// Front face (two triangles)
		0, 1, 2,
		0, 2, 3,

		// Back face (two triangles)
		4, 5, 6,
		4, 6, 7,

		// Left face (two triangles)
		4, 0, 1,
		4, 1, 5,

		// Right face (two triangles)
		3, 2, 6,
		3, 6, 7,

		// Top face (two triangles)
		1, 5, 6,
		1, 6, 2,

		// Bottom face (two triangles)
		0, 3, 7,
		0, 7, 4,
	}; // 36

	// Indices buffer for cube
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(indices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Define the resource data.
	initData.pSysMem = indices;

	if (FAILED(device->CreateBuffer(&bufferDesc, &initData, &indexBuffer)))
	{
		OutputDebugString(L"Failed to create index buffer");
		return;
	}

	D3D11_BUFFER_DESC  ConstantBufferDescription = {};
	ConstantBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	ConstantBufferDescription.ByteWidth = sizeof(TransformBuffer);
	ConstantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBufferDescription.CPUAccessFlags = 0;
	ConstantBufferDescription.StructureByteStride = 0;

	initData.pSysMem = &TransformData;

	HRESULT hr = device->CreateBuffer(&ConstantBufferDescription, &initData, &objectTransformBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create object transform buffer\n");
		return;
	}

	ConstantBufferDescription.ByteWidth = sizeof(SkyboxTransformBuffer);
	initData.pSysMem = &SkyboxTransformData;
	if (FAILED(device->CreateBuffer(&ConstantBufferDescription, &initData, &skyboxTransformBuffer)))
	{
		OutputDebugString(L"Failed to create Skybox constant buffer");
		return;
	}

	ConstantBufferDescription.ByteWidth = sizeof(MaterialLightingBuffer);
	initData.pSysMem = &MaterialLightingData;
	hr = device->CreateBuffer(&ConstantBufferDescription, &initData, &materialLightingBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create MaterialLightingBuffer\n");
		return;
	}

	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	hr = deviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	if (FAILED(hr))
	{
		OutputDebugString(L"Unable to map vertex");
		return;
	}
	memcpy(ms.pData, vertices, sizeof(vertices)); // Copy the data to the buffer

	deviceContext->Unmap(vertexBuffer, NULL);

	ID3D11ShaderResourceView* textureHandler{nullptr};
	for (size_t i = 0; i < GameManager::GetGameObjects().size(); i++)
	{
		auto levelObject = GameManager::GetGameObjects()[i];
		if (i < 140)
		{
			CreateWICTextureFromFile(device, deviceContext, L"PizzaBox.bmp", nullptr, &textureHandler);
			levelObject->GetMaterial().SetMaterialTexture(textureHandler);
		}
		else if(i == 140)
		{
			CreateWICTextureFromFile(device, deviceContext, L"roadTexture.bmp", nullptr, &textureHandler);
			levelObject->GetMaterial().SetMaterialTexture(textureHandler);
		}
		else
		{
			CreateWICTextureFromFile(device, deviceContext, L"TrollyTexture.bmp", nullptr, &textureHandler);
			levelObject->GetMaterial().SetMaterialTexture(textureHandler);
		}
	}

	GameObject* cameraObject = dynamic_cast<GameObject*>(GameManager::GetPlayer().GetCamera());
	CreateDDSTextureFromFile(device, deviceContext, L"NightSky.dds", nullptr, &textureHandler);
	cameraObject->GetMaterial().SetMaterialTexture(textureHandler);

}

HRESULT Renderer::InitializeWindow(const HINSTANCE instance_Handle, const int Cmd_Show)
{
	// First we need to register our window class.
	// This is how windows stores properties for windows that we want to create.
	WNDCLASSEX wc = {}; // "= {}" sets all values to 0. Can also be achieved with "ZeroMemory(&WC, sizeof(WNDCLASSEX))" macro 
	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = GameManager::WindowProc; // Our window procedure function, make sure it handles window creation or calls DefWindowProc(...)
	wc.hInstance = instance_Handle; // Give our app's handle 
	wc.lpszClassName = L"WindowClass1"; // Windows will store our window class under this name
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // Background Color of the win32 app (not needed for 3D3 apps)
	// Register class with above struct. If it fails, if block will execute
	if (!RegisterClassEx(&wc))
	{
		return E_FAIL; // Return fail code to be picked up by FAILED(...) macro
	}
	// Adjust the window dimensions so that the top windows bar is not taking pixels away from our app
	RECT wr = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
	GetClientRect(windowHandler, &wr); // Get the size of window
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	UINT width = wr.right - wr.left;
	UINT height = wr.bottom - wr.top;

	windowName = L"DirectX Hello World!";
	// Create the window and use the result as the handle
	windowHandler = CreateWindowEx(NULL,
		L"WindowClass1", // Name of our window class
		windowName, // Title of the window
		WS_OVERLAPPED | WS_MINIMIZE | WS_SYSMENU, // Window style with no resizing and maxamizing
		// WS_OVERLAPPEDWINDOW, // Alternative window style that allows resizing
		100, // x-position of the window
		100, // Y-position of the window
		width, // Width of the window
		height, // Height of the window
		NULL, // No parent, window, NULL
		NULL, // No menus, Null
		instance_Handle, // Application Handle
		NULL);		// Used with multiple windows, Null

	if (windowHandler == NULL) return E_FAIL;

	ShowWindow(windowHandler, Cmd_Show);

	return S_OK;
}

HRESULT Renderer::InitializeD3D()
{
	// Create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd = {};

	// Fill the swap chain description struct
	scd.BufferCount = 2; // Use at least two buffers for flip-model
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH; // Set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT; // Set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Intended swapchain use 
	scd.OutputWindow = windowHandler; // Window to use 
	scd.SampleDesc.Count = 1; // Number of samples for AA
	scd.Windowed = TRUE; // Windowed/full-screen mode
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // Use modern flip-model swap effect
	scd.Flags = 0; // Remove DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH for flip-model swap chains

	// Create a swap chain, device, and device context from the scd
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,                     // Use default graphics adapter
		D3D_DRIVER_TYPE_HARDWARE, // Use hardware acceleration
		NULL,                     // No software driver
		D3D11_CREATE_DEVICE_DEBUG, // Enable debug layer (if applicable)
		NULL,                     // No specific feature levels requested
		0,                        // Array size of feature levels
		D3D11_SDK_VERSION,        // Always use D3D11_SDK_VERSION
		&scd,                     // Pointer to the swap chain description
		&swapChain,               // Swap chain output
		&device,                  // Device output
		NULL,                     // Feature level output (optional)
		&deviceContext            // Device context output
	);

	if (FAILED(hr)) return hr;

#ifdef _DEBUG
	{
		ID3D11InfoQueue* info;
		device->QueryInterface(IID_PPV_ARGS(&info));
		info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
		info->Release();
	}

	HMODULE dxgidebug = LoadLibraryA("dxgidebug.dll");
	if (dxgidebug != NULL) {
		HRESULT(WINAPI * dxgiGetDebugInterface)(REFIID riid, void** ppDebug);
		*(FARPROC*)&dxgiGetDebugInterface = GetProcAddress(dxgidebug, "DXGIGetDebugInterface");

		IDXGIInfoQueue* dxgiinfo;
		dxgiGetDebugInterface(IID_PPV_ARGS(&dxgiinfo));
		dxgiinfo->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		dxgiinfo->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
		dxgiinfo->Release();
	}
#endif

	// Get the address of the back buffer
	ID3D11Texture2D* pBackBufferTexture = nullptr;

	// Get the back buffer from the swap chain
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTexture);

	if (FAILED(hr)) return hr;

	// Use the back buffer address to create the render target view
	hr = device->CreateRenderTargetView(pBackBufferTexture, NULL, &backBuffer);

	if (FAILED(hr)) return hr;

	pBackBufferTexture->Release(); // This object is no longer needed 

	// ----------------------------- Depth Buffer Creation -----------------------------------
	D3D11_TEXTURE2D_DESC tex2dDesc = {};
	tex2dDesc.Width = SCREEN_WIDTH;
	tex2dDesc.Height = SCREEN_HEIGHT;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D32_FLOAT;
	tex2dDesc.SampleDesc.Count = scd.SampleDesc.Count; // Same sample count as swap chain
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D* zBufferTexture;
	hr = device->CreateTexture2D(&tex2dDesc, NULL, &zBufferTexture);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create Z-Buffer texture!");
		return E_FAIL;
	}

	////  Create the depth stencill desc
	D3D11_DEPTH_STENCIL_DESC dsSateDesc;
	ZeroMemory(&dsSateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsSateDesc.DepthEnable = TRUE;
	dsSateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsSateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsSateDesc.StencilEnable = TRUE;
	dsSateDesc.StencilReadMask = 0xff;
	dsSateDesc.StencilWriteMask = 0xff;
	dsSateDesc.FrontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	dsSateDesc.BackFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_DECR, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	hr = device->CreateDepthStencilState(&dsSateDesc, &depthZBufferState);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create depth stencil state!");
		return E_FAIL;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = device->CreateDepthStencilView(zBufferTexture, &dsvDesc, &depthZBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create depth stencil view!");
		return E_FAIL;
	}

	zBufferTexture->Release();

	// Sampler's description
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create Sampler!");
		return E_FAIL;
	}

	D3D11_BLEND_DESC bd1 = { 0 };
	bd1.RenderTarget[0].BlendEnable = TRUE;
	bd1.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd1.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd1.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd1.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd1.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd1.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd1.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd1.IndependentBlendEnable = FALSE;
	bd1.AlphaToCoverageEnable = FALSE;
	hr = device->CreateBlendState(&bd1, &alphaBlendSateEnable);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create blend state 1!");
		return E_FAIL;
	}

	D3D11_BLEND_DESC bd2 = { 0 };
	bd2.RenderTarget[0].BlendEnable = FALSE;
	bd2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd2.IndependentBlendEnable = FALSE;
	bd2.AlphaToCoverageEnable = FALSE;
	hr = device->CreateBlendState(&bd2, &alphaBlendSateDisable);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create blend state 2!");
		return E_FAIL;
	}

	// Rasterizer desc description
	D3D11_RASTERIZER_DESC rsDesc;
	// Back-Face Culling
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	hr = device->CreateRasterizerState(&rsDesc, &rasterSolid);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create a solid rastarizer state!");
		return E_FAIL;
	}
	// Front-face culling
	rsDesc.CullMode = D3D11_CULL_FRONT;
	device->CreateRasterizerState(&rsDesc, &rasterSkybox);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create a  rastarizer state!");
		return E_FAIL;
	}

	// Depth writting enabled 
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = device->CreateDepthStencilState(&dsDesc, &depthWriteSolidState);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create a solid dept stencil!");
		return E_FAIL;
	}

	// Depth writing disable
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = device->CreateDepthStencilState(&dsDesc, &depthWriteSkyboxState);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create a skybox dept stencil!");
		return E_FAIL;
	}

	// Define and Set viewport
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<FLOAT>(SCREEN_WIDTH);
	viewport.Height = static_cast<FLOAT>(SCREEN_HEIGHT);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);

	InitializePipeline();

	return S_OK;
}

HRESULT Renderer::LoadPixelShader(const wchar_t* const file_Name, ID3D11PixelShader** pixel_Shader)
{
	HRESULT result;

	auto pixelShadersBytecode = DX::ReadData(file_Name);

	result = device->CreatePixelShader(pixelShadersBytecode.data(), pixelShadersBytecode.size(), NULL, pixel_Shader);
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create pixel shader");
		return result;
	}

	return S_OK;
}

HRESULT Renderer::LoadVertexShader(const wchar_t* const file_Name, ID3D11VertexShader** vertex_Shader, ID3D11InputLayout** input_Layout)
{
	HRESULT result;

	// Read compiled shader bytecode
	auto vertexShaderBytecode = DX::ReadData(file_Name);

	// Create the vertex shader
	result = device->CreateVertexShader(vertexShaderBytecode.data(), vertexShaderBytecode.size(), NULL, vertex_Shader);
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create vertex shader");
		return result;
	}

	// Reflect the shader bytecode
	result = D3DReflect(vertexShaderBytecode.data(), vertexShaderBytecode.size(), IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection);
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create shader reflection");
		return result;
	}

	// Get shader description
	D3D11_SHADER_DESC shaderDescription;
	vertexShaderReflection->GetDesc(&shaderDescription);

	vector<D3D11_SIGNATURE_PARAMETER_DESC> signatureParamDescriptions(shaderDescription.InputParameters);
	for (UINT i = 0; i < shaderDescription.InputParameters; i++)
	{
		vertexShaderReflection->GetInputParameterDesc(i, &signatureParamDescriptions[i]);
	}

	// Create input layout based on reflection data
	vector<D3D11_INPUT_ELEMENT_DESC> ied(shaderDescription.InputParameters);
	for (size_t i = 0; i < shaderDescription.InputParameters; i++)
	{
		ied[i].SemanticName = signatureParamDescriptions[i].SemanticName;
		ied[i].SemanticIndex = signatureParamDescriptions[i].SemanticIndex;

		// Handling component types based on mask
		if (signatureParamDescriptions[i].ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
		{
			switch (signatureParamDescriptions[i].Mask)
			{
			case 1:
				ied[i].Format = DXGI_FORMAT_R32_FLOAT; // float1
				break;
			case 3:
				ied[i].Format = DXGI_FORMAT_R32G32_FLOAT; // float2
				break;
			case 7:
				ied[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; // float3
				break;
			case 15:
				ied[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // float4
				break;
			case 31:
				ied[i].Format = DXGI_FORMAT_B8G8R8A8_UNORM; // float4
				break;
			default:
				ied[i].Format = DXGI_FORMAT_UNKNOWN; // Handle invalid cases
				break;
			}
		}

		// Set input slot details
		ied[i].InputSlot = 0;
		ied[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		ied[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[i].InstanceDataStepRate = 0; // Not using instanced data
	}

	// Create input layout using the described attributes
	result = device->CreateInputLayout(ied.data(), shaderDescription.InputParameters, vertexShaderBytecode.data(), vertexShaderBytecode.size(), input_Layout);
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create input layout");
		return result;
	}

	return S_OK;
}

HRESULT Renderer::InitializePipeline()
{
	LoadVertexShader(L"Compiled Shaders/SkyboxVShader.cso", &skyboxVertexShader, &skyboxInputLayout); // change this to new pVs
	LoadPixelShader(L"Compiled Shaders/SkyboxPShader.cso", &skyboxPixelShader); // change this to new pPs 

	LoadVertexShader(L"Compiled Shaders/RefletiveVShader.cso", &vertexShader, &layout);
	LoadPixelShader(L"Compiled Shaders/RefletivePShader.cso", &pixelShader);

	return S_OK;
}

void Renderer::DrawSkyBox(Transform& transform, XMMATRIX& view, XMMATRIX& projection)
{
	// Enable alpha blending for any skybox transparency effects
	deviceContext->OMSetBlendState(alphaBlendSateEnable, nullptr, 0xffffffff);

	// Set depth stencil state to allow the skybox to render behind other objects
	deviceContext->OMSetDepthStencilState(depthWriteSkyboxState, 1);

	// Set rasterizer state for front-face culling
	deviceContext->RSSetState(rasterSkybox);

	// Set skybox shaders
	deviceContext->VSSetShader(skyboxVertexShader, nullptr, 0);
	deviceContext->PSSetShader(skyboxPixelShader, nullptr, 0);
	deviceContext->IASetInputLayout(skyboxInputLayout);

	ID3D11ShaderResourceView* skyboxTexture{ nullptr };
	GameObject* cameraTemp = dynamic_cast<GameObject*>(GameManager::GetPlayer().GetCamera());
	skyboxTexture = cameraTemp->GetMaterial().GetMaterialTexture();
	// Bind skybox texture and sampler
	deviceContext->PSSetShaderResources(2, 1, &skyboxTexture); // Texture at t2
	deviceContext->PSSetSamplers(1, 1, &sampler);                      // Sampler at s1

	TransformData.WVP = transform.GetWorldMatrix() * view * projection;

	// Update the transform constant buffer for the skybox
	deviceContext->UpdateSubresource(skyboxTransformBuffer, 0, nullptr, &TransformData, 0, 0);

	// Bind the transform constant buffer to the vertex shader
	deviceContext->VSSetConstantBuffers(1, 1, &skyboxTransformBuffer);

	// Draw the skybox model
	//Camera* cameraTemp = GameManager::GetPlayer().GetCamera();
	GameManager::GetPlayer().GetCamera()->GetSkyBoxModel().Draw();
}

void Renderer::RenderFrame()
{
	XMMATRIX view = GameManager::GetPlayer().GetCamera()->GetViewMatrix();
	XMMATRIX projection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
		0.1f, 150
	);

	// Clear back buffer and depth buffer
	deviceContext->ClearRenderTargetView(backBuffer, DirectX::Colors::Black);
	deviceContext->ClearDepthStencilView(depthZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set the back buffer as the current render target
	deviceContext->OMSetRenderTargets(1, &backBuffer, depthZBuffer);

	// Set primitive topology
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Render Skybox
	DrawSkyBox(GameManager::GetPlayer().GetCamera()->GetSkyboxTransform(), view, projection);

	// Prepare to render game objects
	deviceContext->IASetInputLayout(layout);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	deviceContext->RSSetState(rasterSolid);
	deviceContext->OMSetBlendState(alphaBlendSateDisable, 0, 0xffffffff);
	deviceContext->OMSetDepthStencilState(depthWriteSolidState, 1);

	const auto& gameObjects = GameManager::GetGameObjects();

	GameObject* cameraTemp = dynamic_cast<GameObject*>(GameManager::GetPlayer().GetCamera());

	for (size_t i = 1; i < gameObjects.size(); ++i) // Skip the first object (skybox)
	{
		auto& gameObject = gameObjects[i];
		Transform currentTransform = gameObject->GetTransform();
		Material* material = &gameObject->GetMaterial();

		if (i < 140)
			material->SetMaterialTexture(gameObject->GetMaterial().GetMaterialTexture());
		else if (i == 140)
			material->SetMaterialTexture(gameObject->GetMaterial().GetMaterialTexture());
		else
			material->SetMaterialTexture(gameObject->GetMaterial().GetMaterialTexture());

		ID3D11ShaderResourceView* currentTexture = material->GetMaterialTexture();
		if (currentTexture)
		{
			deviceContext->PSSetShaderResources(1, 1, &currentTexture);
			currentTexture = cameraTemp->GetMaterial().GetMaterialTexture();
			deviceContext->PSSetShaderResources(0, 1, &currentTexture);
		}
		material->SetMaterialSamplerState(sampler);
	// Ensure the reflective shader sampler is bound to slot 0
		deviceContext->PSSetSamplers(0, 1, &sampler); // Sampler for reflective shader

		// Update lighting and material properties
		MaterialLightingData.ambientLightCol = ambientLightColour;
		MaterialLightingData.directionalLightCol = directionaLightColour;
		MaterialLightingData.directionLightDir = XMVector3Normalize(directionalLightShinesFrom);

		for (size_t j = 0; j < MAX_POINT_LIGHTS; ++j)
		{
			MaterialLightingData.pointLights[j].position = pointLights[j].position;  // If pointLights[j].position is XMFLOAT3
			MaterialLightingData.pointLights[j].strength = pointLights[j].strength;
			MaterialLightingData.pointLights[j].colour = pointLights[j].colour;
			MaterialLightingData.pointLights[j].padding = 0.f; // always zero
		}

		// Update material properties
		MaterialLightingData.diffuseColor = material->GetDiffuseColor();
		MaterialLightingData.specularColor = material->GetSpecularColor();
		MaterialLightingData.emission = material->GetShininess();
		MaterialLightingData.opacity = material->GetOpacity();
		MaterialLightingData.reflectionStrength = material->GetReflectionStrength();
		MaterialLightingData.metallic = material->GetMetallic();
		MaterialLightingData.roughness = material->GetRoughness();
		MaterialLightingData.refractionIndex = material->GetRefractionIndex();
		MaterialLightingData.CameraPosition = GameManager::GetPlayer().GetCamera()->GetTransform().position;
		// Update material lighting buffer
		//deviceContext->PSSetSamplers(0, 1, &sampler); // Sampler for reflective shader
		deviceContext->UpdateSubresource(materialLightingBuffer, 0, nullptr, &MaterialLightingData, 0, 0);
		deviceContext->PSSetConstantBuffers(1, 1, &materialLightingBuffer);

		// Update buffers and draw
		TransformData.WVP = currentTransform.GetWorldMatrix() * view * projection;
		TransformData.WorldMatrix = currentTransform.GetWorldMatrix();

		deviceContext->UpdateSubresource(objectTransformBuffer, 0, nullptr, &TransformData, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &objectTransformBuffer);

		gameObject->GetModel()->Draw();
	}
	// Render UI elements
	RenderUI();

	// Present the frame
	swapChain->Present(0, 0);
}

void Renderer::RenderUI()
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_iconTexture;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch = std::make_unique<SpriteBatch>(deviceContext);
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont = std::make_unique<SpriteFont>(device, L"myfile.spritefont");

	HRESULT hr = CreateWICTextureFromFile(device, deviceContext, L"PizzaBoxUI.png", NULL, &m_iconTexture);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to load icon texture!\n");
		return;
	}

	DirectX::SimpleMath::Vector2 iconPos(10.0f, 20.0f);
	DirectX::SimpleMath::Vector2 fontPos(110, 35);
	std::wstring scoreText = L"X" + std::to_wstring(GameManager::GetPlayer().GetScore());

	m_spriteBatch->Begin();
	m_spriteBatch->Draw(m_iconTexture.Get(), iconPos);
	m_spriteFont->DrawString(m_spriteBatch.get(), scoreText.c_str(), fontPos, DirectX::Colors::BlanchedAlmond);
	m_spriteBatch->End();
}

ID3D11Device& Renderer::GetDevice()
{
	return *device;
}

ID3D11DeviceContext& Renderer::GetDeviceContext()
{
	return *deviceContext;
}

HWND& Renderer::GetWindowHandler()
{
	return windowHandler;
}

void Renderer::Clean3D()
{
	if (sampler) { sampler->Release(); sampler = nullptr; }
	if (depthZBuffer) { depthZBuffer->Release(); depthZBuffer = nullptr; }
	if (vertexBuffer) { vertexBuffer->Release(); vertexBuffer = nullptr; }
	if (indexBuffer) { indexBuffer->Release(); indexBuffer = nullptr; }
	if (vertexShader) { vertexShader->Release(); vertexShader = nullptr; }
	if (pixelShader) { pixelShader->Release(); pixelShader = nullptr; }
	if (layout) { layout->Release(); layout = nullptr; }
	if (depthZBufferState) { depthZBufferState->Release(); depthZBufferState = nullptr; }
	if (swapChain) { swapChain->Release(); swapChain = nullptr; }
	if (backBuffer) { backBuffer->Release(); backBuffer = nullptr; }
	if (windowHandler) { windowHandler = nullptr; }
	if (windowName) { windowName = nullptr; }
	if (vertexShaderReflection) { vertexShaderReflection->Release(); vertexShaderReflection = nullptr; }
	if (rasterSolid) { rasterSolid->Release(); rasterSolid = nullptr; }
	if (rasterSkybox) { rasterSkybox->Release(); rasterSkybox = nullptr; }
	if (depthWriteSolidState) { depthWriteSolidState->Release(); depthWriteSolidState = nullptr; }
	if (depthWriteSkyboxState) { depthWriteSkyboxState->Release(); depthWriteSkyboxState = nullptr; }
	if (objectTransformBuffer) { objectTransformBuffer->Release(); objectTransformBuffer = nullptr; }
	if (materialLightingBuffer) { materialLightingBuffer->Release(); materialLightingBuffer = nullptr; }
	if (skyboxTransformBuffer) { skyboxTransformBuffer->Release(); skyboxTransformBuffer = nullptr; }
	if (skyboxVertexShader) { skyboxVertexShader->Release(); skyboxVertexShader = nullptr; }
	if (skyboxPixelShader) { skyboxPixelShader->Release(); skyboxPixelShader = nullptr; }
	if (skyboxInputLayout) { skyboxInputLayout->Release(); skyboxInputLayout = nullptr; }
	if (alphaBlendSateDisable) { alphaBlendSateDisable->Release(); alphaBlendSateDisable = nullptr; }
	if (alphaBlendSateEnable) { alphaBlendSateEnable->Release(); alphaBlendSateEnable = nullptr;}
	if (device) { device->Release(); device = nullptr; }
	if (deviceContext) { deviceContext->Release(); deviceContext = nullptr; }
}
