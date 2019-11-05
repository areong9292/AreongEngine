#include "ModelClass.h"

ModelClass::ModelClass()
{
	// ���ؽ� ���ۿ� �ε��� ������ �����͸� null�� �ʱ�ȭ
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;

	// �� ������ �ʱ�ȭ
	m_model = nullptr;

	m_Material = nullptr;
}

ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

// ���ؽ� ���ۿ� �ε��� ������ �ʱ�ȭ �Լ��� ȣ���Ѵ�
// �� ������ �̸��� ���ڷ� �Ͽ� ���� �ε��Ѵ�
// �ؽ��� ������ �̸��� ���ڷ� �Ͽ� �ؽ��ĸ� �ε��Ѵ�
bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;
	m_Device = device;

	// �� �����͸� �о�ͼ� ���ؽ�, �ε��� �迭�� ä�� ��(LoadModel)
	// ���ؽ�, �ε��� ���۸� �����Ѵ�(InitializeBuffers)
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// ���ؽ� ���ۿ� �ε��� ���۸� �ʱ�ȭ�Ѵ�
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	m_Material = new GameObject();
	m_Material->addComponent<Material>();

	result = m_Material->getComponent<Material>()->LoadTexture(device);
	if (!result)
	{
		return false;
	}

	return true;
}

// ���ؽ� ���ۿ� �ε��� ���۸� �����ϴ� �޼ҵ带 ȣ��
void ModelClass::Shutdown()
{
	// ����� ���ؽ�, �ε��� ���۸� ��ȯ�Ѵ�
	ShutdownBuffers();

	// ����� �� ��ü�� ��ȯ�Ѵ�
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// ���ؽ� ���ۿ� �ε��� ���۸� �׷��� ���������ο� �ִ� �޼ҵ� ȣ��
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

Material * ModelClass::GetModelMaterial()
{
	return m_Material->getComponent<Material>();
}

void ModelClass::test(vector<FBXImporter::MyVertex> test)
{
	int count = 0;
	int countEnd = 0;
	if (test.size() >= m_vertexCount / 3)
	{
		count = m_vertexCount / 3;
		countEnd = test.size();
	}
	else
	{
		count = test.size();
		countEnd = m_vertexCount / 3;
	}
	for (int i = 0; i < count; i++)
	{
		m_model[i].x = test[i].pos[0];
		m_model[i].y = test[i].pos[1];
		m_model[i].z = test[i].pos[2];
		//fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
	}
	/*for (int i = count; i < countEnd; i++)
	{
		m_model[i].x = 0;
		m_model[i].y = 0;
		m_model[i].z = 0;
		//fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
	}*/

	// ���ؽ� ���ۿ� �ε��� ���۸� �ʱ�ȭ�Ѵ�
	bool result = InitializeBuffers(m_Device);
	if (!result)
	{
		//return false;
	}
}

// ���ؽ� ���ۿ� �ε��� ���۸� �����Ѵ�
// ������ ���Ϸκ��� ���� ������ �о�ͼ� ���۸� �����
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;		// ���� ���ؽ���
	unsigned long* indices;		// ���� ���ؽ��� �ε�����

	// ���ؽ�, �ε��� ������ desc
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;

	// ���ؽ�, �ε��� ������
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	int i;

	// ���ؽ�, �ε��� �迭�� ũ��(m_vertexCount, m_indexCount)��
	// LoadModel���� �� ������ �°� �����ȴ�

	// ���ؽ� �迭�� �����Ѵ�
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// �ε��� �迭�� �����Ѵ�
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ���ؽ� �迭�� ���� ä���
	// ���� ������ ä���
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// ä�� �����ͷ� ���ؽ� ���ۿ� �ε��� ���۸� �����Ѵ�
	// 1. ���ۿ� ���� desc�� �ۼ��Ѵ�
	// 2. ������ ä�� �迭�� subresource������(D3D11_SUBRESOURCE_DATA)�� �����Ѵ�
	// 3. device�� CreateBuffer �޼ҵ带 ȣ���Ͽ� ������ �����͸� �޾ƿ´�

	// 1. ���ؽ� ������ description�� �ۼ�
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 2. ���ؽ� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 3. ���ؽ� ���۸� ����
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 1. �ε��� ������ description�� �ۼ�
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	// 2. �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	// 3. �ε��� ���۸� ����
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// ���� ���� �� ���ؽ�, �ε��� �迭 �޸𸮸� �����Ѵ�
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// �ε��� ���۸� ����
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// ���ؽ� ���۸� ����
	if (m_vertexBuffer != nullptr)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

// ���ؽ� ���ۿ� �ε��� ���۸� GPU�� Input assembler�� ���۷μ� Ȱ��ȭ ��Ų��
// -> ���ؽ� ���ۿ� �ε��� ���۸� �׷��� ���������ο� �ִ´�

// ���� ���ϴ� �׷��� ������������ �Է� ������ �ܰ��
// ���������ο� ���ҽ��� �����͸� �����Ѵ�
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// ���ؽ� ������ ������ �������� �����Ѵ�
	stride = sizeof(VertexType);
	offset = 0;

	// Input assembler�� ���ؽ�,�ε��� ���۸� Ȱ��ȭ �Ͽ� �׷��� �� �ְ� �Ѵ�
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���ؽ� ���۷� �׸� �⺻���� �����Ѵ�(�ﰢ��)
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// ���ؽ��� �� ������
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin >> m_vertexCount;

	// �ε����� ���� ���ؽ� ���� ���� ����
	m_indexCount = m_vertexCount;

	// �� ������ �ʱ�ȭ
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// ���� �� ������ �޾ƿ�
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);

	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model != nullptr)
	{
		delete[] m_model;
		m_model = nullptr;
	}

	return;
}
