#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include <d3d11.h>
#include <DirectXMath.h>

// ������ ���� ���� include
#include "DesignPatternHeader.h"

// ��� ���̴� Ŭ���� include
#include "../Engine/ShaderClass/ColorShaderClass.h"
#include "../Engine/ShaderClass/FontShaderClass.h"
#include "../Engine/ShaderClass/LightShaderClass.h"
#include "../Engine/ShaderClass/TextureShaderClass.h"


// ī�޶�, ����Ʈ Ŭ���� include
#include "CameraClass.h"
#include "LightClass.h"

using namespace DirectX;

// ���̴��� ����� ��� ������ �����ϴ� Ŭ����
// �̱������� �����Ǹ� ī�޶�� ����Ʈ Ŭ������ �����͸� ������ �ִ�
// ���ڷ� ���� ���׸����� ������ ���� ���������ش�
class Material;
class ModelClass;
class Mesh;
class ShaderManager : public Singleton<ShaderManager>
{
public:
	enum ShaderType
	{
		LIGHT,
		TEXTURE,
		COLOR,
		FONT,
		ShaderTypeCount = 4
	};

	ShaderManager();
	ShaderManager(const ShaderManager& other);
	~ShaderManager();

	bool ShaderManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, CameraClass* mainCamera, LightClass* mainLight);
	void Shutdown();
	bool Render(ModelClass* model, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	bool RenderMesh(Mesh mesh, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

	char* GetShaderType(ShaderType shaderType);

private:
	// �ܼ� �����ͷ� ���
	// ShaderManager���� �����ϸ� �ȵȴ�
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	HWND m_Hwnd;
	CameraClass* m_MainCamera;
	LightClass* m_MainLight;

	// ���̴� Ŭ����
	ColorShaderClass* m_ColorShader;
	FontShaderClass* m_FontShader;
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;

public:
	int shaderCount = ShaderTypeCount;
};

#endif // !_SHADERMANAGER_H_