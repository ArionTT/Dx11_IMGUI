#include "D3Device_Instan.h"



D3Device_Instan::D3Device_Instan()
{
	m_pDevice = NULL;
	m_pDeviceContext = NULL;
	m_pSwapChain = NULL;
	m_pRenderTargetView = NULL;
}

D3Device_Instan::~D3Device_Instan()
{
}

bool D3Device_Instan::Init(HWND hwnd,int width,int height)
{
	this->hwnd = hwnd;
	bool hr = true;
	hr = InitDevice(width, height);
	if (!hr)
		return hr;

	hr = InitImGUI();
	if (!hr)
		return hr;

	return true;
}

void D3Device_Instan::BeginRender()
{
	float ClearColor[] = { 0.35f,0.35f,0.35f,1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
}

void D3Device_Instan::Frame()
{
	//Frame Rendering Here
}

void D3Device_Instan::EndRender()
{
	m_pSwapChain->Present(0, 0);
}

void D3Device_Instan::Shutdown()
{
	CleanRenderTarget();
	CleanD3D11Device();
}

ID3D11Device*D3Device_Instan::GetDevice()
{
	return m_pDevice;
}

IDXGISwapChain*D3Device_Instan::GetSwapChain()
{
	return m_pSwapChain;
}

void D3Device_Instan::CleanRenderTarget()
{
	if (m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = NULL;
	}
}

void D3Device_Instan::CleanD3D11Device()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	if (m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	if (m_pDeviceContext)
	{
		m_pDeviceContext->Release();
		m_pDeviceContext = NULL;
	}
}

bool D3Device_Instan::InitDevice(int w,int h)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	int wheight = rc.bottom - rc.top;
	int wwidth = rc.right - rc.left;
	//SwapChain
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 2;
	desc.BufferDesc.Height = wheight;
	desc.BufferDesc.Width = wwidth;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Windowed = true;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	D3D_DRIVER_TYPE driver = D3D_DRIVER_TYPE_HARDWARE;
	UINT flag = 0;
#ifdef _DEBUG
	flag |= D3D10_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	D3D_FEATURE_LEVEL feature;
	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	hr = D3D11CreateDeviceAndSwapChain(NULL, driver, NULL, flag, &featureLevel, 1,
		D3D11_SDK_VERSION, &desc, &m_pSwapChain, &m_pDevice, &feature, &m_pDeviceContext);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"SwapChain And Device Create Failed", L"Error", MB_OK);
		return false;
	}

	//Create Render Target
	ID3D11Texture2D*pTextureBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTextureBuffer);
	D3D11_RENDER_TARGET_VIEW_DESC rtvds;
	ZeroMemory(&rtvds, sizeof(rtvds));
	rtvds.Format = desc.BufferDesc.Format;
	rtvds.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_pDevice->CreateRenderTargetView(pTextureBuffer, &rtvds, &m_pRenderTargetView);
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	pTextureBuffer->Release();
	return true;
}

bool D3Device_Instan::InitImGUI()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&m_TicksPerSecond))
		return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&m_Time))
		return false;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	io.ImeWindowHandle = hwnd;

	return true;
}

IMGUI_API LRESULT D3Device_Instan::ImGui_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		return true;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return true;
	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		return true;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return true;
	case WM_MBUTTONDOWN:
		io.MouseDown[2] = true;
		return true;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		return true;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return true;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		return true;
	case WM_KEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return true;
	case WM_KEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return true;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return true;
	}
	return 0;
}
