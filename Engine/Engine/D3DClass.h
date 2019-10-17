#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

// 객체 모듈을 사용하기 위해 링크하는 라이브러리들 명시
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

// 다렉 10꺼도 로드하는데 11되도 안바뀌는 기능이 있어서 사용함
#pragma comment(lib, "d3dx10.lib")

// 링크한 라이브러리들, DirectX타입 정의와 같은 것들에 대한 헤더 추가
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include <iostream>
#include <fstream>

// Direct3D 기능들을 다루는 클래스
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

	// Direct3D 11 장치 객체를 저장하는 인터페이스(포인터)
	ID3D11Device* GetDevice();

	// 생성된 장치를 사용하여 게임에 필요한 에셋들
	// (버퍼, 텍스쳐)을 제어하고 관리하기 위한 인터페이스(포인터)
	ID3D11DeviceContext* GetDeviceContext();

	/// 행렬들 구하는 메소드
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

	// Z버퍼를 켜고 끄는 메소드
	void TurnZBufferOn();
	void TurnZBufferOff();

	// 알파블렌딩 켜고 끄는 메소드
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:
	bool m_vsync_enabled;							// 수직동기화 사용여부

	int m_videoCardMemory;							// 사용가능한 그래픽카드 메모리 저장용
	char m_videoCardDescription[128];				// 그래픽 카드 이름 담는 용

	IDXGISwapChain* m_swapChain;					// 스왑체인

	ID3D11Device* m_device;							// 다이렉트 장치
	ID3D11DeviceContext* m_deviceContext;			// 다이렉트 장치 컨텍스트

	ID3D11RenderTargetView* m_renderTargetView;		// 렌더타겟 뷰

	ID3D11Texture2D* m_depthStencilBuffer;			// 깊이-스텐실 버퍼
	ID3D11DepthStencilState* m_depthStencilState;	// 깊이-스텐실 상태
	ID3D11DepthStencilView* m_depthStencilView;		// 깊이-스텐실 뷰

	ID3D11RasterizerState* m_rasterState;			// 레스터화기 상태

	D3DXMATRIX m_projectionMatrix;					// 투영행렬
	D3DXMATRIX m_worldMatrix;						// 월드행렬
	D3DXMATRIX m_orthoMatrix;						// 직교투영행렬

	// 뷰 행렬은 카메라 클래스에 있다

	ID3D11DepthStencilState* m_depthDisabledStencilState;	// Z버퍼 끈 깊이-스텐실 상태

	// 블렌딩 상태변수
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};

#endif