#include "GraphicsDevice-Win32.h"

#include "Graphics\MeshData.h"
#include "Graphics\MeshManager.h"

#include "Input\InputManager.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <cstdio>


const LONG g_WindowWidth = 1280;
const LONG g_WindowHeight = 720;
LPCSTR g_WindowClassName = "DirectXWindowClass";
LPCSTR g_WindowName = "DirectXTemplate";
HWND g_WindowHandle = 0;

const BOOL g_EnableVSync = FALSE;

// Demo parameters.
DirectX::XMMATRIX g_WorldMatrix;
DirectX::XMMATRIX g_ViewMatrix;
DirectX::XMMATRIX g_ProjectionMatrix;

// Vertex data for a colored cube.
struct VertexPosColor
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
};

VertexPosColor g_Vertices[8] =
{
	{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) }, // 0
	{ DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 1
	{ DirectX::XMFLOAT3(1.0f,  1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 2
	{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 3
	{ DirectX::XMFLOAT3(-1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 4
	{ DirectX::XMFLOAT3(-1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 5
	{ DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 6
	{ DirectX::XMFLOAT3(1.0f, -1.0f,  1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) }  // 7
};

WORD g_Indicies[36] =
{
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7
};

template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != NULL)
	{
		ptr->Release();
		ptr = NULL;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

float batX = 0.0f;
float batY = 0.0f;
float batZ = 0.0f;


namespace CE
{
	bool GraphicsDeviceWin32::Initialize(HINSTANCE hInstance, int cmdShow)
	{
		// Check for DirectX Math library support.
		if (!DirectX::XMVerifyCPUSupport())
		{
			MessageBox(nullptr, TEXT("Failed to verify DirectX Math library support."), TEXT("Error"), MB_OK);
			return false;
		}

		// Initialize our application.
		if (InitializeApplication(hInstance, cmdShow) != 0)
		{
			MessageBox(nullptr, TEXT("Failed to create application window."), TEXT("Error"), MB_OK);
			return false;
		}

		if (InitializeDirectX(hInstance, g_EnableVSync) != 0)
		{
			MessageBox(nullptr, TEXT("Failed to create DirectX device and swap chain."), TEXT("Error"), MB_OK);
			return false;
		}

		if (!LoadContent())
		{
			MessageBox(nullptr, TEXT("Failed to load content."), TEXT("Error"), MB_OK);
			return false;
		}

		return true;
	}

	void GraphicsDeviceWin32::Destroy()
	{
		UnloadContent();
		Cleanup();
	}

	void GraphicsDeviceWin32::Update(float deltaTime)
	{
		DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0, 0, -100, 1);
		DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0, 0, 0, 1);
		DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
		g_ViewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
		g_d3dDeviceContext->UpdateSubresource(g_d3dConstantBuffers[ConstantBuffer::CB_Frame], 0, nullptr, &g_ViewMatrix, 0, 0);

		if (InputManager::Get().GetKey('W'))
		{
			printf("move bitch");
			batX += 0.01f;
		}

		g_WorldMatrix = DirectX::XMMatrixTranslation(batX, batY, batZ);
		g_d3dDeviceContext->UpdateSubresource(g_d3dConstantBuffers[ConstantBuffer::CB_Object], 0, nullptr, &g_WorldMatrix, 0, 0);
	}

	void GraphicsDeviceWin32::Render()
	{
		assert(g_d3dDevice);
		assert(g_d3dDeviceContext);

		Clear(DirectX::Colors::Salmon, 1.0f, 0);


		const UINT vertexStride = sizeof(VertexPosColor);
		const UINT offset = 0;

		g_d3dDeviceContext->IASetVertexBuffers(0, 1, &g_d3dVertexBuffer, &vertexStride, &offset);
		g_d3dDeviceContext->IASetInputLayout(g_d3dInputLayout);
		g_d3dDeviceContext->IASetIndexBuffer(g_d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		g_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		g_d3dDeviceContext->VSSetShader(g_d3dVertexShader, nullptr, 0);
		g_d3dDeviceContext->VSSetConstantBuffers(0, 3, g_d3dConstantBuffers);


		g_d3dDeviceContext->RSSetState(g_d3dRasterizerState);
		g_d3dDeviceContext->RSSetViewports(1, &g_Viewport);


		g_d3dDeviceContext->PSSetShader(g_d3dPixelShader, nullptr, 0);


		g_d3dDeviceContext->OMSetRenderTargets(1, &g_d3dRenderTargetView, g_d3dDepthStencilView);
		g_d3dDeviceContext->OMSetDepthStencilState(g_d3dDepthStencilState, 1);

		// "I'm Batman."
		MeshData* meshData = MeshManager::Get().GetMeshData("..\\..\\Assets\\bat.fbx");

		g_d3dDeviceContext->DrawIndexed((UINT)meshData->m_indices.size(), 0, 0);


		Present(g_EnableVSync);
	}

	// Initialize the application window.
	int GraphicsDeviceWin32::InitializeApplication(HINSTANCE hInstance, int cmdShow)
	{
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = &WndProc;
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndClass.lpszMenuName = nullptr;
		wndClass.lpszClassName = g_WindowClassName;

		if (!RegisterClassEx(&wndClass))
		{
			return -1;
		}

		RECT windowRect = { 0, 0, g_WindowWidth, g_WindowHeight };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		g_WindowHandle = CreateWindowA(g_WindowClassName, g_WindowName,
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr, nullptr, hInstance, nullptr);

		if (!g_WindowHandle)
		{
			return -1;
		}

		ShowWindow(g_WindowHandle, cmdShow);
		UpdateWindow(g_WindowHandle);

		return 0;
	}

	// Initialize the DirectX device and swap chain.
	int GraphicsDeviceWin32::InitializeDirectX(HINSTANCE hInstance, BOOL vSync)
	{
		UNREFERENCED_PARAMETER(hInstance);

		// A window handle must have been created already.
		assert(g_WindowHandle != 0);

		RECT clientRect;
		GetClientRect(g_WindowHandle, &clientRect);

		// Compute the exact client dimensions. This will be used
		// to initialize the render targets for our swap chain.
		unsigned int clientWidth = clientRect.right - clientRect.left;
		unsigned int clientHeight = clientRect.bottom - clientRect.top;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = clientWidth;
		swapChainDesc.BufferDesc.Height = clientHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate = QueryRefreshRate(clientWidth, clientHeight, vSync);
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = g_WindowHandle;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Windowed = TRUE;


		UINT createDeviceFlags = 0;
#if _DEBUG
		createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		// These are the feature levels that we will accept.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		// This will be the feature level that 
		// is used to create our device and swap chain.
		D3D_FEATURE_LEVEL featureLevel;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION, &swapChainDesc, &g_d3dSwapChain, &g_d3dDevice, &featureLevel,
			&g_d3dDeviceContext);

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
				nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
				D3D11_SDK_VERSION, &swapChainDesc, &g_d3dSwapChain, &g_d3dDevice, &featureLevel,
				&g_d3dDeviceContext);
		}

		if (FAILED(hr))
		{
			return -1;
		}


		// Next initialize the back buffer of the swap chain and associate it to a 
		// render target view.
		ID3D11Texture2D* backBuffer;
		hr = g_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr))
		{
			return -1;
		}

		hr = g_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &g_d3dRenderTargetView);
		if (FAILED(hr))
		{
			return -1;
		}

		SafeRelease(backBuffer);


		// Create the depth buffer for use with the depth/stencil view.
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDesc.Width = clientWidth;
		depthStencilBufferDesc.Height = clientHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		hr = g_d3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &g_d3dDepthStencilBuffer);
		if (FAILED(hr))
		{
			return -1;
		}


		hr = g_d3dDevice->CreateDepthStencilView(g_d3dDepthStencilBuffer, nullptr, &g_d3dDepthStencilView);
		if (FAILED(hr))
		{
			return -1;
		}


		// Setup depth/stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilStateDesc.StencilEnable = FALSE;

		hr = g_d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &g_d3dDepthStencilState);


		// Setup rasterizer state.
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state object.
		hr = g_d3dDevice->CreateRasterizerState(&rasterizerDesc, &g_d3dRasterizerState);
		if (FAILED(hr))
		{
			return -1;
		}


		// Initialize the viewport to occupy the entire client area.
		g_Viewport.Width = static_cast<float>(clientWidth);
		g_Viewport.Height = static_cast<float>(clientHeight);
		g_Viewport.TopLeftX = 0.0f;
		g_Viewport.TopLeftY = 0.0f;
		g_Viewport.MinDepth = 0.0f;
		g_Viewport.MaxDepth = 1.0f;


		return 0;
	}

	//The QueryRefreshRate function is used to query the ideal refresh rate
	//given the specified screen dimensions. If the vSync flag is FALSE then
	//this function simply returns 0/1 which indicates that the screen should
	//be refreshed as quickly as possible without waiting for a vertical sync.
	//You can see the implementation of the QueryRefreshRate function by
	//(downloading the demo available at the end of this article)TM.
	DXGI_RATIONAL GraphicsDeviceWin32::QueryRefreshRate(
		unsigned int clientWidth,
		unsigned int clientHeight,
		BOOL vSync)
	{
		UNREFERENCED_PARAMETER(clientWidth);
		UNREFERENCED_PARAMETER(clientHeight);
		UNREFERENCED_PARAMETER(vSync);

		DXGI_RATIONAL retVal;
		retVal.Numerator = 1;
		retVal.Denominator = 1;
		return retVal;
	}

	bool GraphicsDeviceWin32::LoadContent()
	{
		assert(g_d3dDevice);

		// "I'm Batman."
		MeshData* meshData = MeshManager::Get().GetMeshData("..\\..\\Assets\\bat.fbx");

		// Create an initialize the vertex buffer.
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof(MeshData::Vertex) * (UINT)meshData->m_vertices.size();
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

		resourceData.pSysMem = meshData->m_vertices.data();

		HRESULT hr = g_d3dDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &g_d3dVertexBuffer);
		if (FAILED(hr))
		{
			return false;
		}


		// Create and initialize the index buffer.
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.ByteWidth = sizeof(WORD) * (UINT)meshData->m_indices.size();
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		resourceData.pSysMem = meshData->m_indices.data();

		hr = g_d3dDevice->CreateBuffer(&indexBufferDesc, &resourceData, &g_d3dIndexBuffer);
		if (FAILED(hr))
		{
			return false;
		}


		// Create the constant buffers for the variables defined in the vertex shader.
		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
		constantBufferDesc.CPUAccessFlags = 0;
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		hr = g_d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[ConstantBuffer::CB_Application]);
		if (FAILED(hr))
		{
			return false;
		}
		hr = g_d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[ConstantBuffer::CB_Frame]);
		if (FAILED(hr))
		{
			return false;
		}
		hr = g_d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[ConstantBuffer::CB_Object]);
		if (FAILED(hr))
		{
			return false;
		}


		// Load the shaders at runtime.
		//g_d3dVertexShader = LoadShader<ID3D11VertexShader>(L"../data/shaders/SimpleVertexShader.hlsl", "SimpleVertexShader", "latest");
		//g_d3dPixelShader = LoadShader<ID3D11PixelShader>(L"../data/shaders/SimplePixelShader.hlsl", "SimplePixelShader", "latest");


		// Load the compiled vertex shader.
		ID3DBlob* vertexShaderBlob;
		LPCWSTR compiledVertexShaderObject = L"SimpleVertexShader.cso";

		hr = D3DReadFileToBlob(compiledVertexShaderObject, &vertexShaderBlob);
		if (FAILED(hr))
		{
			return false;
		}

		hr = g_d3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &g_d3dVertexShader);
		if (FAILED(hr))
		{
			return false;
		}


		// Load shader from byte array.
		//hr = g_d3dDevice->CreateVertexShader(g_SimpleVertexShader, sizeof(g_SimpleVertexShader), nullptr, &g_d3dVertexShader);
		//if (FAILED(hr))
		//{
		//	return false;
		//}

		//hr = g_d3dDevice->CreatePixelShader(g_SimplePixelShader, sizeof(g_SimplePixelShader), nullptr, &g_d3dPixelShader);
		//if (FAILED(hr))
		//{
		//	return false;
		//}


		// Create the input layout for the vertex shader.
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = g_d3dDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &g_d3dInputLayout);
		if (FAILED(hr))
		{
			return false;
		}

		SafeRelease(vertexShaderBlob);


		// Load the compiled pixel shader.
		ID3DBlob* pixelShaderBlob;
		LPCWSTR compiledPixelShaderObject = L"SimplePixelShader.cso";

		hr = D3DReadFileToBlob(compiledPixelShaderObject, &pixelShaderBlob);
		if (FAILED(hr))
		{
			return false;
		}

		hr = g_d3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &g_d3dPixelShader);
		if (FAILED(hr))
		{
			return false;
		}

		SafeRelease(pixelShaderBlob);


		// Setup the projection matrix.
		RECT clientRect;
		GetClientRect(g_WindowHandle, &clientRect);

		// Compute the exact client dimensions.
		// This is required for a correct projection matrix.
		float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
		float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

		g_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), clientWidth / clientHeight, 0.1f, 100.0f);

		g_d3dDeviceContext->UpdateSubresource(g_d3dConstantBuffers[ConstantBuffer::CB_Application], 0, nullptr, &g_ProjectionMatrix, 0, 0);

		return true;
	}

	template<>
	std::string GraphicsDeviceWin32::GetLatestProfile<ID3D11VertexShader>()
	{
		assert(g_d3dDevice);

		// Query the current feature level.
		D3D_FEATURE_LEVEL featureLevel = g_d3dDevice->GetFeatureLevel();

		switch (featureLevel)
		{
			case D3D_FEATURE_LEVEL_11_1:
			case D3D_FEATURE_LEVEL_11_0:
				return "vs_5_0";
			case D3D_FEATURE_LEVEL_10_1:
				return "vs_4_1";
			case D3D_FEATURE_LEVEL_10_0:
				return "vs_4_0";
			case D3D_FEATURE_LEVEL_9_3:
				return "vs_4_0_level_9_3";
			case D3D_FEATURE_LEVEL_9_2:
			case D3D_FEATURE_LEVEL_9_1:
				return "vs_4_0_level_9_1";
		}

		return "";
	}

	template<>
	std::string GraphicsDeviceWin32::GetLatestProfile<ID3D11PixelShader>()
	{
		assert(g_d3dDevice);

		// Query the current feature level.
		D3D_FEATURE_LEVEL featureLevel = g_d3dDevice->GetFeatureLevel();
		switch (featureLevel)
		{
			case D3D_FEATURE_LEVEL_11_1:
			case D3D_FEATURE_LEVEL_11_0:
				return "ps_5_0";
			case D3D_FEATURE_LEVEL_10_1:
				return "ps_4_1";
			case D3D_FEATURE_LEVEL_10_0:
				return "ps_4_0";
			case D3D_FEATURE_LEVEL_9_3:
				return "ps_4_0_level_9_3";
			case D3D_FEATURE_LEVEL_9_2:
			case D3D_FEATURE_LEVEL_9_1:
				return "ps_4_0_level_9_1";
		}

		return "";
	}

	template<>
	ID3D11VertexShader* GraphicsDeviceWin32::CreateShader<ID3D11VertexShader>(
		ID3DBlob* pShaderBlob,
		ID3D11ClassLinkage* pClassLinkage)
	{
		assert(g_d3dDevice);
		assert(pShaderBlob);

		ID3D11VertexShader* pVertexShader = nullptr;
		g_d3dDevice->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &pVertexShader);

		return pVertexShader;
	}

	template<>
	ID3D11PixelShader* GraphicsDeviceWin32::CreateShader<ID3D11PixelShader>(
		ID3DBlob* pShaderBlob,
		ID3D11ClassLinkage* pClassLinkage)
	{
		assert(g_d3dDevice);
		assert(pShaderBlob);

		ID3D11PixelShader* pPixelShader = nullptr;
		g_d3dDevice->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pClassLinkage, &pPixelShader);

		return pPixelShader;
	}

	template<class ShaderType>
	ShaderType* GraphicsDeviceWin32::LoadShader(
		const std::wstring& fileName,
		const std::string& entryPoint,
		const std::string& _profile)
	{
		ID3DBlob* pShaderBlob = nullptr;
		ID3DBlob* pErrorBlob = nullptr;
		ShaderType* pShader = nullptr;

		std::string profile = _profile;
		if (profile == "latest")
		{
			profile = GetLatestProfile<ShaderType>();
		}

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		HRESULT hr = D3DCompileFromFile(fileName.c_str(), nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), profile.c_str(),
			flags, 0, &pShaderBlob, &pErrorBlob);

		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				std::string errorMessage = (char*)pErrorBlob->GetBufferPointer();
				OutputDebugStringA(errorMessage.c_str());

				SafeRelease(pShaderBlob);
				SafeRelease(pErrorBlob);
			}

			return false;
		}

		pShader = CreateShader<ShaderType>(pShaderBlob, nullptr);

		SafeRelease(pShaderBlob);
		SafeRelease(pErrorBlob);

		return pShader;
	}

	void GraphicsDeviceWin32::UnloadContent()
	{
		SafeRelease(g_d3dConstantBuffers[ConstantBuffer::CB_Application]);
		SafeRelease(g_d3dConstantBuffers[ConstantBuffer::CB_Frame]);
		SafeRelease(g_d3dConstantBuffers[ConstantBuffer::CB_Object]);
		SafeRelease(g_d3dIndexBuffer);
		SafeRelease(g_d3dVertexBuffer);
		SafeRelease(g_d3dInputLayout);
		SafeRelease(g_d3dVertexShader);
		SafeRelease(g_d3dPixelShader);
	}


	void GraphicsDeviceWin32::Cleanup()
	{
		SafeRelease(g_d3dDepthStencilView);
		SafeRelease(g_d3dRenderTargetView);
		SafeRelease(g_d3dDepthStencilBuffer);
		SafeRelease(g_d3dDepthStencilState);
		SafeRelease(g_d3dRasterizerState);
		SafeRelease(g_d3dSwapChain);
		SafeRelease(g_d3dDeviceContext);
		SafeRelease(g_d3dDevice);
	}

	// Clear the color and depth buffers.
	void GraphicsDeviceWin32::Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil)
	{
		g_d3dDeviceContext->ClearRenderTargetView(g_d3dRenderTargetView, clearColor);
		g_d3dDeviceContext->ClearDepthStencilView(g_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
	}

	void GraphicsDeviceWin32::Present(bool vSync)
	{
		if (vSync)
		{
			g_d3dSwapChain->Present(1, 0);
		}
		else
		{
			g_d3dSwapChain->Present(0, 0);
		}
	}
}
