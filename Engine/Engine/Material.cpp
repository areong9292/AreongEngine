#include "Material.h"

Material::~Material()
{
	// 사용한 텍스쳐 객체를 반환한다
	ReleaseTexture();
}

// Component의 생성자를 명시적으로 호출해야한다
// 안그러면 기본 생성자가 호출되는데 삭제한 함수여서 에러 발생함
Material::Material(GameObject* gameObject) : Component(gameObject)
{
	m_ShaderType = ShaderManager::LIGHT;
	m_TextureFilePath = L"../Engine/data/seafloor.dds";

	/*
	bool result;

	// 이 모델에서 사용할 텍스쳐를 로드한다
	result = LoadTexture(device, m_TextureFilePath);
	if (!result)
	{
		return false;
	}*/
}

void Material::onCreate()
{
}

void Material::onDestroy()
{
}

ShaderManager::ShaderType Material::GetShaderType()
{
	return m_ShaderType;
}

void Material::SetShaderType(ShaderManager::ShaderType setShaderType)
{
	m_ShaderType = setShaderType;
}

WCHAR * Material::GetTexturePath()
{
	return m_TextureFilePath;
}

void Material::UpdateComponent()
{
}


// 택스쳐 객체를 생성하고 초기화하는 메소드
bool Material::LoadTexture(ID3D11Device* device)
{
	bool result;

	m_Texutre = new TextureClass;
	if (m_Texutre == nullptr)
	{
		return false;
	}

	result = m_Texutre->Initialize(device, m_TextureFilePath);
	if (!result)
	{
		return false;
	}

	return true;
}

void Material::ReleaseTexture()
{
	if (m_Texutre != nullptr)
	{
		m_Texutre->Shutdown();
		delete m_Texutre;
		m_Texutre = nullptr;
	}

	return;
}

ID3D11ShaderResourceView * Material::GetTexture()
{
	if (m_Texutre != nullptr)
		return m_Texutre->GetTexture();
	else
		return nullptr;
}