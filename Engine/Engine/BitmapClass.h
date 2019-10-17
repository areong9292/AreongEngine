#ifndef  _BITMAPCLASS_H_
#define _BITMAPCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include "TextureClass.h"

// 화면에 그리는데 필요한 각 이미지를 표현하는 클래스
// 모든 2D이미지에 대해 BitmapClass를 만들어야한다
class BitmapClass
{
private:

	// 2D 이미지는 위치 벡터(어디에 그릴건지),
	// 텍스쳐 좌표(텍스쳐 어느 부분을 씌울건지)만 있으면 된다
	struct  VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

	bool Initialize(ID3D11Device * device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	// 버텍스, 인덱스 버퍼 초기화
	bool InitializeBuffers(ID3D11Device* device);

	// 버텍스, 인덱스 버퍼 해제
	void ShutdownBuffers();

	// 버퍼를 봐서 변경사항이 있으면 갱신하고 그린다
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	// 텍스쳐 로드, 해제
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;

	// 화면 크기, 이미지 크기, 이전에 그렸던 위치 기억용
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif // ! _BITMAPCLASS_H_