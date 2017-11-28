#include "Device.h"



Device::Device()
{
	timer = new Time();
	m_camera = new Camera();
	m_input = new Input();
	looper = new Model();
}

Device::~Device()
{
}

HRESULT Device::Initialize(HINSTANCE hinstance, HWND hwnd, int width, int height, int sx, int sy)
{
	HRESULT hr = S_OK;
	m_hwnd = hwnd;

	clear = new float[4];
	for (int i = 0; i<4; i++)
		clear[i] = ClearColor[i];

	hr = m_input->Initialize(hinstance, hwnd, width, height);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Input Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	//XMFLOAT3 eye = XMFLOAT3(0.0f, 1.0f, 5.0f);
	//XMFLOAT3 at = XMFLOAT3(0.0f, 1.0f, 0.0f);
	hr=m_camera->Initialize(hinstance, hwnd,m_input, width, height);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Camera Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	hr = InitializeSwapChain();
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Swap Chain Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	hr = InitializeRenderTarget(Device::AddDepth);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Render Target Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	hr = InitializeDepthView();
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Depth View Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	hr = InitializeViewPort(this->width, this->height);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"View Port Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	hr = InitializeBlend();
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Blend Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	hr = InitializeCullState();
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Cull  State Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	hr = InitializeModel();
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Model  Initialize Failed!", L"Error", MB_OK);
		return hr;
	}

	hr = imgui::Initialize(m_pDevice, m_pImmediateContext, hwnd, m_camera,m_input,width, height,sx,sy);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Initialize  IMGUI   Failed!", L"Error", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Device::Update()
{
	HRESULT hr = S_OK;
	//Camera Movement
	m_input->Frame();
	m_camera->Frame();
	timer->Frame();


	deltaTime = timer->GetDeltaTime();
	FPS = timer->GetFps();
	m_camera->SetDeltaTime(deltaTime);
	//Angel
	Angel += deltaTime*2.0f;
	if (Angel > 2 * PI)
		Angel -= 2 * PI;

	////Constance Buffer
	////m_World = XMMatrixRotationY(Angel);
	//m_View= m_camera->GetViewMatrix();
	//m_Projection = m_camera->GetProjectionMatrix();
	//ConstanceBuffer cb;
	////cb.view =XMMatrixTranspose(this->m_View);
	////cb.projection = XMMatrixTranspose(this->m_Projection);
	////cb.world = XMMatrixTranspose(this->m_World);
	//cb.world = m_World;
	//cb.view = m_View;
	//cb.projection = m_Projection;
	//m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	////Light Constant Buffer
	//XMFLOAT3 lightPos = XMFLOAT3(-500.0f, -1000.f, 0.0f);
	//XMMATRIX translate = XMMatrixRotationY(Angel);
	//XMVECTOR _lightPos = XMLoadFloat3(&lightPos);
	//XMFLOAT3 lightDir = XMFLOAT3(500.0f, 1000.f, 0.0f);
	//XMFLOAT4 lightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//XMFLOAT3 viewPos = m_camera->GetEyePos();
	//LightConst lc;
	//lc.LightColor = lightColor;
	//lc.LightDir = lightDir;
	//lc.LightPos = lightPos;
	//lc.ViewPos = viewPos;
	//m_pImmediateContext->UpdateSubresource(m_pLightConstBuffer, 0, NULL, &lc, 0, 0);

	////Material Buffer
	//FLOAT glossness = 8.0f;
	//XMFLOAT4 matColor = XMFLOAT4(1.0f, 0.498f, 0.3137f, 1.0f); //Éºº÷ºì
	//Material mat;
	//mat.Glossness = glossness;
	//mat.MatColor = matColor;
	//m_pImmediateContext->UpdateSubresource(m_pMaterialBuffer, 0, NULL, &mat, 0, 0);


	//cube->SetView(m_camera->GetViewMatrix());
	//cube->SetProjection(m_camera->GetProjectionMatrix());
	looper->SetView(m_camera->GetViewMatrix());
	looper->SetProjection(m_camera->GetProjectionMatrix());
	//grid->SetView(m_camera->GetViewMatrix());
	//grid->SetProjection(m_camera->GetProjectionMatrix());

	//cube->Update(m_pImmediateContext);
	looper->Update(m_pImmediateContext);
	//grid->Update(m_pImmediateContext);

	//Line Mode
	if (m_camera->isSpaceDown())
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	else
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return S_OK;
}

