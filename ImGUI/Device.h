#ifndef DEVICE_H
#define DEVICE_H

#include<D3D11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<xnamath.h>
#include<time.h>
#include<iostream>
#include"Time.h"
#include"Camera.h"
#include"Model.h"
#include"imgui.h"

#define PI 3.141592654f

//Clear Color
const float ClearColor[4] = { 0.2784f,0.2784f,0.2784f,1.0f };


class Device
{
public:
	Device();
	~Device();

public:
	HRESULT Initialize(HINSTANCE hinstance, HWND hwnd,int width,int height,int sx,int sy);
	HRESULT Update();
	void Render();

protected:
	HRESULT InitializeSwapChain();
	HRESULT InitializeRenderTarget(bool NoDepth=false);
	HRESULT InitializeShaderView();
	HRESULT InitializeDepthView();
	HRESULT InitializeZBuffer();
	HRESULT InitializeBlend();
	HRESULT InitializeCullState();
	HRESULT InitializeModel();
	HRESULT InitializeViewPort(int width,int height,int TopX=0,int TopY=0);

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	void TurnBlend(bool on);
	void TureZBuffer(bool on);
	void TrueCullState(int cullstate);

	void RenderBegin();
	void RenderEnd();

protected:
	//Window
	HWND  m_hwnd = NULL;
	int width = 0;
	int height = 0;
	//D3D 
	IDXGISwapChain*m_pSwapChain = NULL;
	ID3D11Device*m_pDevice = NULL;
	ID3D11DeviceContext*m_pImmediateContext = NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11ShaderResourceView*m_pShaderResourceView = NULL;
	ID3D11Texture2D*m_pRenderTexture = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	ID3D11Texture2D*m_pDepthStencil = NULL;
	ID3D11BlendState*m_alphaEnableBlendingState = NULL;
	ID3D11BlendState*m_alphaDisableBlendingState = NULL;
	ID3D11DepthStencilState*m_pDepthEnableState = NULL;
	ID3D11DepthStencilState*m_pDepthDisableState = NULL;
	ID3D11RasterizerState*m_pCullBackState = NULL;
	ID3D11RasterizerState*m_pCullFrontState = NULL;
	ID3D11RasterizerState*m_pCullNoneState = NULL;
	//Matrix
	XMMATRIX  m_World;
	XMMATRIX  m_View;
	XMMATRIX  m_Projection;
	XMMATRIX m_Ortho;
	//Buffer
	UINT verticesCount = 0;
	UINT indicesCount = 0;
	//Model
	Model*looper = NULL;
	//Camera
	Camera*m_camera=NULL;
	//Input
	Input*m_input;
	//UI
	float* clear;
	//Timer
	Time*timer=NULL;
	float deltaTime;
	float FPS;
	//Angel
	float Angel = 0;
	//Bounding Sphere
	float boundingRadius = 0.0f;
	int t_w = 150, t_h = 100;

protected:
	//A parameter to Turn on or Ture off  Blend /Z Buffer
	const bool ON = true;
	//A parameter to Turn on or Ture off  Blend /Z Buffer
	const bool OFF = false;
	//A parameter to change Cull Mode
	// *CULL_MODE  =0  CULL_BACK
	// *CULL_MODE  =1  CULL_FRONT
	// *CULL_MODE  =2  CULL_NONE
	enum CULL_MODE
	{
		CULL_BACK,
		CULL_FRONT,
		CULL_NONE
	};

	//A Parameter for InitializeRenderTarget(bool)
	//if parameter is false, Depth view has bean used
	const bool AddDepth = false;
	const bool NoDepth = true;
};



#endif

