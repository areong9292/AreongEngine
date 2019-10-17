#include "BitmapClass.h"



BitmapClass::BitmapClass()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_Texture = nullptr;
}

BitmapClass::BitmapClass(const BitmapClass& other)
{
}


BitmapClass::~BitmapClass()
{
}

// ��Ʈ�� Ŭ������ �ʱ�ȭ�Ѵ�
bool BitmapClass::Initialize(ID3D11Device * device,
	int screenWidth, int screenHeight,
	WCHAR* textureFilename,
	int bitmapWidth, int bitmapHeight)
{
	bool result;

	// ȭ�� ũ��� �̹��� ũ�� ����
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// ���� ������ ��ġ�� -1�� �ʱ�ȭ
	// �׸� ���� ������ -1�� �Ѵ�
	m_previousPosX = -1;
	m_previousPosY = -1;

	// ���۸� �����ϰ� �ʱ�ȭ�Ѵ�
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// �ؽ��ĸ� �ε��Ѵ�
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void BitmapClass::Shutdown()
{
	// �ؽ��ĸ� �����Ѵ�
	ReleaseTexture();

	// ���ؽ�, �ε��� ���۸� �����Ѵ�
	ShutdownBuffers();
	return;
}

// 2d �̹����� ���۸� �׷��� ī�忡 �ִ´�
bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result;

	// ���� �����Ӱ� ��ġ�� ���ؼ� �ٲ���� ���� ���ؽ� ������
	// ���ؽ����� ���ο� ��ġ�� �����Ѵ�
	result = UpdateBuffers(deviceContext, positionX, positionY);
	if (!result)
	{
		return false;
	}

	RenderBuffers(deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;		// ���ؽ� �迭
	unsigned long* indices;		// �ε��� �迭

	// ���ؽ�, �ε��� ������ desc
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;

	// ���ؽ�, �ε��� ������
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;
	int i;

	// ���ؽ� �� ���� - 2�� �ﰢ������ �簢�� �������Ѵ�
	m_vertexCount = 6;

	m_indexCount = m_vertexCount;

	// ���ؽ� �迭 ����
	vertices = new VertexType[m_vertexCount];
	if (vertices == nullptr)
	{
		return false;
	}

	// �ε��� �迭 ����
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ���ؽ� �迭 �ʱ�ȭ
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// �ε��� �迭 �ʱ�ȭ
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// ���ؽ� ������ desc
	// D3D11_USAGE_DYNAMIC - ���� ���� ����
	// D3D11_CPU_ACCESS_WRITE - ���� ���� = �������� ���� ���氡��
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// ������ �����͸� ���ؽ� �迭�� ������ ����
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���۸� �����Ѵ�
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	// �ε��� ������ desc
	// D3D11_USAGE_DEFAULT - ���� ����
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	// ������ �����͸� �ε��� �迭�� ������ ����
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	// ���۸� �����Ѵ�
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// �� ���� �� ��볡�� ���ؽ�, �ε��� �迭�� �����Ѵ�
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	// �ε��� ���� ����
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// ���ؽ� ���� ����
	if (m_vertexBuffer != nullptr)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	// ������ ��ġ�� ���� ��� �ٽ� �׸� �ʿ����
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// ȭ�� ũ�⸦ �������� ��Ʈ���� ��, ��, ��, �Ʒ��� ����Ѵ�
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;
	top = (float)(m_screenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	// ����� ������ �ӽ� ���ؽ� �迭�� �����
	vertices = new VertexType[m_vertexCount];
	if (vertices == nullptr)
	{
		return false;
	}

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f); // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);
	
	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f); // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);
	
	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f); // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);
	
	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f); // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);
	
	vertices[4].position = D3DXVECTOR3(right, top, 0.0f); // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);
	
	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f); // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	// ���ؽ� ���۸� ��� ����
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ���ؽ� ������ �����͸� ����Ű�� �����͸� �����´�
	verticesPtr = (VertexType*)mappedResource.pData;

	// �ӽ� ���ؽ� �迭�� ������ ���ؽ� ���ۿ� �����Ѵ�
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*m_vertexCount));

	// ��� ���ؽ� ���۸� Ǯ���ش�
	deviceContext->Unmap(m_vertexBuffer, 0);

	// �پ� ���ؽ� �迭�� �����Ѵ�
	delete[] vertices;
	vertices = nullptr;

	return true;
}

// ���̴��� gpu���� ���ؽ�, �ε��� ���۸� ����� �� �ֵ��� �����Ѵ�
// = �Է� ������� ���ؽ�, �ε��� ���۸� �����Ѵ�
void BitmapClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	// ���ؽ� ���۸� �Է� ������� ����
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// �ε��� ���۸� �Է� ������� ����
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �׸��� �⺻ ������ �ﰢ������ ����
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	// �ؽ��� ��ü�� �����Ѵ�
	m_Texture = new TextureClass;
	if (m_Texture == nullptr)
	{
		return false;
	}

	// �ؽ��� ��ü�� �ʱ�ȭ�Ѵ�
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void BitmapClass::ReleaseTexture()
{
	// �ؽ��� ��ü�� �����Ѵ�
	if (m_Texture != nullptr)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}

	return;
}
