#pragma once
#include "D3DApp.h"

// ��� �������� �����ϴ� �Ŵ���
class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	bool Init(int screenWidth, int screenHeight, HWND hWnd);
	bool ReleaseAll();

	void Render();

private:
	D3DApp* d3dApp;
};