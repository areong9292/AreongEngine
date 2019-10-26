#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include <d3d11.h>

// 디자인 패턴 관련 include
#include "DesignPatternHeader.h"

// 모든 쉐이더 클래스 include
#include "../Engine/ShaderClass/ColorShaderClass.h"
#include "../Engine/ShaderClass/FontShaderClass.h"
#include "../Engine/ShaderClass/LightShaderClass.h"
#include "../Engine/ShaderClass/TextureShaderClass.h"


// 카메라, 라이트 클래스 include
#include "CameraClass.h"
#include "LightClass.h"


// 쉐이더를 사용한 모든 렌더링 관리하는 클래스
// 싱글톤으로 생성되며 카메라와 라이트 클래스의 포인터를 가지고 있다
// 인자로 받은 머테리얼의 정보에 맞춰 렌더링해준다
class Material;
class ModelClass;
class ShaderManager : public Singleton<ShaderManager>
{
public:
	enum ShaderType
	{
		LIGHT,
		TEXTURE,
		COLOR,
		FONT
	};

	ShaderManager();
	ShaderManager(const ShaderManager& other);
	~ShaderManager();

	bool ShaderManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, CameraClass* mainCamera, LightClass* mainLight);
	void Shutdown();
	bool Render(ModelClass* model, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);


private:
	// 단순 포인터로 사용
	// ShaderManager에서 삭제하면 안된다
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	HWND m_Hwnd;
	CameraClass* m_MainCamera;
	LightClass* m_MainLight;

	// 쉐이더 클래스
	ColorShaderClass* m_ColorShader;
	FontShaderClass* m_FontShader;
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
};

#endif // !_SHADERMANAGER_H_