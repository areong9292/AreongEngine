#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_texture = nullptr;
}

TextureClass::TextureClass(const TextureClass & other)
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// 디바이스와 파일이름을 가지고 텍스쳐 파일을 로드한다
	result = D3DX11CreateShaderResourceViewFromFile(
											device,
											filename,
											NULL,
											NULL,
											&m_texture,
											NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	return;
}

ID3D11ShaderResourceView * TextureClass::GetTexture()
{
	return m_texture;
}
