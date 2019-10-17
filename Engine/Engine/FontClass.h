#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;

#include "TextureClass.h"


// 글꼴 텍스쳐, 텍스트 파일로부터 글꼴 데이터를 읽는다
// 이 데이터로부터 버텍스 버퍼를 생성한다
class FontClass
{
private:
	// 글꼴 인덱스 데이터를 저장하기 위함
	struct FontType
	{
		float left, right;	// 텍스쳐의 UV좌푠
		int size;			// 문자의 픽셀 너비
	};

	// 문자를 삼각형 2개로 그린다 - 2D 텍스쳐 그리듯이
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

	// 문자열을 입력받아 해당 문자열을 그릴 삼각형들의
	// 버텍스 배열을 만든다
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
