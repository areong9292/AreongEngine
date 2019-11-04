#include "ShaderManager.h"
#include "Material.h"
#include "ModelClass.h"

ShaderManager::ShaderManager()
{
	m_Device = nullptr;
	m_DeviceContext = nullptr;
	m_Hwnd = nullptr;
	m_MainCamera = nullptr;
	m_MainLight = nullptr;
}

ShaderManager::ShaderManager(const ShaderManager & other)
{
}

ShaderManager::~ShaderManager()
{
	Shutdown();
}

bool ShaderManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, CameraClass* mainCamera, LightClass* mainLight)
{
	m_Device = device;
	m_DeviceContext = deviceContext;
	m_Hwnd = hwnd;
	m_MainCamera = mainCamera;
	m_MainLight = mainLight;

	m_ColorShader = nullptr;
	m_FontShader = nullptr;
	m_LightShader = nullptr;
	m_TextureShader = nullptr;

	return true;
}

void ShaderManager::Shutdown()
{
	// ���̴� ��ü ��ȯ
	if (m_ColorShader != nullptr)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = nullptr;
	}

	if (m_FontShader != nullptr)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = nullptr;
	}

	if (m_LightShader != nullptr)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
	}

	if (m_TextureShader != nullptr)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

}


bool ShaderManager::Render(ModelClass* model, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (model == nullptr)
	{
		return false;
	}

	Material* modelMaterial = model->GetModelMaterial();
	if (modelMaterial == nullptr)
	{
		return false;
	}

	bool result;
	ShaderType modelShaderType = modelMaterial->GetShaderType();
	switch (modelShaderType)
	{
	case ShaderManager::LIGHT:
		if (m_LightShader == nullptr)
		{
			m_LightShader = new LightShaderClass;
			if (m_LightShader == nullptr)
			{
				return false;
			}

			result = m_LightShader->Initialize(m_Device, m_Hwnd);
			if (!result)
			{
				return false;
			}
		}

		// ���̴��� �׸���
		result = m_LightShader->Render(
			m_DeviceContext,
			model->GetIndexCount(),
			worldMatrix, viewMatrix, projectionMatrix,		// ����, ��, ���� ���
			modelMaterial->GetTexture(),					// ���� ���׸����� �ؽ���
			m_MainLight->GetDirection(),					// ������ ����
			m_MainLight->GetAmbientColor(),					// �ֺ����� ��
			m_MainLight->GetDiffuseColor(),					// ������ ��

			// �ݻ籤
			m_MainCamera->GetPosition(),					// ī�޶��� ��ġ
			m_MainLight->GetSpecularColor(),				// �ݻ籤�� ��
			m_MainLight->GetSpecularPower());				// �ݻ籤�� ����
		//m_LightShader->Render(m_Device,)

		break;
	case ShaderManager::TEXTURE:

		if (m_TextureShader == nullptr)
		{
			m_TextureShader = new TextureShaderClass;
			if (m_TextureShader == nullptr)
			{
				return false;
			}

			result = m_TextureShader->Initialize(m_Device, m_Hwnd);
			if (!result)
			{
				return false;
			}
		}

		result = m_TextureShader->Render(m_DeviceContext, model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, modelMaterial->GetTexture());

		break;
	case ShaderManager::COLOR:
		break;
	case ShaderManager::FONT:
		break;
	default:
		break;
	}

	return true;
}

char* ShaderManager::GetShaderType(ShaderType shaderType)
{
	switch (shaderType)
	{
	case ShaderManager::LIGHT:
		return "LightShader";
		break;
	case ShaderManager::TEXTURE:
		return "TextureShader";
		break;
	case ShaderManager::COLOR:
		return "ColorShader";
		break;
	case ShaderManager::FONT:
		return "FontShader";
		break;
	default:
		break;
	}
	return nullptr;
}
