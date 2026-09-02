
#include <vector>

#include "Renderer.h"

#include "Sphere.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

void URenderer::Create(HWND hWindow)
{
	// Direct3D 장치 및 스왑 체인 생성
	CreateDeviceAndSwapChain(hWindow);

	// 프레임 버퍼 생성
	CreateFrameBuffer();

	// 래스터라이저 상태 설정
	CreateRasterizerState();
}

void URenderer::CreateDeviceAndSwapChain(HWND hWindow) {
	// 지원하는 Direct3D 기능 레벨을 정의
	D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

	//스왑 체인 설정 구조체 초기화
	DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
	swapchaindesc.BufferDesc.Width = 0;
	swapchaindesc.BufferDesc.Height = 0;
	swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 32비트 색상
	swapchaindesc.SampleDesc.Count = 1; // 멀티샘플링 비활성화
	swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟으로 사용
	swapchaindesc.BufferCount = 2; // 더블 버퍼링
	swapchaindesc.OutputWindow = hWindow; // 랜더링할 창핸들
	swapchaindesc.Windowed = TRUE; // 창모드
	swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 스왑 방식

	// Direct3D 장치와 스왑 체인을 생성
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
		&swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

	// 생성된 스왑 체인의 정보 가져오기
	SwapChain->GetDesc(&swapchaindesc);

	//뷰포트 정보 설정
	ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
}

// Direct3D 장치 및 스왑체인을 해제하는 함수
void URenderer::ReleaseDeviceAndSwapChain()
{
	if (DeviceContext)
	{
		DeviceContext->Flush(); // 남아있는 GPU 명령 실행
	}

	if (SwapChain)
	{
		SwapChain->Release();
		SwapChain = nullptr;
	}

	if (Device)
	{
		Device->Release();
		Device = nullptr;
	}

	if (DeviceContext)
	{
		DeviceContext->Release();
		DeviceContext = nullptr;
	}
}

// 프레임 버퍼를 생성하는 함수
void URenderer::CreateFrameBuffer()
{
	// 스왑 체인으로 부터 백 버퍼 텍스처 가져오기
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

	// 랜더 타겟 뷰 생성
	D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
	framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // 색상 포맷
	framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D텍스처

	Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
}

// 프레임 버퍼를 해제하는 함수
void URenderer::ReleaseFrameBuffer()
{
	if (FrameBuffer)
	{
		FrameBuffer->Release();
		FrameBuffer = nullptr;
	}

	if (FrameBufferRTV)
	{
		FrameBufferRTV->Release();
		FrameBufferRTV = nullptr;
	}
}

// 레스터라이저 상태를 생성하는 함수
void URenderer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerdesc = {};
	rasterizerdesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드
	rasterizerdesc.CullMode = D3D11_CULL_BACK; // 백페이스 컬링

	Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
}

// 레스터라이저 상태를 해제하는 함수
void URenderer::ReleaseRasterizerState()
{
	if (RasterizerState)
	{
		RasterizerState->Release();
		RasterizerState = nullptr;
	}
}

// 랜더러에 사용된 모든 리소스를 해제하는 함수
void URenderer::Release()
{
	RasterizerState->Release();

	// 렌더 타겟을 초기화
	DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	ReleaseFrameBuffer();
	ReleaseDeviceAndSwapChain();

	// Todo: fix
	{
		ReleaseVertexBuffer(VertexBufferTriangle);
		ReleaseVertexBuffer(VertexBufferSquare);
		//ReleaseVertexBuffer(VertexBufferSphere);
	
		for (ID3D11ShaderResourceView* textureSRV : TextureSRVs)
		{
			textureSRV->Release();
		}
	}
}

// 스왑체인의 백버퍼와 프론트 버퍼를 교체하며 화면에 출력
void URenderer::SwapBuffer()
{
	SwapChain->Present(1, 0); // 1: VSync 활성화
}

void URenderer::CreateShader()
{
	ID3DBlob* vertexshaderCSO;
	ID3DBlob* pixelshaderCSO;

	D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
	Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

	D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
	Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

	Stride = sizeof(FVertexSimple);

	vertexshaderCSO->Release();
	pixelshaderCSO->Release();
}

void URenderer::ReleaseShader()
{
	if (SimpleInputLayout)
	{
		SimpleInputLayout->Release();
		SimpleInputLayout = nullptr;
	}

	if (SimplePixelShader)
	{
		SimplePixelShader->Release();
		SimplePixelShader = nullptr;
	}

	if (SimpleVertexShader)
	{
		SimpleVertexShader->Release();
		SimpleVertexShader = nullptr;
	}
}
void  URenderer::Prepare()
{
	DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);


	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DeviceContext->RSSetViewports(1, &ViewportInfo);
	DeviceContext->RSSetState(RasterizerState);

	DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
	DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void URenderer::PrepareShader()
{
	DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
	DeviceContext->IASetInputLayout(SimpleInputLayout);
		
	// Todo: Move to other class
	/*
	ID3D11ShaderResourceView* pixelResources[1] = 
	{ 
		TextureResourceView,
	};
	DeviceContext->PSSetShaderResources(0, 1, &TextureSRVs[0]);
	*/

	DeviceContext->PSSetSamplers(0, 1, &SamplerState);

	if (ConstantBuffer)
	{
		DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	}
}

void URenderer::RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
{
	UINT offset = 0;
	DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

	DeviceContext->Draw(numVertices, 0);
}

