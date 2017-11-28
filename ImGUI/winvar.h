#pragma once



namespace WINVAR
{

	void InitVar();
	int GetHeight();
	void SetHeight(int h);
	int GetWidth();
	void SetWidth(int w);
	int GetStartX();
	void SetStartX(int x);
	int GetStartY();
	void SetStartY(int y);
	void GetVars(int &w, int &h, int &x, int &y);
	void SetVars(int w, int h, int x, int y);

}