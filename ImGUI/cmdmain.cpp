#include<Windows.h>
#include<iostream>
#include"Device.h"
#include"winvar.h"

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

int main()
{
	Device*device = new Device();
	WINVAR::SetVars(1000, 600, 300, 60);
	int width = WINVAR::GetWidth();
	int height = WINVAR::GetHeight();
	int sx = WINVAR::GetStartX();
	int sy = WINVAR::GetStartY();
	HINSTANCE hInstance;
	hInstance = GetModuleHandle(NULL);
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.lpszMenuName = NULL;
	wndcls.lpfnWndProc = WinProc;
	wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndcls.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wndcls.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndcls.lpszClassName = L"CMDWin";
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	wndcls.hInstance = hInstance;

	RegisterClass(&wndcls);

	HWND hwnd = CreateWindow(
		L"CMDWin",L"命令行窗口 --创建 windows 窗口",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		sx, sy, width, height,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd,SW_SHOW);
	UpdateWindow(hwnd);

	device->Initialize(hInstance, hwnd, width, height,sx,sy);

	MSG msg;
	bool done=false;
	ZeroMemory(&msg, sizeof(MSG));

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (WM_QUIT == msg.message)
			done = true;
		else
		{
			device->Update();
			device->Render();
		}
	}
	return 0;
}