ID3D11Buffer* URenderer::CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
{
	// 버텍스 버퍼 생성
	D3D11_BUFFER_DESC vertexbufferdesc = {};
	vertexbufferdesc.ByteWidth = byteWidth;
	vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // 업데이트
	vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

	ID3D11Buffer* vertexBuffer;

	Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

	return vertexBuffer;
}

void URenderer::ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	vertexBuffer->Release();
}

void URenderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC constantbufferdesc = {};
	constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0; // 상수버퍼 크기가 16바이트의 배수임을 보장
	constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // 매 프레임 CPU로 업데이트
	constantbufferdesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
}

void URenderer::ReleaseConstantBuffer()
{
	if (ConstantBuffer)
	{
		ConstantBuffer->Release();
		ConstantBuffer = nullptr;
	}
}

void URenderer::UpdateConstant(FVector offset, float scale, FColor color)
{
	if (ConstantBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

		DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR);
		FConstants* constants = (FConstants*)constantbufferMSR.pData;
		{
			constants->Offset = offset;
			constants->Scale = scale;
			constants->Color = color;
		}
		DeviceContext->Unmap(ConstantBuffer, 0);
	}
}

void URenderer::CreateTexture(const std::string fileName, ID3D11ShaderResourceView** outTextureSRV)
{
	int width;
	int height;
	int channels;

	unsigned char* loadImage = stbi_load(fileName.c_str(), &width, &height, &channels, 4);
	//assert(channels == 4);

	std::vector<uint8_t> image;

	image.resize(width * height * channels);
	memcpy(image.data(), loadImage, image.size() * sizeof(uint8_t));

	// Create texture.
	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.Width = width;
	txtDesc.Height = height;
	txtDesc.MipLevels = txtDesc.ArraySize = 1;
	txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = image.data();
	InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * channels;
	// InitData.SysMemSlicePitch = 0;

	Device->CreateTexture2D(&txtDesc, &InitData, &Texture);
	assert(Texture != nullptr);

	Device->CreateShaderResourceView(Texture, nullptr, outTextureSRV);
	
	Texture->Release();
	stbi_image_free(loadImage);
}

void URenderer::CreateSampler()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the Sample State
	Device->CreateSamplerState(&sampDesc, &SamplerState);
}

void URenderer::InitializeResources()
{
	// Todo: Fix

	// 버텍스 생성
	FVertexSimple triangle_vertices[] =
	{
		{  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f }, // Top vertex (red)
		{  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f }, // Bottom-right vertex (green)
		{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }  // Bottom-left vertex (blue)
	};

	FVertexSimple square_vertices[] =
	{
		// Front face (Z+)
		{ -1.0f, -1.0f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f }, // Bottom-left (red)
		{ -1.0f, 1.0f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f }, // Top-left (yellow)
		{ 1.0f, -1.0f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f }, // Bottom-right (green)

		{ -1.0f, 1.0f, 0.5f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f }, // Top-left (yellow)
		{ 1.0f, 1.0f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f }, // Top-right (blue)
		{  1.0f, -1.0f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f }, // Bottom-right (green)
	};

	{
		// 버텍스 버퍼 생성
		//renderer.VertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
		//renderer.NumVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
		VertexBufferTriangle = CreateVertexBuffer(triangle_vertices, sizeof(triangle_vertices));
		NumVerticesTriangle = sizeof(triangle_vertices) / sizeof(FVertexSimple);

		VertexBufferSquare = CreateVertexBuffer(square_vertices, sizeof(square_vertices));
		NumVerticesSquare = sizeof(square_vertices) / sizeof(FVertexSimple);
	}

	// Create textures
	{
		std::string imageFileNames[EI_MAX] =
		{
			"Resources/Images/Background_Start.png",
			"Resources/Images/Background_Start_With_Menu.png",
			"Resources/Images/Background_Game.png",
			"Resources/Images/Background_End.png",
			"Resources/Images/Button_Credit.png",
			"Resources/Images/Button_Replay.png",
			"Resources/Images/Button_Exit.png",
			"Resources/Images/String_Total_Score.png",
			"Resources/Images/String_Game_Clear.png",
			"Resources/Images/Number_9.png",
			"Resources/Images/Number_8.png",
			"Resources/Images/Number_7.png",
			"Resources/Images/Number_6.png",
			"Resources/Images/Number_5.png",
			"Resources/Images/Number_4.png",
			"Resources/Images/Number_3.png",
			"Resources/Images/Number_2.png",
			"Resources/Images/Number_1.png",
			"Resources/Images/Number_0.png",
			"Resources/Images/Ball_Yellow.png",
			"Resources/Images/Ball_Red.png",
			"Resources/Images/Ball_Purple.png",
			"Resources/Images/Ball_Green.png",
			"Resources/Images/Ball_Blue.png",
			"Resources/Images/Ball_Black.png",
		};

		ID3D11ShaderResourceView* textureSRV = nullptr;
		
		for (unsigned int i = 0; i < EI_MAX; ++i)
		{
			CreateTexture(imageFileNames[i], &textureSRV);
			TextureSRVs.push_back(textureSRV);
		}
	}

	CreateSampler();
}

void URenderer::SetTextureSRV(EImage eImage)
{
	DeviceContext->PSSetShaderResources(0, 1, &TextureSRVs[eImage]);
}

