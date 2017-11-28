#pragma once

#include"ImGUI\imgui.h"
#include<D3D11.h>
#include<D3DX11.h>
#include <dinput.h>
#include <tchar.h>
#include<iostream>

class D3Device_Instan
{
public:
	D3Device_Instan();
	~D3Device_Instan();
	bool Init(HWND hwnd,int width,int height);
	void BeginRender();
	void Frame();
	void EndRender();
	void Shutdown();
	ID3D11Device*GetDevice();
	IDXGISwapChain*GetSwapChain();
	IMGUI_API LRESULT ImGui_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitDevice(int w,int h);
	bool InitImGUI();


protected:
	void CleanRenderTarget();
	void CleanD3D11Device();

protected:
	HWND hwnd;
	ID3D11Device*m_pDevice;
	ID3D11DeviceContext*m_pDeviceContext;
	IDXGISwapChain*m_pSwapChain;
	ID3D11RenderTargetView*m_pRenderTargetView;

	float m_TicksPerSecond;
	float m_Time;
};

