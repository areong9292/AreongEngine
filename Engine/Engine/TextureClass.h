#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <d3dx11tex.h>

// 텍스쳐 자원을 불러오고 해제하고 접근하는 작업
// 모든 텍스쳐에 대해 각각 이 클래스가 만들어져야한다
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& other);
	~TextureClass();

	// 파일 이름으로 텍스쳐를 불러온다
	bool Initialize(ID3D11Device*, WCHAR*);

	// 텍스쳐를 해제한다
	void Shutdown();

	// 텍스쳐 리소스 포인터를 반환
	ID3D11ShaderResourceView* GetTexture();

private:
	// 텍스쳐 리소스 포인터
	ID3D11ShaderResourceView* m_texture;
};

#endif // !_TEXTURECLASS_H_