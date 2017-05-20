#ifndef _CE_GRAPHICS_DEVICE_WIN32_H_
#define _CE_GRAPHICS_DEVICE_WIN32_H_

#include "common\Singleton.h"

#include <Windows.h>

#include <d3d11.h>

#include <string>


namespace CE
{
	class GraphicsDeviceWin32 : public Singleton<GraphicsDeviceWin32>
	{
	public:
		bool Initialize(HINSTANCE hInstance, int cmdShow);
		void Destroy();

		void Update(float deltaTime);
		void Render();

	private:
		// Initialize the application window.
		int InitializeApplication(HINSTANCE hInstance, int cmdShow);

		// Initialize the DirectX device and swap chain.
		int InitializeDirectX(HINSTANCE hInstance, BOOL vSync);
		DXGI_RATIONAL QueryRefreshRate(
			unsigned int clientWidth,
			unsigned int clientHeight,
			BOOL vSync);

		bool LoadContent();
		// Get the latest profile for the specified shader type.
		template<typename ShaderType> std::string GetLatestProfile();
		template<typename ShaderType> ShaderType* CreateShader(
			ID3DBlob* pShaderBlob,
			ID3D11ClassLinkage* pClassLinkage);
		template<typename ShaderType> ShaderType* LoadShader(
			const std::wstring& fileName,
			const std::string& entryPoint,
			const std::string& profile);

		void UnloadContent();
		void Cleanup();

		void Clear(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);
		void Present(bool vSync);

	private:
		// Direct3D device and swap chain.
		ID3D11Device* g_d3dDevice = nullptr;
		ID3D11DeviceContext* g_d3dDeviceContext = nullptr;
		IDXGISwapChain* g_d3dSwapChain = nullptr;

		// Render target view for the back buffer of the swap chain.
		ID3D11RenderTargetView* g_d3dRenderTargetView = nullptr;
		// Depth/stencil view for use as a depth buffer.
		ID3D11DepthStencilView* g_d3dDepthStencilView = nullptr;
		// A texture to associate to the depth stencil view.
		ID3D11Texture2D* g_d3dDepthStencilBuffer = nullptr;

		// Define the functionality of the depth/stencil stages.
		ID3D11DepthStencilState* g_d3dDepthStencilState = nullptr;
		// Define the functionality of the rasterizer stage.
		ID3D11RasterizerState* g_d3dRasterizerState = nullptr;
		D3D11_VIEWPORT g_Viewport = {0};

		// Vertex buffer data.
		ID3D11InputLayout* g_d3dInputLayout = nullptr;
		ID3D11Buffer* g_d3dVertexBuffer = nullptr;
		ID3D11Buffer* g_d3dIndexBuffer = nullptr;

		// Shader data.
		ID3D11VertexShader* g_d3dVertexShader = nullptr;
		ID3D11PixelShader* g_d3dPixelShader = nullptr;

		// Shader resources.
		enum ConstantBuffer
		{
			CB_Application,
			CB_Frame,
			CB_Object,
			CB_Count
		};
		ID3D11Buffer* g_d3dConstantBuffers[ConstantBuffer::CB_Count];
	};

	class GraphicsDevice : public GraphicsDeviceWin32 {};
}

#endif // _CE_GRAPHICS_DEVICE_WIN32_H_