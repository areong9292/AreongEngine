#include "stdafx.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

bool RenderManager::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	d3dApp = new D3DApp();
	if (d3dApp != nullptr)
	{
		// DirectX11 초기화
		if (!d3dApp->InitializeDirect3d11App(screenWidth, screenHeight, hWnd))
		{
			MessageBox(0, L"Direct3D Initialization - Failed",
				L"Error", MB_OK);
			return false;
		}

		// d3d 씬 초기화
		if (!d3dApp->InitScene())
		{
			MessageBox(0, L"Scene Initialization - Failed",
				L"Error", MB_OK);
			return false;
		}
	}

	return true;
}

bool RenderManager::ReleaseAll()
{
	// 종료 후 메모리 정리
	if (d3dApp != nullptr)
	{
		d3dApp->ReleaseObjects();
	}

	return true;
}

void RenderManager::Render()
{
	if (d3dApp != nullptr)
	{
		d3dApp->UpdateScene();
		d3dApp->DrawScene();
	}
}
