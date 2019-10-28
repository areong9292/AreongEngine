#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <d3d11.h>

#include "DesignPatternHeader.h"
#include "ShaderManager.h"

// �ؽ��� �ε带 ���� include
#include "TextureClass.h"

class Material : public Component
{
public:
	Material(GameObject* gameObject);
	virtual ~Material();

	virtual void onCreate();
	virtual void onDestroy();

	// ���̴� Ÿ�� ����
	ShaderManager::ShaderType GetShaderType();
	void SetShaderType(ShaderManager::ShaderType setShaderType);

	void SetTexture(ID3D11Device* device, WCHAR * textureFilePath);

	WCHAR* GetTexturePath();

	// �ؽ��ĸ� �ҷ��´�
	bool LoadTexture(ID3D11Device* device);

	ID3D11ShaderResourceView* GetTexture();

private:
	virtual void UpdateComponent();
	ShaderManager::ShaderType m_ShaderType;

	WCHAR* m_TextureFilePath;

	// �ؽ��ĸ� ��ȯ�Ѵ�
	void ReleaseTexture();

	TextureClass* m_Texutre;
};

#endif // !_MATERIAL_H_