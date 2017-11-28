#include<iostream>
#include"imgui.h"
#include"winvar.h"


namespace imgui
{
	//Global Var for Test Button/Label
	ID3D11Device*ui_device = NULL;
	ID3D11DeviceContext*ui_context = NULL;
	//Rect Buffer
	ID3D11Buffer*g_btnVertexBuffer = NULL;
	ID3D11Buffer*g_btnIndicesBuffer = NULL;
	ID3D11Buffer*g_btnConstBuffer = NULL;
	ID3D11VertexShader*g_btnVertexShader = NULL;
	ID3D11PixelShader*g_btnPixelShader = NULL;
	ID3D11InputLayout*g_btnLayout = NULL;
	unsigned int indexCount = 0;
	unsigned int vertexCount = 0;
	//Text Buffer
	ID3D11Buffer*g_txtVertexBuffer = NULL;
	ID3D11Buffer*g_txtIndicesBuffer = NULL;
	ID3D11Buffer*g_txtConstBuffer = NULL;
	ID3D11VertexShader*g_txtVertexShader = NULL;
	ID3D11PixelShader*g_txtPixelShader = NULL;
	ID3D11ShaderResourceView*g_txtSRV=NULL;
	ID3D11InputLayout*g_txtLayout = NULL;
	ID3D11SamplerState*g_txtSampleState = NULL;
	unsigned int text_indexCount = 0;
	//Others
	HWND ui_hwnd;
	int ui_winWidth = 0;
	int ui_winHeight = 0;
	int ui_screenW = 0;
	int ui_screenH = 0;
	int ui_topX = 0;
	int ui_topY = 0;
	Time*ui_time = NULL;
	Camera *ui_camera = NULL;
	Input*ui_input = NULL;

	UIState uistate = { 0,0,0,0,0 };
	int g_preposX = 0;
	int g_preposY = 0;
	int g_preheight = 0;
	int g_prewidth = 0;
	int g_preTextLen = 0;
	int ui_lastX = 0;
	int ui_lastY = 0;
	UIChar *ui_charList=NULL;
	char*str = NULL;

	//struct ImRGB
	//{
	//	ImColor RGB2ImColor()
	//	{
	//		ImColor *color = new ImColor(r / 255.0f, g / 255.0f, b / 255.0f);
	//		return *color;
	//	}
	//	ImColor RGB2ImColor(int r, int g, int b)
	//	{
	//		ImColor *color = new ImColor(r / 255.0f, g / 255.0f, b / 255.0f);
	//		return *color;
	//	}
	//	ImRGB()
	//	{
	//		r = g = b = 255;
	//	}
	//	ImRGB(int r, int g, int b)
	//	{
	//		this->r = r;
	//		this->g = g;
	//		this->b = b;
	//	}
	//	ImRGB(const ImRGB & color)
	//	{
	//		r = color.r;
	//		g = color.g;
	//		b = color.b;
	//	}
	//	int r;
	//	int g;
	//	int b;
	//};


	HRESULT Initialize(ID3D11Device*device, ID3D11DeviceContext*context, HWND hwnd, Camera*camera, Input*input, int winWidth, int winHeight,int sx,int sy)
	{
		HRESULT hr = S_OK;

		//Globel Var
		ui_device = device;
		ui_context = context;
		ui_hwnd = hwnd;
		ui_winWidth = winWidth;
		ui_winHeight = winHeight;
		ui_screenW = GetSystemMetrics(SM_CXSCREEN);
		ui_screenH = GetSystemMetrics(SM_CYSCREEN);
		ui_topX = sx;
		ui_topY = sy;
		ui_time = new Time();
		ui_camera = new Camera();
		ui_camera = camera;
		ui_input = new Input();
		ui_input = input;

		//Initialize UI Buffer
		hr = InitRect();
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed to initialize Label Buffer ", L"ERROR", MB_OK);
			return hr;
		}