void Device::Render()
{
	RenderBegin();

	TurnBlend(Device::ON);

	looper->Render(m_pImmediateContext);
	//
	//if (imgui::ImButton(1, 50, 50, 120, 30, new imgui::ImColor(0.0f, 0.8f, 0.5f, 0.2f)))
	//{
	//	clear = new float[4];
	//	srand((unsigned)time(NULL));
	//	int x = rand() % 11;
	//	int y = rand() % 11;
	//	int z = rand() % 11;
	//	clear[0] = x*0.1f;
	//	clear[1] = y*0.1f;
	//	clear[2] = z*0.1f;
	//	clear[3] = 1.0f;
	//}

	////std::string lab = "FPS:";
	////std::string fps;
	////imgui::ImFloat2String(fps, timer->GetFps());
	////lab.append(fps);
	////imgui::ImTextField(2, 600, 30, 60, 20, fps);
	////imgui::ImSimpleWindowBegin(5, 200, 50, 500, 500, "Simple Window");

	//imgui::ImTextField(6, 100, 400, 600, 30, "123132adadada");

	//imgui::ImButton(2, 200, 50, 120, 30);
	//imgui::ImButton(3, 50, 200, 120, 30);
	//imgui::ImButton(4, 200, 200, 120, 30);

	//float a=imgui::ImSliderHorizon(5, 550, 50, 300, 25);
	//std::string str;
	//imgui::ImFloat2String(str, a);
	//std::cout << "string::" << str << std::endl;
	//imgui::ImLabel(30, 30, 400, 500, new imgui::ImColor(0.1f, 0.1f, 0.1f,a));

	//imgui::ImTextField(7, 550, 100, 200, 25, str);

	{
		std::string lab = "This is a TextField";
		imgui::ImTextField(2, 20, 340, 60, 20, lab);
	}

	{
		imgui::ImButton(1, 30, 30, 120, 30);
		imgui::ImButton(2, 30, 70, 120, 30);
		imgui::ImButton(3, 30, 110, 120, 30);
		imgui::ImButton(4, 30, 150, 120, 30);
	}

	{
		float num = 0;
		num = imgui::ImSliderHorizon(5, 30, 190, 160, 30);
		std::string n="";
		imgui::ImFloat2String(n, num);
		imgui::ImTextField(4, 30, 230, 100, 30, n);
	}

	{
		imgui::ImLabel(20, 20, 240, 300, new imgui::ImColor(0.4, 0.4, 0.4));
	}





	TurnBlend(Device::OFF);


	RenderEnd();
}

HRESULT Device::InitializeSwapChain()
{
	HRESULT hr = S_OK;
	//---Device And Swap Chain---
	//Device Flags
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//WIndowSize
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	this->width = rc.right - rc.left;
	this->height = rc.bottom - rc.top;
	//Swap Chain
	D3D_DRIVER_TYPE driverTypes = D3D_DRIVER_TYPE_HARDWARE;
	//D3D_FEATURE_LEVEL featureLevels[] = 
	//{
	//	D3D_FEATURE_LEVEL_11_0,
	//	D3D_FEATURE_LEVEL_10_1,
	//	D3D_FEATURE_LEVEL_10_0, };

	//UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC sd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hwnd;
	sd.Windowed = TRUE;
	sd.BufferCount = 1;  //Ë«»º³å
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	//Create Device And Swap Chain
	hr = D3D11CreateDeviceAndSwapChain(NULL, driverTypes, NULL, createDeviceFlags, &featureLevel, 1
		, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, &m_featureLevel, &m_pImmediateContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Swap Chain Failed", L"Error", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Device::InitializeRenderTarget(bool NoDepth)
{
	HRESULT hr = S_OK;

	//D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	//D3D11_TEXTURE2D_DESC td;

	//td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//td.Usage = D3D11_USAGE_DEFAULT;
	//td.MipLevels = 1;
	//td.ArraySize = 1;
	//td.SampleDesc.Count = 1;
	//td.SampleDesc.Quality = 0;
	//td.Width = width;
	//td.Height = height;
	//td.CPUAccessFlags = 0;
	//td.MiscFlags = 0;
	//hr=m_pDevice->CreateTexture2D(&td, NULL, &m_pRenderTexture);
	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, L"Create Render / Shader Texture Failed", L"Error", MB_OK);
	//	return hr;
	//}

	//rtvd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//rtvd.Texture2D.MipSlice = 0;
	//rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//hr = m_pDevice->CreateRenderTargetView(m_pRenderTexture, &rtvd, &m_pRenderTargetView);
	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, L"Create Render Target View Failed", L"Error", MB_OK);
	//	return hr;
	//}


	//---Render Target Creation---
	//Get Swap Chain Buffer
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;
	//Create Render Target View
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Render Target Failed", L"Error", MB_OK);
		return hr;
	}

	if(NoDepth)
		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	return hr;
}

HRESULT Device::InitializeShaderView()
{
	HRESULT hr = S_OK;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	srvd.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	srvd.Texture2D.MostDetailedMip = 0;
	hr = m_pDevice->CreateShaderResourceView(m_pRenderTexture, &srvd, &m_pShaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Shader Resource View Failed", L"Error", MB_OK);
		return hr;
	}

	return hr;
}

HRESULT Device::InitializeDepthView()
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hr = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencil);
	if (FAILED(hr))
		return hr;


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	return hr;
}

HRESULT Device::InitializeZBuffer()
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthEnableState);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	hr = m_pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisableState);
	if (FAILED(hr))
	{
		return hr;
	}

	// Set the depth stencil state.
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthEnableState, 1);



	return hr;
}

