#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <d3dx11tex.h>

// �ؽ��� �ڿ��� �ҷ����� �����ϰ� �����ϴ� �۾�
// ��� �ؽ��Ŀ� ���� ���� �� Ŭ������ ����������Ѵ�
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	// ���� �̸����� �ؽ��ĸ� �ҷ��´�
	bool Initialize(ID3D11Device*, WCHAR*);

	// �ؽ��ĸ� �����Ѵ�
	void Shutdown();

	// �ؽ��� ���ҽ� �����͸� ��ȯ
	ID3D11ShaderResourceView* GetTexture();

private:
	// �ؽ��� ���ҽ� ������
	ID3D11ShaderResourceView* m_texture;
};

#endif // !_TEXTURECLASS_H_