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

// 비트맵 클래스를 초기화한다
bool BitmapClass::Initialize(ID3D11Device * device,
	int screenWidth, int screenHeight,
	WCHAR* textureFilename,
	int bitmapWidth, int bitmapHeight)
{
	bool result;

	// 화면 크기와 이미지 크기 저장
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// 이전 렌더링 위치는 -1로 초기화
	// 그린 적이 없으니 -1로 한다
	m_previousPosX = -1;
	m_previousPosY = -1;

	// 버퍼를 생성하고 초기화한다
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// 텍스쳐를 로드한다
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void BitmapClass::Shutdown()
{
	// 텍스쳐를 해제한다
	ReleaseTexture();

	// 버텍스, 인덱스 버퍼를 해제한다
	ShutdownBuffers();
	return;
}

// 2d 이미지의 버퍼를 그래픽 카드에 넣는다
bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result;

	// 이전 프레임과 위치를 비교해서 바뀌었을 때만 버텍스 버퍼의
	// 버텍스들을 새로운 위치로 갱신한다
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
	VertexType* vertices;		// 버텍스 배열
	unsigned long* indices;		// 인덱스 배열

	// 버텍스, 인덱스 버퍼의 desc
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;

	// 버텍스, 인덱스 데이터
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;
	int i;

	// 버텍스 수 설정 - 2개 삼각형으로 사각형 만들어야한다
	m_vertexCount = 6;

	m_indexCount = m_vertexCount;

	// 버텍스 배열 생성
	vertices = new VertexType[m_vertexCount];
	if (vertices == nullptr)
	{
		return false;
	}

	// 인덱스 배열 생성
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 버텍스 배열 초기화
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// 인덱스 배열 초기화
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// 버텍스 버퍼의 desc
	// D3D11_USAGE_DYNAMIC - 동적 버퍼 생성
	// D3D11_CPU_ACCESS_WRITE - 쓰기 가능 = 동적으로 버퍼 변경가능
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 버퍼의 데이터를 버텍스 배열의 값으로 설정
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 버퍼를 생성한다
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	// 인덱스 버퍼의 desc
	// D3D11_USAGE_DEFAULT - 정적 버퍼
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	// 버퍼의 데이터를 인덱스 배열의 값으로 설정
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	// 버퍼를 생성한다
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// 다 만든 후 사용끝난 버텍스, 인덱스 배열을 해제한다
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void BitmapClass::ShutdownBuffers()
{
	// 인덱스 버퍼 해제
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// 버텍스 버퍼 해제
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

	// 이전과 위치가 같을 경우 다시 그릴 필요없다
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// 화면 크기를 기준으로 비트맵의 좌, 우, 위, 아래를 계산한다
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;
	top = (float)(m_screenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	// 계산한 값으로 임시 버텍스 배열을 만든다
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

	// 버텍스 버퍼를 잠근 다음
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 버텍스 버퍼의 데이터를 가리키는 포인터를 가져온다
	verticesPtr = (VertexType*)mappedResource.pData;

	// 임시 버텍스 배열의 내용을 버텍스 버퍼에 복사한다
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*m_vertexCount));

	// 잠근 버텍스 버퍼를 풀어준다
	deviceContext->Unmap(m_vertexBuffer, 0);

	// 다쓴 버텍스 배열은 해제한다
	delete[] vertices;
	vertices = nullptr;

	return true;
}

// 쉐이더가 gpu에서 버텍스, 인덱스 버퍼를 사용할 수 있도록 설정한다
// = 입력 어셈블러에 버텍스, 인덱스 버퍼를 셋팅한다
void BitmapClass::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	// 버텍스 버퍼를 입력 어셈블러에 설정
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 인덱스 버퍼를 입력 어셈블러에 설정
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 그리는 기본 도형을 삼각형으로 지정
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	// 텍스쳐 객체를 생성한다
	m_Texture = new TextureClass;
	if (m_Texture == nullptr)
	{
		return false;
	}

	// 텍스쳐 객체를 초기화한다
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void BitmapClass::ReleaseTexture()
{
	// 텍스쳐 객체를 해제한다
	if (m_Texture != nullptr)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}

	return;
}
