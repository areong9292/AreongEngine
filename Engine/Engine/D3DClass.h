#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

// ��ü ����� ����ϱ� ���� ��ũ�ϴ� ���̺귯���� ���
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

// �ٷ� 10���� �ε��ϴµ� 11�ǵ� �ȹٲ�� ����� �־ �����
#pragma comment(lib, "d3dx10.lib")

// ��ũ�� ���̺귯����, DirectXŸ�� ���ǿ� ���� �͵鿡 ���� ��� �߰�
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include <iostream>
#include <fstream>

// Direct3D ��ɵ��� �ٷ�� Ŭ����
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int , bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	// Direct3D 11 ��ġ ��ü�� �����ϴ� �������̽�(������)
	ID3D11Device* GetDevice();

	// ������ ��ġ�� ����Ͽ� ���ӿ� �ʿ��� ���µ�
	// (����, �ؽ���)�� �����ϰ� �����ϱ� ���� �������̽�(������)
	ID3D11DeviceContext* GetDeviceContext();

	/// ��ĵ� ���ϴ� �޼ҵ�
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

	// Z���۸� �Ѱ� ���� �޼ҵ�
	void TurnZBufferOn();
	void TurnZBufferOff();

	// ���ĺ��� �Ѱ� ���� �޼ҵ�
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:
	bool m_vsync_enabled;							// ��������ȭ ��뿩��

	int m_videoCardMemory;							// ��밡���� �׷���ī�� �޸� �����
	char m_videoCardDescription[128];				// �׷��� ī�� �̸� ��� ��

	IDXGISwapChain* m_swapChain;					// ����ü��

	ID3D11Device* m_device;							// ���̷�Ʈ ��ġ
	ID3D11DeviceContext* m_deviceContext;			// ���̷�Ʈ ��ġ ���ؽ�Ʈ

	ID3D11RenderTargetView* m_renderTargetView;		// ����Ÿ�� ��

	ID3D11Texture2D* m_depthStencilBuffer;			// ����-���ٽ� ����
	ID3D11DepthStencilState* m_depthStencilState;	// ����-���ٽ� ����
	ID3D11DepthStencilView* m_depthStencilView;		// ����-���ٽ� ��

	ID3D11RasterizerState* m_rasterState;			// ������ȭ�� ����

	D3DXMATRIX m_projectionMatrix;					// �������
	D3DXMATRIX m_worldMatrix;						// �������
	D3DXMATRIX m_orthoMatrix;						// �����������

	// �� ����� ī�޶� Ŭ������ �ִ�

	ID3D11DepthStencilState* m_depthDisabledStencilState;	// Z���� �� ����-���ٽ� ����

	// ���� ���º���
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};

#endif