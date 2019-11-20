#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

// �ؽ�Ʈ ����(�� ����)�� �б� ���� include
#include <fstream>

// ���� ���׸���
#include "Material.h"
#include "DesignPatternHeader.h"

// FBXimporter
#include "FBXImporter.h"

using namespace std;
using namespace DirectX;

// 3D�𵨵��� ������ �����е��� ĸ��ȭ�ϴ� Ŭ����
class ModelClass
{
private:
	// ���� ���ۿ� ����� ������ ����ü ����
	// ���ؽ� ���̴��� �Է°� ���°� ���ƾ��Ѵ�
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;	// �ؽ��ĸ� ���� ���̴�
		XMFLOAT3 normal;	// ���� ������ ���� ���� ���� �߰�
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

	Material* GetModelMaterial();

	void test(vector<FBXImporter::MyVertex> test);

private:
	// 3D ���� ���ؽ� ����, �ε��� ���۵��� �ʱ�ȭ�� ���� ������ ����
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	// �׷��� ī�忡 �𵨵��� ���� ������ �ְ� �÷� ���̴��� �׸� �غ� �Ѵ�
	void RenderBuffers(ID3D11DeviceContext*);

	// �� �����͸� �ҷ��´�
	bool LoadModel(char*);

	// �� �����͸� ��ȯ�Ѵ�
	void ReleaseModel();

private:
	// ���ؽ� ����, �ε��� ����, �� ������ ũ�� ������ ������ �ִ� ����
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	// �� ������ �����
	ModelType* m_model;

	// ���� ���׸��� ������Ʈ
	GameObject* m_Material;

	ID3D11Device* m_Device;
};

#endif