		hr = InitTextRect();
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed to initialize Text Feild Buffer ", L"ERROR", MB_OK);
			return hr;
		}

		return hr;
	}

	HRESULT InitRect()
	{
		HRESULT hr = S_OK;

		UIVertexType*vertices = NULL;
		WORD *indices = NULL;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, consBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		indexCount = 6;
		vertexCount =6;
		vertices = new UIVertexType[vertexCount];
		indices = new WORD[indexCount];
		for (int i = 0; i < indexCount; i++)
			indices[i] = i;

		//ZeroMemory(vertices, sizeof(UIVertexType)*vertexCount);
		memset(vertices, 0, sizeof(UIVertexType)*vertexCount);
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&consBufferDesc, sizeof(D3D11_BUFFER_DESC));

		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(UIVertexType)*vertexCount;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		hr = ui_device->CreateBuffer(&vertexBufferDesc, &vertexData, &g_btnVertexBuffer);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Vertex Buffer ", L"ERROR", MB_OK);
			return hr;
		}

		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		indexBufferDesc.ByteWidth = sizeof(WORD)*indexCount;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		hr = ui_device->CreateBuffer(&indexBufferDesc, &indexData, &g_btnIndicesBuffer);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Index Buffer ", L"ERROR", MB_OK);
			return hr;
		}

		consBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		consBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		consBufferDesc.CPUAccessFlags = 0;
		consBufferDesc.ByteWidth = sizeof(UIConstBuffer);
		consBufferDesc.MiscFlags = 0;
		consBufferDesc.StructureByteStride = 0;

		hr = ui_device->CreateBuffer(&consBufferDesc, NULL, &g_btnConstBuffer);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Matrix Const Buffer ", L"ERROR", MB_OK);
			return hr;
		}

		delete[]vertices;
		delete[]indices;
		vertices = NULL;
		indices = NULL;


		ID3DBlob*vertexBlob = NULL;
		ID3DBlob*pixelBlob = NULL;

		hr = UICompileShaderFromFile(L"vertex.fx", "vert", "vs_4_0", &vertexBlob);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed  Compile Vertex shader ", L"ERROR", MB_OK);
			return hr;
		}

		hr = ui_device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &g_btnVertexShader);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Vertex Shader ", L"ERROR", MB_OK);
			vertexBlob->Release();
			return hr;
		}

		hr = UICompileShaderFromFile(L"pixel.fx", "pixel", "ps_4_0", &pixelBlob);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed  Compile Pixel Shader ", L"ERROR", MB_OK);
			return hr;
		}

		hr = ui_device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &g_btnPixelShader);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Pixel Shader ", L"ERROR", MB_OK);
			pixelBlob->Release();
			return hr;
		}

		D3D11_INPUT_ELEMENT_DESC layout[2];
		layout[0].SemanticName = "POSITION";
		layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout[0].AlignedByteOffset = 0;
		layout[0].SemanticIndex = 0;
		layout[0].InputSlot = 0;
		layout[0].InstanceDataStepRate = 0;

		layout[1].SemanticName = "TEXCOORD";
		layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout[1].AlignedByteOffset = 12;//D3D11_APPEND_ALIGNED_ELEMENT;
		layout[1].InputSlot = 0;
		layout[1].SemanticIndex = 0;
		layout[1].InstanceDataStepRate = 0;

		UINT numElements = ARRAYSIZE(layout);

		hr = ui_device->CreateInputLayout(layout, numElements, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &g_btnLayout);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Input Layout", L"ERROR", MB_OK);
			return hr;
		}

		pixelBlob->Release();
		vertexBlob->Release();


		return hr;
	}

	HRESULT InitTextRect()
	{
		HRESULT hr = S_OK;
		// Text  List  Init
		char*cstring = "pqrstuvwxyz{|}~ `abcdefghijklmnoPQRSTUVWXYZ[\\]^_@ABCDEFGHIJKLMNO0123456789:;<=>? !\"#$%&\'()*+,-.//";
		ui_charList = new UIChar[256];
		for (int i = 0; i < 16 * 6; i++)
		{
			ui_charList[(int)cstring[i]].x = (i % 16) * CHAR_WIDTH;
			ui_charList[(int)cstring[i]].y = (int)(i / 16) * CHAR_HEIGHT;
			ui_charList[(int)cstring[i]].ex = ui_charList[(int)cstring[i]].x + CHAR_WIDTH;
			ui_charList[(int)cstring[i]].ey = ui_charList[(int)cstring[i]].y + CHAR_HEIGHT;
			ui_charList[(int)cstring[i]].uvSX = (float)ui_charList[(int)cstring[i]].x / IMG_WIDTH;
			ui_charList[(int)cstring[i]].uvSY = (float)ui_charList[(int)cstring[i]].y / IMG_HEIGHT;
			ui_charList[(int)cstring[i]].uvEX = (float)ui_charList[(int)cstring[i]].ex / IMG_WIDTH;
			ui_charList[(int)cstring[i]].uvEY = (float)ui_charList[(int)cstring[i]].ey / IMG_HEIGHT;
			//std::cout << "编号:" << (int)cstring[i] << " 字符" << cstring[i] << " X值：" << ui_charList[(int)cstring[i]].x 
			//	<< " Y值：" << ui_charList[(int)cstring[i]].y <<"UV(Y):"<< ui_charList[(int)cstring[i]].uvSY<<", "<< ui_charList[(int)cstring[i]].uvEY << std::endl;
		}

		//Text Feild Init
		UIVertexType*vertices = NULL;
		WORD*indices = NULL;
		D3D11_BUFFER_DESC vertexDesc, indexDesc, constDesc;
		D3D11_SAMPLER_DESC samplerDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		int txt_vertexCount, txt_indicesCount;


		txt_vertexCount = 6*MAX_TEXT_LENGTH;
		txt_indicesCount = txt_vertexCount;
		vertices = new UIVertexType[txt_vertexCount];
		indices = new WORD[txt_indicesCount];
		for (int i = 0; i < txt_indicesCount; i++)
			indices[i] = i;

		//ZeroMemory(vertices, sizeof(UIVertexType)*vertexCount);
		memset(vertices, 0, sizeof(UIVertexType)*txt_vertexCount);
		ZeroMemory(&vertexDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&indexDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&constDesc, sizeof(D3D11_BUFFER_DESC));

		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexDesc.ByteWidth = sizeof(UIVertexType)*txt_vertexCount;
		vertexDesc.MiscFlags = 0;
		vertexDesc.StructureByteStride = 0;

		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		hr = ui_device->CreateBuffer(&vertexDesc, &vertexData, &g_txtVertexBuffer);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Vertex Buffer (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		indexDesc.Usage = D3D11_USAGE_DYNAMIC;
		indexDesc.ByteWidth = sizeof(WORD)*txt_indicesCount;
		indexDesc.MiscFlags = 0;
		indexDesc.StructureByteStride = 0;

		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		hr = ui_device->CreateBuffer(&indexDesc, &indexData, &g_txtIndicesBuffer);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Index Buffer (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.Usage = D3D11_USAGE_DEFAULT;
		constDesc.CPUAccessFlags = 0;
		constDesc.ByteWidth = sizeof(UITextConstBuffer);
		constDesc.MiscFlags = 0;
		constDesc.StructureByteStride = 0;

		hr = ui_device->CreateBuffer(&constDesc, NULL, &g_txtConstBuffer);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Matrix Const Buffer (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		delete[]vertices;
		delete[]indices;
		vertices = NULL;
		indices = NULL;

		//texture
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MinLOD = 0;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;

		// Create the texture sampler state.
		hr = ui_device->CreateSamplerState(&samplerDesc, &g_txtSampleState);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Sample State (TxT)", L"ERROR", MB_OK);
			return false;
		}

		hr = D3DX11CreateShaderResourceViewFromFile(ui_device, L"img/font.dds", NULL, NULL, &g_txtSRV, NULL);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Shader Resource View (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		//V`P  Shader & Input Layout
		ID3DBlob*vertexBlob = NULL;
		ID3DBlob*pixelBlob = NULL;
		hr = UICompileShaderFromFile(L"textvertex.fx", "vert", "vs_4_0", &vertexBlob);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Compile VS Faild (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		hr = ui_device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &g_txtVertexShader);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  VS Faild (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		hr = UICompileShaderFromFile(L"textpixel.fx", "pixel", "ps_4_0", &pixelBlob);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Compile PS Faild (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		hr = ui_device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &g_txtPixelShader);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  PS Faild (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		D3D11_INPUT_ELEMENT_DESC layout[2];
		layout[0].SemanticName = "POSITION";
		layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout[0].AlignedByteOffset = 0;
		layout[0].SemanticIndex = 0;
		layout[0].InputSlot = 0;
		layout[0].InstanceDataStepRate = 0;

		layout[1].SemanticName = "TEXCOORD";
		layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout[1].AlignedByteOffset = 12;//D3D11_APPEND_ALIGNED_ELEMENT;
		layout[1].InputSlot = 0;
		layout[1].SemanticIndex = 0;
		layout[1].InstanceDataStepRate = 0;

		UINT numElements = ARRAYSIZE(layout);

		hr = ui_device->CreateInputLayout(layout, numElements, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &g_txtLayout);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Create  Input Layout (TxT)", L"ERROR", MB_OK);
			return hr;
		}

		pixelBlob->Release();
		vertexBlob->Release();

		return hr;
	}

	HRESULT UICompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
			MessageBox(ui_hwnd, L"Complie 1 failed", L"ERROR", MB_OK);
			if (pErrorBlob != NULL)
			{
				MessageBox(ui_hwnd, L"Shader Compiler Debugger Info Has Out", L"Error", MB_OK);
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

	bool MouseTrans()
	{
		//鼠标坐标转化  全屏幕--->窗口
		//ui_input->Frame();

		int mx = ui_input->GetPosX();
		int my = ui_input->GetPosY();

		if (mx <= WINVAR::GetStartX())
			uistate.mouseX = 0;
		else if (mx >= WINVAR::GetStartX() + WINVAR::GetWidth())
			uistate.mouseX = WINVAR::GetWidth();
		else
			uistate.mouseX = mx - WINVAR::GetStartX();

		if (my <= WINVAR::GetStartY())
			uistate.mouseY = 0;
		else if (my >= WINVAR::GetStartY() + WINVAR::GetHeight())
			uistate.mouseY = WINVAR::GetHeight();
		else
			uistate.mouseY = my - WINVAR::GetStartY() - 30;


		if (ui_input->isMouseLeftDown())
			uistate.mouseDown = 1;
		else
			uistate.mouseDown = 0;

		return 1;
	}

	bool UpdateBuffer(int x, int y, int w, int h, ImColor *color)
	{

		//鼠标转化
		MouseTrans();
		//检查位置和高度是否没有改变
		if (x == g_preposX&&y == g_preposY&&w == g_prewidth&&h == g_preheight)
			return true;

		g_preheight = h;
		g_prewidth = w;
		g_preposX = x;
		g_preposY = y;

		//Declare Area
		int left, bottom, right, top;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		UIVertexType*vertices = NULL;
		UIVertexType*verticesPtr = NULL;
		UIConstBuffer constBuffer;
		HRESULT hr = S_OK;

		//Update Vertex Buffer
		//left = x;
		//top = y;
		//right = x + w;
		//bottom = y + h;
		bottom = y;
		left = x;
		top = y + h;
		right = x + w;
		vertices = new UIVertexType[vertexCount];

		//记录：
		//[1]  DX坐标轴窗口中心为原点（0,0）
		//[2]  顶点载入方向为顺时针（逆时针导致背面裁剪）（ps：也有可能是我相机位置）
		vertices[0].pos = XMFLOAT3(left, top, 0.0f);
		vertices[0].tex = XMFLOAT2(0.0f, 0.0f);

		vertices[2].pos = XMFLOAT3(right, bottom, 0.0f);
		vertices[2].tex = XMFLOAT2(1.0f, 1.0f);

		vertices[1].pos = XMFLOAT3(right, top, 0.0f);
		vertices[1].tex = XMFLOAT2(0.0f, 1.0f);

		vertices[3].pos = XMFLOAT3(left, top, 0.0f);
		vertices[3].tex = XMFLOAT2(0.0f, 0.0f);

		vertices[5].pos = XMFLOAT3(left, bottom, 0.0f);
		vertices[5].tex = XMFLOAT2(1.0f, 0.0f);

		vertices[4].pos = XMFLOAT3(right, bottom, 0.0f);
		vertices[4].tex = XMFLOAT2(1.0f, 1.0f);


		hr = ui_context->Map(g_btnVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Failed Mapped  Vertex Buffer ", L"ERROR", MB_OK);
			return hr;
		}

		verticesPtr = (UIVertexType*)mappedResource.pData;

		memcpy((void*)verticesPtr, (void*)vertices, sizeof(UIVertexType)*vertexCount);
		//mappedResource.pData = vertices;

		ui_context->Unmap(g_btnVertexBuffer, 0);

		delete[] vertices;

		//Update Constant Buffer
		constBuffer.world = XMMatrixIdentity();
		//constBuffer.view = ui_camera->GetViewMatrix();
		constBuffer.view = ui_camera->GetViewConstMatrix();
		constBuffer.projection = ui_camera->GetOrthoMatrix();
		constBuffer.bgColor = XMFLOAT4(color->r, color->g, color->b,color->a);

		ui_context->UpdateSubresource(g_btnConstBuffer, 0, NULL, &constBuffer, 0, 0);

	}

	bool UpdateText(int x, int y, int w, int h, UISentence* sentence)
	{
		//鼠标转化
		MouseTrans();
		//变动识别
		if (x == g_preposX&&y == g_preposY&&w == g_prewidth&&h == g_preheight)
			return true;

		g_preheight = h;
		g_prewidth = w;
		g_preposX = x;
		g_preposY = y;


		//Update 缓冲区
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		UIVertexType*vertices = NULL;
		UIVertexType*verticesPtr = NULL;
		WORD*indices = NULL;
		UITextConstBuffer constBuffer;
		int textWidth = h-h/10;
		int textHeight = h;

		//int textWidth = w / sentence->strLen;
		//int textHeight = h / sentence->strLen;
		HRESULT hr = S_OK;
		int text_vertexCount;

		text_vertexCount = sentence->strLen * 6;
		text_indexCount = text_vertexCount;
		vertices = new UIVertexType[text_vertexCount];

		//xy坐标预处理---->向右下角轻微移动
		x += 5;
		y += 5;
		//顶点遍历计数器
		int v = 0;
		std::string str=sentence->str.c_str();
		for (int i = 0; i < sentence->strLen; i++)
		{
			vertices[v].pos = XMFLOAT3((float)x, (float)y, 0.0f);
			vertices[v].tex = XMFLOAT2(ui_charList[(int)str[i]].uvSX, ui_charList[(int)str[i]].uvEY);
			v++;
			vertices[v].pos = XMFLOAT3((float)x+textWidth, (float)y+h, 0.0f);
			vertices[v].tex = XMFLOAT2(ui_charList[(int)str[i]].uvEX, ui_charList[(int)str[i]].uvSY);
			v++;
			vertices[v].pos = XMFLOAT3((float)x + textWidth, (float)y, 0.0f);
			vertices[v].tex = XMFLOAT2(ui_charList[(int)str[i]].uvEX, ui_charList[(int)str[i]].uvEY);
			v++;
			vertices[v].pos = XMFLOAT3((float)x, (float)y, 0.0f);
			vertices[v].tex = XMFLOAT2(ui_charList[(int)str[i]].uvSX, ui_charList[(int)str[i]].uvEY);
			v++;
			vertices[v].pos = XMFLOAT3((float)x, (float)y+h, 0.0f);
			vertices[v].tex = XMFLOAT2(ui_charList[(int)str[i]].uvSX, ui_charList[(int)str[i]].uvSY);
			v++;
			vertices[v].pos = XMFLOAT3((float)x + textWidth, (float)y + h, 0.0f);
			vertices[v].tex = XMFLOAT2(ui_charList[(int)str[i]].uvEX, ui_charList[(int)str[i]].uvSY);
			v++;
			x += textWidth;
		}

		hr = ui_context->Map(g_txtVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			MessageBox(ui_hwnd, L"Map (TEXT) Vertex Buffer Faild", L"Error", MB_OK);
			return hr;
		}

		verticesPtr = (UIVertexType*)mappedResource.pData;
		memcpy((void*)verticesPtr, (void*)vertices, sizeof(UIVertexType)*text_vertexCount);
		ui_context->Unmap(g_txtVertexBuffer, 0);

		delete[] vertices;

		//Update Constant Buffer
		constBuffer.world = XMMatrixIdentity();
		//constBuffer.view = ui_camera->GetViewMatrix();
		constBuffer.view = ui_camera->GetViewConstMatrix();
		constBuffer.projection = ui_camera->GetOrthoMatrix();
		constBuffer.textColor = XMFLOAT4(sentence->textColor->r, sentence->textColor->g, sentence->textColor->b, sentence->textColor->a);
		constBuffer.bgColor = XMFLOAT4(sentence->bgColor->r, sentence->bgColor->g, sentence->bgColor->b, sentence->bgColor->a);

		ui_context->UpdateSubresource(g_txtConstBuffer, 0, NULL, &constBuffer, 0, 0);

		return 1;
	}

	void PosTrans(int&x, int &y, int width, int height)
	{
		//绘图坐标转化 中心坐标系--->左上角坐标系
		y = -y;
		x -= ui_winWidth / 2;
		y += (ui_winHeight / 2 - height);
	}

	//src1*n+src2*(1-n)
	ImColor ImLerp(ImColor src1, ImColor src2, float n)
	{
		ImColor dest;
		dest.r = src1.r*n + src2.r*(1 - n);
		dest.g = src1.g*n + src2.g*(1 - n);
		dest.b = src1.b*n + src2.b*(1 - n);
		return dest;
	}

	void DrawRect(int x, int y, int width, int height, ImColor*color)
	{
		PosTrans(x, y, width, height);

		UpdateBuffer(x, y, width, height, color);
		UINT stride = sizeof(UIVertexType);
		UINT offset = 0;

		ui_context->IASetVertexBuffers(0, 1, &g_btnVertexBuffer, &stride, &offset);
		ui_context->IASetInputLayout(g_btnLayout);
		ui_context->IASetIndexBuffer(g_btnIndicesBuffer, DXGI_FORMAT_R16_UINT, 0);

		ui_context->VSSetShader(g_btnVertexShader, NULL, 0);
		ui_context->VSSetConstantBuffers(0, 1, &g_btnConstBuffer);
		ui_context->PSSetShader(g_btnPixelShader, NULL, 0);

		ui_context->DrawIndexed(indexCount, 0, 0);
	}

	void DrawTextField(int x, int y, int width, int height, UISentence*sentence)
	{
		PosTrans(x, y, width, height);

		UpdateText(x, y, width, height, sentence);

		UINT stride = sizeof(UIVertexType);
		UINT offset = 0;
		ui_context->IASetIndexBuffer(g_txtIndicesBuffer, DXGI_FORMAT_R16_UINT, 0);
		ui_context->IASetVertexBuffers(0, 1, &g_txtVertexBuffer, &stride, &offset);
		ui_context->IASetInputLayout(g_txtLayout);

		ui_context->VSSetShader(g_txtVertexShader, NULL, 0);
		ui_context->PSSetShader(g_txtPixelShader, NULL, 0);

		ui_context->VSSetConstantBuffers(0, 1, &g_txtConstBuffer);
		ui_context->PSSetConstantBuffers(0, 1, &g_txtConstBuffer);
		ui_context->PSSetShaderResources(0, 1, &g_txtSRV);
		ui_context->PSSetSamplers(0, 1, &g_txtSampleState);

		ui_context->DrawIndexed(text_indexCount, 0, 0);
	}

	int isAreaHit(int x, int y, int width, int height)
	{
		//std::cout << "[1] Mouse :" << uistate.mouseX << ":xs " << x <<":xe"<<x+width<< std::endl;
		//std::cout<<" ----- " << uistate.mouseY <<":ys "<<y<<":ye "<<y+height<< std::endl<<std::endl;
		if (uistate.mouseX<x || uistate.mouseY<y || uistate.mouseX>x + width || uistate.mouseY>y + height)
			return 0;
		return 1;
	}

	int ImButton(int id, int x, int y, int width, int height,ImColor*button)
	{
		//判定点击
		if (isAreaHit(x, y, width, height) == 1)
		{
			uistate.hotitem = id;
			if (uistate.activeitem == 0 && uistate.mouseDown)
				uistate.activeitem = id;
			else if(!uistate.mouseDown)
				uistate.activeitem = 0;
		}
		else
			uistate.hotitem = 0;

		ImColor *shadow = new ImColor(0.0f, 0.0f, 0.0f);
		//ImColor*button = new ImColor(0.0f, 0.3f, 0.6f);
		ImColor*line = new ImColor(0.9f, 0.9f, 0.9f);
		*line = ImLerp(*line, *button, 0.8f);
		//绘制
		//--点击
		if (uistate.hotitem == id)
		{
			if (uistate.activeitem == id)
			{
				DrawRect(x + 3, y + 3, width-6, height-6, button);
			}
			else
			{
				DrawRect(x, y, width, height, button);
				DrawRect(x - 2, y - 2, width + 4, height + 4, line);
			}
		}
		else
			DrawRect(x, y, width, height, button);
		//--阴影
		DrawRect(x + 3, y + 3, width, height, shadow);


		if (uistate.mouseDown == 0 &&
			uistate.hotitem == id &&
			uistate.activeitem == id)
			return 1;

		// Otherwise, no clicky.
		return 0;
	}

	int ImLabel(int x, int y, int width, int height, ImColor* color)
	{
		DrawRect(x, y, width, height, color);
		DrawRect(x - 1, y - 1, width + 2, height + 2, new ImColor(0.0f, 0.0f, 0.0f));

		return 1;
	}

	float ImSliderVertical(int id, int x, int y, int width, int height, float start , float end )
	{
		//slider button x=x-2  y=y-2  width=width-4  height=1/5 *height
		return 0;
	}

	float ImSliderHorizon(int id, int x, int y, int width, int height, float start , float end )
	{
		////slider button x=x-2  y=y-2  width=width-4  height=30
		static int slideHeight = height +8;
		static int slideWidth = 30;
		static int slideX = x + 4;
		static int slideY = y - 4;
		float barLen = width - 8;
		float Num = end - start;//倍率计算

		ImColor*shadow = new ImColor(0.0f, 0.0f, 0.0f);
		ImColor*button = new ImColor(0.5f, 0.5f, 0.5f);
		ImColor*click = new ImColor(0.7f, 0.7f, 0.7f);
		ImColor*line = new ImColor(0.9f, 0.9f, 0.9f);
		*line = ImLerp(*line, *button, 0.8f);

		//判定点击
		if (isAreaHit(slideX, slideY, slideWidth, slideHeight) == 1)
		{
			uistate.hotitem = id;
			if (uistate.activeitem == 0 && uistate.mouseDown)
			{
				ui_lastX = uistate.mouseX;
				uistate.activeitem = id;
			}
			else if (!uistate.mouseDown)
				uistate.activeitem = 0;
		}
		else
			uistate.hotitem = 0;

		int dx = 0;

		if (uistate.hotitem == id)
		{
			if (uistate.activeitem == id)
			{
				dx = uistate.mouseX - ui_lastX;
				if (slideX < x + 4)
					slideX = x + 4;
				else if (slideX > x + width - 34)
					slideX = x + width - 34;
				else
					slideX += dx;
				DrawRect(slideX , slideY, slideWidth, slideHeight, click);
				ui_lastX = uistate.mouseX;
			}
			else
			{
				DrawRect(slideX , slideY, slideWidth, slideHeight, button);
				DrawRect(slideX - 2, slideY - 2, slideWidth + 4, slideHeight + 4, line);
			}
		}
		else
			DrawRect(slideX, slideY, slideWidth, slideHeight, button);

		int mid = slideX + slideWidth / 2;

		DrawRect(x+1, y+1, mid-x,height-2, new ImColor(0.0f, 1.0f, 0.0f));
		DrawRect(x - 1, y - 1, width + 2, height + 2, new ImColor(0.1f, 0.1f, 0.1f));

		return Num*(float)((slideX-x-4) / (barLen-slideWidth-8));
	}

	int ImTextField(int id, int x, int y, int width, int height, std::string str)
	{
		UISentence*sentence = new UISentence(str, new ImColor(0.9f, 0.9f, 0.9f), new ImColor(0.05f, 0.05f, 0.05f));
		DrawTextField(x, y, width, height, sentence);
		return 0;
	}

	int ImSimpleWindowBegin(int id,int x, int y, int width, int height,std::string title)
	{
		int dx=0, dy=0;
		static int lastX = 0, lastY = 0;
		static int winX = x, winY = y;
		if (isAreaHit(x, y, width, 30) == 1)
		{
			uistate.hotitem = id;
			if (uistate.activeitem == 0 && uistate.mouseDown)
			{
				lastX = uistate.mouseX;
				lastY = uistate.mouseY;
				uistate.activeitem = id;
			}
			else if (!uistate.mouseDown)
				uistate.activeitem = 0;
		}
		else
			uistate.hotitem = 0;


			if (uistate.activeitem == id)
			{
				winX += uistate.mouseX - lastX;
				winY += uistate.mouseY - lastY;

				lastX = uistate.mouseX;
				lastY = uistate.mouseY;
			}


		//Title
		UISentence*sentence = new UISentence(title, new ImColor(1.0f, 1.0f, 1.0f), new ImColor(0.0f, 0.0f, 0.0f));
		DrawTextField(winX + 2, winY + 2, width / 2, 26, sentence);
		//Bar
		ImButton(2, winX + width - 100, winY + 3, 24, 24, new ImColor(0.8f, 0.4f, 0.0f));
		ImButton(3, winX + width - 65, winY + 3, 24, 24, new ImColor(0.8f, 0.4f, 0.0f));
		ImButton(4, winX + width - 30, winY + 3, 24, 24, new ImColor(0.8f, 0.0f, 0.0f));
		DrawRect(winX, winY, width, 30, new ImColor(0.08f, 0.08f, 0.08f));

		//Content
		std::string xx, yy;
		imgui::ImFloat2String(xx, winX);
		imgui::ImFloat2String(yy, winY);
		std::string aa = "X:",bb="Y:";
		aa.append(xx);
		bb.append(yy);
		UISentence*sen = new UISentence(aa, new ImColor(1.0f, 1.0f, 1.0f), new ImColor(0.0f, 0.0f, 0.0f));
		DrawTextField(winX + 2, winY + 40, width / 2-20, 30, sen);
		sen=new UISentence(bb, new ImColor(1.0f, 1.0f, 1.0f), new ImColor(0.0f, 0.0f, 0.0f));
		DrawTextField(winX + 7 + width / 2, winY + 40, width/2-20,30,sen);

		//Window
		DrawRect(winX + 2, winY + 32, width - 4, height - 34, new ImColor(0.2f, 0.2f, 0.2f));
		DrawRect(winX, winY + 30, width, height - 30, new ImColor(0.0f, 0.0f, 0.0f));
		
		return 0;
	}


	void ImFloat2String(std::string &str, float src)
	{
		std::ostringstream buffer;
		buffer << src;
		str = buffer.str();
	}

}