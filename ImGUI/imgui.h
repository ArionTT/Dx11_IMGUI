#ifndef IMGUI_H
#define IMGUI_H

#include<string>
#include<D3D11.h>
#include<D3DX11.h>
#include<D3Dcompiler.h>
#include<xnamath.h>
#include <sstream>
#include <iostream>
#include"Time.h"
#include"Camera.h"

#define CHAR_WIDTH (int)32
#define CHAR_HEIGHT (int)32
#define IMG_WIDTH (float)512
#define IMG_HEIGHT (float)256
#define MAX_TEXT_LENGTH (int)32


namespace imgui
{
	//Declare Struct
	struct UIVertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};
	struct UIConstBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMFLOAT4 bgColor;
	};
	struct UITextConstBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMFLOAT4 textColor;
		XMFLOAT4 bgColor;
	};
	struct ImColor
	{
		ImColor()
		{
			r = g = b = a=1.0f;
		}
		ImColor(float r, float g = 1.0f, float b = 1.0f,float a=1.0f)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		/*ImColor(ImRGB color)
		{
		r = color.r / 255.0f;
		g = color.g / 255.0f;
		b = color.b / 255.0f;
		}*/
		ImColor(const ImColor&color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;
		}
		float r;
		float g;
		float b;
		float a;
	};
	struct UIState
	{
		int mouseX;
		int mouseY;
		int mouseDown;

		int hotitem;
		int activeitem;
	};
	struct UIChar
	{
		int x;
		int y;
		int ex;
		int ey;
		float uvSX;
		float uvSY;
		float uvEX;
		float uvEY;
	};
	struct UISentence
	{
		UISentence(std::string str, ImColor*text, ImColor*background)
		{
			strLen = str.length();
			textColor = new ImColor();
			bgColor = new ImColor();
			this->str = str;
			textColor = text;
			bgColor = background;
		}
		~UISentence()
		{
			delete textColor;
			delete bgColor;
		}
		std::string str;
		int strLen;
		ImColor*textColor;
		ImColor*bgColor;
		int vertexCount;
		int indexCount;
		int maxLength;
	};


	HRESULT Initialize(ID3D11Device*device, ID3D11DeviceContext*context, HWND hwnd, Camera*camera, Input*input, int winWidth, int winHeight, int sx, int sy);
	HRESULT UICompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitRect();
	HRESULT InitTextRect();
	bool UpdateBuffer(int x, int y, int w, int h, ImColor *color);
	bool UpdateText(int x, int y, int w, int h, UISentence* sentence);
	void DrawRect(int x, int y, int width, int height, ImColor*color);
	int isAreaHit(int x, int y, int width, int height);
	int ImButton(int id, int x, int y, int width, int height, ImColor*button = new ImColor(0.0f, 0.3f, 0.6f));
	int ImLabel(int x, int y, int width, int height, ImColor*color);
	float ImSliderVertical(int id, int x, int y, int width, int height, float start = 0.0f, float end = 1.0f);
	float ImSliderHorizon(int id, int x, int y, int width, int height, float start = 0.0f, float end = 1.0f);
	int ImTextField(int id, int x, int y, int width, int height, std::string str = "HelloWorld>,.<123[]{};:");

	int ImSimpleWindowBegin(int id, int x, int y, int width, int height, std::string title);

	void ImFloat2String(std::string &str, float src);
}


#endif
