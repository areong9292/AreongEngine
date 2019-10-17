#ifndef  _BITMAPCLASS_H_
#define _BITMAPCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include "TextureClass.h"

// ȭ�鿡 �׸��µ� �ʿ��� �� �̹����� ǥ���ϴ� Ŭ����
// ��� 2D�̹����� ���� BitmapClass�� �������Ѵ�
class BitmapClass
{
private:

	// 2D �̹����� ��ġ ����(��� �׸�����),
	// �ؽ��� ��ǥ(�ؽ��� ��� �κ��� �������)�� ������ �ȴ�
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
	// ���ؽ�, �ε��� ���� �ʱ�ȭ
	bool InitializeBuffers(ID3D11Device* device);

	// ���ؽ�, �ε��� ���� ����
	void ShutdownBuffers();

	// ���۸� ���� ��������� ������ �����ϰ� �׸���
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	// �ؽ��� �ε�, ����
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;

	// ȭ�� ũ��, �̹��� ũ��, ������ �׷ȴ� ��ġ ����
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif // ! _BITMAPCLASS_H_