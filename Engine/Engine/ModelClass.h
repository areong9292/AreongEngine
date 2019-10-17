#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

// �ؽ�Ʈ ����(�� ����)�� �б� ���� include
#include <fstream>
using namespace std;

// �ؽ��� �ε带 ���� include
#include "TextureClass.h"

// 3D�𵨵��� ������ �����е��� ĸ��ȭ�ϴ� Ŭ����
class ModelClass
{
private:
	// ���� ���ۿ� ����� ������ ����ü ����
	// ���ؽ� ���̴��� �Է°� ���°� ���ƾ��Ѵ�
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;	// �ؽ��ĸ� ���� ���̴�
		D3DXVECTOR3 normal;		// ���� ������ ���� ���� ���� �߰�
	};

	// �� ���� ����ü
	struct ModelType
	{
		float x, y, z;		// ��ġ ��ǥ
		float tu, tv;		// �ؽ��� ��ǥ
		float nx, ny, nz;	// ���� ����
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	// 3D ���� ���ؽ� ����, �ε��� ���۵��� �ʱ�ȭ�� ���� ������ ����
	bool Initialize(ID3D11Device*, char*, WCHAR*);		// �� ���ϸ�, �ؽ��� ���
	void Shutdown();

	// �׷��� ī�忡 �𵨵��� ���� ������ �ְ� �÷� ���̴��� �׸� �غ� �Ѵ�
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	// ���̴����� �ؽ��� ���ҽ��� �����Ѵ�
	ID3D11ShaderResourceView* GetTexture();

private:
	// 3D ���� ���ؽ� ����, �ε��� ���۵��� �ʱ�ȭ�� ���� ������ ����
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	// �׷��� ī�忡 �𵨵��� ���� ������ �ְ� �÷� ���̴��� �׸� �غ� �Ѵ�
	void RenderBuffers(ID3D11DeviceContext*);

	// �ؽ��ĸ� �ҷ��´�
	bool LoadTexture(ID3D11Device*, WCHAR*);

	// �ؽ��ĸ� ��ȯ�Ѵ�
	void ReleaseTexture();

	// �� �����͸� �ҷ��´�
	bool LoadModel(char*);

	// �� �����͸� ��ȯ�Ѵ�
	void ReleaseModel();

private:
	// ���ؽ� ����, �ε��� ����, �� ������ ũ�� ������ ������ �ִ� ����
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texutre;

	// �� ������ �����
	ModelType* m_model;
};

#endif