#pragma once
#include "Gaussianblur.h"
#include "Circle.h"
#include "Raytracer.h"

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

public:
	// 메인에서 필요한 데이터 반환
	int GetWidth();
	int GetHeight();

	// ImGui에서 필요한 데이터 반환
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	HWND GetHwnd();

private:
	void RenderBegin();
	void RenderEnd();

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	void CreateGeometry();
	void CreateInputLayout();
	
	// 원 추가
	void CreateCircleShaders();
	void CreateCircleInputLayout();

	void CreateVS();
	void CreatePS();

	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();
	void CreateSRV();

	void CreateConstantBuffer();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);



private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;

public:
	Gaussianblur gaussianblur;
	// 원 객체
	std::unique_ptr<Circle> _circle;
	Raytracer raytracer;
	std::vector<glm::vec4> pixels;


private:
	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	/// RTV
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.f, 0.f, 0.f, 0.f };

	ComPtr<ID3D11Texture2D> _texture = nullptr;

private:
	// Geometry
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	ComPtr<ID3D11InputLayout> _circleInputLayout = nullptr;

	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3D11VertexShader> _circleVertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;
	ComPtr<ID3DBlob> _circleVSBlob = nullptr;

	// RS
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3D11PixelShader> _circlePixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;
	ComPtr<ID3DBlob> _circlePSBlob = nullptr;

	// SV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;


	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	ComPtr<ID3D11BlendState> _blendState = nullptr;
	// [ CPU<->RAM ] [GPU<->VRAM]

private:
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;
};

