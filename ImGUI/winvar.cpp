#include"winvar.h"


namespace WINVAR
{
	int winHeight;
	int winWidth;
	int winStartX;
	int winStartY;

	void InitVar()
	{
		winHeight = winWidth = winStartX = winStartY = 0;
	}
	int GetHeight()
	{
		return winHeight;
	}
	void SetHeight(int h)
	{
		winHeight = h;
	}
	int GetWidth()
	{
		return winWidth;
	}
	void SetWidth(int w)
	{
		winWidth = w;
	}
	int GetStartX()
	{
		return winStartX;
	}
	void SetStartX(int x)
	{
		winStartX = x;
	}
	int GetStartY()
	{
		return winStartY;
	}
	void SetStartY(int y)
	{
		winStartY = y;
	}
	void GetVars(int &w, int &h, int &x, int &y)
	{
		w = winWidth;
		h = winHeight;
		x = winStartX;
		y = winStartY;
	}
	void SetVars(int w, int h, int x, int y)
	{
		winWidth = w;
		winHeight = h;
		winStartX = x;
		winStartY = y;
	}

}