#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include "stdio.h"
#include "DirectXTex.h"
using namespace std;
using namespace DirectX;

// �ؽ��� �ڿ��� �ҷ����� �����ϰ� �����ϴ� �۾�
// ��� �ؽ��Ŀ� ���� ���� �� Ŭ������ ����������Ѵ�
class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

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
	ScratchImage LoadTextureFromFile(WCHAR*);

private:
	// �ؽ��� ���ҽ� ������
	ID3D11ShaderResourceView* m_textureView;
	ID3D11Texture2D* m_texture;
	unsigned char* m_targaData;
};

#endif // !_TEXTURECLASS_H_