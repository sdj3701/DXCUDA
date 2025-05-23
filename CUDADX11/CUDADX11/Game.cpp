#include "pch.h"
#include "Game.h"
#include "Circle.h"
#include "Gaussianblur.h"

// 헤더에서 raytracer를 선언 한걸 초기화 해줘야함
Game::Game()
	: raytracer(GWinSizeX, GWinSizeY)
{
}

Game::~Game()
{

}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	//// 원 객체 생성
	//_circle = make_unique<Circle>(
	//	glm::vec2(_width / 2.0f, _height / 2.0f),  // 화면 중앙
	//	_height / 4.0f,                          // 화면 높이의 1/4 크기
	//	glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)       // 빨간색
	//);

	CreateDeviceAndSwapChain();		// 뒷편에서 그림을 그리고 메인 화면에 출력
	CreateRenderTargetView();		// 백 버퍼를 가져와서 이를 렌더 타겟 뷰로 변환
	SetViewport();					// 3D 렌더링이 표시될 영역을 정의합니다.

	CreateGeometry();				// 기하학적인 도형
	CreateVS();						// 쉐이더에 있는 정보 vs 정보 넘기기
	CreateInputLayout();			// CPU에 들어간 데이터를 어떻게 읽어야 하는지
	CreatePS();						// 쉐이더에 있는 정보 ps 정보 넘기기
	
	CreateCircleShaders();			// Circle 쉐이더 VS,PS 정보 넘기기
	CreateCircleInputLayout();		// Circle의 데이터를 어떻게 읽어야 하는지

	CreateRasterizerState();		//
	CreateSamplerState();			//
	CreateBlendState();				//
	CreateSRV();					//
	CreateConstantBuffer();			//

}

void Game::Update()
{
	static int count = 0;
	pixels.resize(raytracer.width * raytracer.height);
	// 구 렌더링 해줌 각 픽셀로
	raytracer.Render(pixels);

	// 텍스처 업데이트
	D3D11_MAPPED_SUBRESOURCE ms;
	HRESULT hr = _deviceContext->Map(_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	if (SUCCEEDED(hr)) {
		memcpy(ms.pData, pixels.data(), pixels.size() * sizeof(glm::vec4));
		_deviceContext->Unmap(_texture.Get(), 0);
	}
	count++;


	// TODO : 일단 상수 버퍼 사용안함
	//D3D11_MAPPED_SUBRESOURCE subResource;
	//ZeroMemory(&subResource, sizeof(subResource));

	//_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	//::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
	//_deviceContext->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	RenderBegin();
	
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA (셋팅 단계)
		// 정점 버퍼 설정 (메쉬 정보)
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		
		//인덱스 버퍼 설정 (정점 그리는 순서 정보) 
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		
		// 정점 데이터 구조 정의 (위치, uv 좌표)
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetInputLayout(_circleInputLayout.Get());
		
		// 삼각형 리스트로 그리기 설정
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				
		// VS (정점 처리 단계)
		// 버텍스 셰이더 설정
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetShader(_circleVertexShader.Get(), nullptr, 0);
		// 변환 행렬 등 상수 버퍼 설정
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
		
		// RS (정점을 픽셀로 변환)
		// 레스터라이저 상태 설정(와이어프레임/솔리드, 컬링 모드 등)
		_deviceContext->RSSetState(_rasterizerState.Get());

		// PS (픽셀 색상 계산)
		// 픽셀 셰이더 설정
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShader(_circlePixelShader.Get(), nullptr, 0);
		// 텍스처 리소스 설정 (이미지 데이터)
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		// 샘플러 상태 설정 (텍스처 필터링 방법)
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		_circle->DrawCircle(_width, _height, _shaderResourceView, _deviceContext, _constantBuffer);
		
		// OM (최종 픽셀 색상 결정)
		//블렌드 상태 설정 (투명도 처리 방법)
		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);
		 
		//_deviceContext->Draw(_vertices.size(), 0);
		// 인덱스 버퍼를 사용해서 그리기 (메모리 효율적)
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	RenderEnd();
}

int Game::GetWidth()
{
	return this->_width;
}

int Game::GetHeight()
{
	return this->_height;
}

ID3D11Device* Game::GetDevice()
{
	return _device.Get();
}

ID3D11DeviceContext* Game::GetDeviceContext()
{
	return _deviceContext.Get();
}

HWND Game::GetHwnd()
{
	return _hwnd;
}

