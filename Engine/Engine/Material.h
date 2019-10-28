#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <d3d11.h>

#include "DesignPatternHeader.h"
#include "ShaderManager.h"

// 텍스쳐 로드를 위한 include
#include "TextureClass.h"

class Material : public Component
{
public:
	Material(GameObject* gameObject);
	virtual ~Material();

	virtual void onCreate();
	virtual void onDestroy();

	// 쉐이더 타입 셋팅
	ShaderManager::ShaderType GetShaderType();
	void SetShaderType(ShaderManager::ShaderType setShaderType);

	void SetTexture(ID3D11Device* device, WCHAR * textureFilePath);

	WCHAR* GetTexturePath();

	// 텍스쳐를 불러온다
	bool LoadTexture(ID3D11Device* device);

	ID3D11ShaderResourceView* GetTexture();

private:
	virtual void UpdateComponent();
	ShaderManager::ShaderType m_ShaderType;

	WCHAR* m_TextureFilePath;

	// 텍스쳐를 반환한다
	void ReleaseTexture();

	TextureClass* m_Texutre;
};

#endif // !_MATERIAL_H_