HRESULT Device::InitializeBlend()
{
	HRESULT hr = S_OK;

	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = true;
	blendStateDescription.AlphaToCoverageEnable = false;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Create the blend state using the description.
	hr = m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(hr))
	{
		return hr;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	hr = m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT Device::InitializeCullState()
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pCullBackState);
	if (FAILED(hr))
	{
		return hr;
	}

	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pCullFrontState);
	if (FAILED(hr))
	{
		return hr;
	}

	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pCullNoneState);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT Device::InitializeViewPort(int width, int height, int TopX, int TopY)
{
	HRESULT hr = S_OK;

	//Create ViewPort
	D3D11_VIEWPORT viewport;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = TopX;
	viewport.TopLeftY = TopY;
	m_pImmediateContext->RSSetViewports(1, &viewport);

	return hr;
}

HRESULT Device::InitializeModel()
{
	HRESULT hr = S_OK;


	//hr = cube->Initialize("model/cube8.obj", m_hwnd, m_pDevice, m_pImmediateContext, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), m_camera->GetEyePos());
	//if (FAILED(hr))
	//{
	//	MessageBox(m_hwnd, L"Model < Cube > Initialize Failed!", L"Error", MB_OK);
	//	return hr;
	//}
	//hr = cube->InitializeShader(m_pDevice, m_pImmediateContext, L"vertex.fx", L"pixel.fx", "vert", "pixel", "vs_4_0", "ps_4_0");
	//if (FAILED(hr))
	//{
	//	MessageBox(m_hwnd, L"Shader < Cube > Initialize Failed!", L"Error", MB_OK);
	//	return hr;
	//}
	//cube->SetPosition(10.0f, 10.0f, 10.0f);



	hr = looper->Initialize("model/looper.obj", m_hwnd, m_pDevice, m_pImmediateContext, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), m_camera->GetEyePos());
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, L"Model < Looper > Initialize Failed!", L"Error", MB_OK);
		return hr;
	}
	hr = looper->InitializeShader(m_pDevice, m_pImmediateContext, L"mvertex.fx", L"mpixel.fx", "vert", "pixel", "vs_4_0", "ps_4_0");
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, L"Shader < Looper > Initialize Failed!", L"Error", MB_OK);
		return hr;
	}



	//hr = grid->Initialize("model/plane.obj", m_hwnd, m_pDevice, m_pImmediateContext, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), m_camera->GetEyePos());
	//if (FAILED(hr))
	//{
	//	MessageBox(m_hwnd, L"Model < Grid > Initialize Failed!", L"Error", MB_OK);
	//	return hr;
	//}
	//hr = grid->InitializeShader(m_pDevice, m_pImmediateContext, L"vgrid.fx", L"pgrid.fx", "vert", "pixel", "vs_4_0", "ps_4_0");
	//if (FAILED(hr))
	//{
	//	MessageBox(m_hwnd, L"Shader < Grid > Initialize Failed!", L"Error", MB_OK);
	//	return hr;
	//}
	//Material mat;
	//mat.Glossness = 1.0f;
	//mat.MatColor = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	//grid->SetMaterial(mat);

	//hr = grid->Initialize(m_hwnd, m_pDevice, m_pImmediateContext);
	//if (FAILED(hr))
	//{
	//	MessageBox(m_hwnd, L"GRID  Initialize Failed!", L"Error", MB_OK);
	//	return hr;
	//}

	return hr;
}

HRESULT Device::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(m_hwnd, L"Complie 1 failed", L"ERROR", MB_OK);
		if (pErrorBlob != NULL)
		{
			MessageBox(m_hwnd, L"Shader Compiler Debugger Info Has Out", L"Error", MB_OK);
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}
		if (pErrorBlob)
			pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob)
		pErrorBlob->Release();

	return S_OK;
}

void Device::TurnBlend(bool on)
{
	if (on)
	{
		float factor[4] = { 0.0f,0.0f,0.0f,0.0f };
		m_pImmediateContext->OMSetBlendState(m_alphaEnableBlendingState, factor, 0xffffffff);
	}
	else
	{
		float factor[4] = { 0.0f,0.0f,0.0f,0.0f };
		m_pImmediateContext->OMSetBlendState(m_alphaDisableBlendingState, factor, 0xffffffff);
	}
}

void Device::TureZBuffer(bool on)
{
	if (on)
	{
		m_pImmediateContext->OMSetDepthStencilState(m_pDepthEnableState, 1);
	}
	else
	{
		m_pImmediateContext->OMSetDepthStencilState(m_pDepthDisableState, 1);
	}
}

void Device::TrueCullState(int cullstate)
{
	switch (cullstate)
	{
	case 0:
		m_pImmediateContext->RSSetState(m_pCullBackState);
		break;
	case 1:
		m_pImmediateContext->RSSetState(m_pCullFrontState);
		break;
	case 2:
		m_pImmediateContext->RSSetState(m_pCullNoneState);
		break;
	default:
		break;
	}
}

void Device::RenderBegin()
{

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clear);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::RenderEnd()
{
	m_pSwapChain->Present(0, 0);
}
