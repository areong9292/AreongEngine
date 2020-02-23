#pragma once
#include "D3DApp.h"

// 모든 렌더링을 관리하는 매니저
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