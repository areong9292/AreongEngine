#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;

#include "TextureClass.h"


// �۲� �ؽ���, �ؽ�Ʈ ���Ϸκ��� �۲� �����͸� �д´�
// �� �����ͷκ��� ���ؽ� ���۸� �����Ѵ�
class FontClass
{
private:
	// �۲� �ε��� �����͸� �����ϱ� ����
	struct FontType
	{
		float left, right;	// �ؽ����� UV��Ǧ
		int size;			// ������ �ȼ� �ʺ�
	};

	// ���ڸ� �ﰢ�� 2���� �׸��� - 2D �ؽ��� �׸�����
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	// ���ڿ��� �Է¹޾� �ش� ���ڿ��� �׸� �ﰢ������
	// ���ؽ� �迭�� �����
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

private:
	bool LoadFontData(char* filename);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};


#endif // !_FONTCLASS_H_
