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
		// DirectX11 �ʱ�ȭ
		if (!d3dApp->InitializeDirect3d11App(screenWidth, screenHeight, hWnd))
		{
			MessageBox(0, L"Direct3D Initialization - Failed",
				L"Error", MB_OK);
			return false;
		}

		// d3d �� �ʱ�ȭ
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
	// ���� �� �޸� ����
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