void Game::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);	// 후면버퍼에 그려달라고 요청
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);		// 나머지 부분에 무슨색으로 채워줄까?
	_deviceContext->RSSetViewports(1, &_viewport);										// 화면의 크기 정보
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);		// 전면 버퍼에 넘겨달라는 코드
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));		// 데이터를 밀어두고 내가 다시 채워준다.
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,		// 그래픽 카드를 사용하겠다.
		nullptr,
		0,								
		nullptr,						// DX 버전 지원
		0,
		D3D11_SDK_VERSION,				// 버전
		&desc,							// 
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void Game::CreateRenderTargetView()
{

	HRESULT hr;

	// 백 버퍼 가져오기 
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// 렌더 타겟 뷰 생성 (호출을 통해 파이프라인에 바인딩)
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Game::SetViewport()
{
	// 래스터라이저 단계에서 사용되는 뷰포트 설정
	// 3D 공간에서 계산된 좌표를 2D 화면 죄표로 변환하는 방법
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

void Game::CreateGeometry()
{
	// 이미지 출력하는 코드 (Canvas)
	// VertexData
	// 정점에 대한 정보(Data) 입력
	{
		_vertices.resize(4);

		_vertices[0].position = Vec3(-1.f, -1.f, 0.f);
		_vertices[0].uv = Vec2(0.f, 1.f);
		//_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[1].position = Vec3(-1.f, 1.f, 0.f);
		_vertices[1].uv = Vec2(0.f, 0.f);
		//_vertices[1].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[2].position = Vec3(1.f, -1.f, 0.f);
		_vertices[2].uv = Vec2(1.f, 1.f);
		//_vertices[2].color = Color(1.f, 0.f, 0.f, 1.f);
		_vertices[3].position = Vec3(1.f, 1.f, 0.f);
		_vertices[3].uv = Vec2(1.f, 0.f);
		//_vertices[3].color = Color(1.f, 0.f, 0.f, 1.f);
	}

	// VertexBuffer
	// CPU메모리에 있는 데이터를 GPU메모리에 넘겨줘야하는 작업
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE; // 중요 : 어떻게 작업을 할 것인지? GPU만 읽을지 쓸지 정하는 방법 데이터
		// 처음 생성하면서 읽으면 원점을 수정하지 않기 때문에 읽기만 사용
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 어떤 방법으로 바인드 할 것인지?
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data(); // CPU의 처음 데이터를 입력

		// GPU에 메모리 생성 후 데이터 넘김
		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

	// index
	{
		_indices = { 0,1,2,2,1,3 };
	}

	// IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();

		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
	
}

void Game::CreateInputLayout()
{
	// 어떻게 구현 되어 있는가? 0 -> 12 data byte넘어가는 차이 4바이트 float Vec3로 구현 4*3 = 12
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	// 구현 정점과 쉐이더에 대한 정보 입력을 해야함
	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateCircleShaders()
{
	// 버텍스 셰이더 로드
	LoadShaderFromFile(L"CircleVS.hlsl", "main", "vs_5_0", _circleVSBlob);
	HRESULT hr = _device->CreateVertexShader(_circleVSBlob->GetBufferPointer(),
		_circleVSBlob->GetBufferSize(),
		nullptr,
		_circleVertexShader.GetAddressOf());
	CHECK(hr);

	// 픽셀 셰이더 로드
	LoadShaderFromFile(L"CirclePS.hlsl", "main", "ps_5_0", _circlePSBlob);
	hr = _device->CreatePixelShader(_circlePSBlob->GetBufferPointer(),
		_circlePSBlob->GetBufferSize(),
		nullptr,
		_circlePixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateCircleInputLayout()
{
	// 원 셰이더용 입력 레이아웃 설정
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	_device->CreateInputLayout(layout,
		count,
		_circleVSBlob->GetBufferPointer(),
		_circleVSBlob->GetBufferSize(),
		_circleInputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	// _vsBlob에다가 쉐이더 파일에 있는 정보를 입력해달라고 요청
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	// 정점 쉐이더를 생성
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	// _psBlob에다가 쉐이더 파일에 있는 정보를 입력해달라고 요청
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	// 픽셀 쉐이더를 생성
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;

	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);

}

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 18;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	_device->CreateSamplerState(&desc, _samplerState.GetAddressOf());
}

void Game::CreateBlendState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&desc, _blendState.GetAddressOf());
}

void Game::CreateSRV()
{
	// 1. 기존 이미지 로드 코드 대신 빈 텍스처 생성
	D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = _width;
    textureDesc.Height = _height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 일반적인 색상 포맷
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DYNAMIC; // CPU에서 쓰기 위해 DYNAMIC으로 변경
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 쓰기 접근 허용
    textureDesc.MiscFlags = 0;

	// 2. 초기 데이터 설정 (검은 배경)
	std::vector<glm::vec4> initialData(_width * _height, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	D3D11_SUBRESOURCE_DATA subresourceData;
	ZeroMemory(&subresourceData, sizeof(subresourceData));
	subresourceData.pSysMem = initialData.data();
	subresourceData.SysMemPitch = _width * sizeof(uint32_t);
	subresourceData.SysMemSlicePitch = 0;

	// 3. 텍스처 생성
	HRESULT hr = _device->CreateTexture2D(&textureDesc, &subresourceData, _texture.GetAddressOf());
	CHECK(hr);

	// 4. 셰이더 리소스 뷰 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = _device->CreateShaderResourceView(_texture.Get(), &srvDesc, _shaderResourceView.GetAddressOf());
	CHECK(hr);

	//DirectX::TexMetadata md;
	//DirectX::ScratchImage img;

	//const wchar_t* filename = L"image_1.jpg";

	//HRESULT hr = ::LoadFromWICFile(filename, WIC_FLAGS_NONE, &md, img);
	//CHECK(hr);

	//// 픽셀 데이터 추출 가우시안 블러를 적용하기 위한 추가 작업
	//const DirectX::Image* image = img.GetImage(0, 0, 0);
	//if (!image)
	//{
	//	std::cout << "not find Image : " << filename << "\n";
	//	return;
	//}

	//// 픽셀데이터 추출
	//gaussianblur.ProcessPixelDataFromDirectXImage(img, md);

	//hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	//CHECK(hr);
}

void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	// 디버그 모드이고 컴파일러는 건너 뛰겠다.
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// 쉐이더에 필요한 데이터
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr);

	CHECK(hr);
}
