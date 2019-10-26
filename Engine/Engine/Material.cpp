#include "Material.h"

Material::~Material()
{
	// ����� �ؽ��� ��ü�� ��ȯ�Ѵ�
	ReleaseTexture();
}

// Component�� �����ڸ� ��������� ȣ���ؾ��Ѵ�
// �ȱ׷��� �⺻ �����ڰ� ȣ��Ǵµ� ������ �Լ����� ���� �߻���
Material::Material(GameObject* gameObject) : Component(gameObject)
{
	m_ShaderType = ShaderManager::LIGHT;
	m_TextureFilePath = L"../Engine/data/seafloor.dds";

	/*
	bool result;

	// �� �𵨿��� ����� �ؽ��ĸ� �ε��Ѵ�
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


// �ý��� ��ü�� �����ϰ� �ʱ�ȭ�ϴ� �޼ҵ�
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