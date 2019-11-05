#include "ModelClass.h"

ModelClass::ModelClass()
{
	// 버텍스 버퍼와 인덱스 버퍼의 포인터를 null로 초기화
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;

	// 모델 포인터 초기화
	m_model = nullptr;

	m_Material = nullptr;
}

ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

// 버텍스 버퍼와 인덱스 버퍼의 초기화 함수를 호출한다
// 모델 파일의 이름을 인자로 하여 모델을 로드한다
// 텍스쳐 파일의 이름을 인자로 하여 텍스쳐를 로드한다
bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;
	m_Device = device;

	// 모델 데이터를 읽어와서 버텍스, 인덱스 배열을 채운 뒤(LoadModel)
	// 버텍스, 인덱스 버퍼를 생성한다(InitializeBuffers)
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// 버텍스 버퍼와 인덱스 버퍼를 초기화한다
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

// 버텍스 버퍼와 인덱스 버퍼를 해제하는 메소드를 호출
void ModelClass::Shutdown()
{
	// 사용한 버텍스, 인덱스 버퍼를 반환한다
	ShutdownBuffers();

	// 사용한 모델 객체를 반환한다
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 버텍스 버퍼와 인덱스 버퍼를 그래픽 파이프라인에 넣는 메소드 호출
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

	// 버텍스 버퍼와 인덱스 버퍼를 초기화한다
	bool result = InitializeBuffers(m_Device);
	if (!result)
	{
		//return false;
	}
}

// 버텍스 버퍼와 인덱스 버퍼를 생성한다
// 데이터 파일로부터 모델의 정보를 읽어와서 버퍼를 만든다
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;		// 모델의 버텍스들
	unsigned long* indices;		// 모델의 버텍스의 인덱스들

	// 버텍스, 인덱스 버퍼의 desc
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;

	// 버텍스, 인덱스 데이터
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	int i;

	// 버텍스, 인덱스 배열의 크기(m_vertexCount, m_indexCount)는
	// LoadModel에서 모델 정보에 맞게 결정된다

	// 버텍스 배열을 생성한다
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 생성한다
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 버텍스 배열에 값을 채운다
	// 모델의 값으로 채운다
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// 채운 데이터로 버텍스 버퍼와 인덱스 버퍼를 생성한다
	// 1. 버퍼에 대한 desc를 작성한다
	// 2. 위에서 채운 배열을 subresource포인터(D3D11_SUBRESOURCE_DATA)에 연결한다
	// 3. device의 CreateBuffer 메소드를 호출하여 버퍼의 포인터를 받아온다

	// 1. 버텍스 버퍼의 description을 작성
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 2. 버텍스 데이터를 가리키는 보조 리소스 구조체를 작성
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 3. 버텍스 버퍼를 생성
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 1. 인덱스 버퍼의 description을 작성
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	// 2. 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	// 3. 인덱스 버퍼를 생성
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// 버퍼 생성 후 버텍스, 인덱스 배열 메모리를 해제한다
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// 버텍스 버퍼를 해제
	if (m_vertexBuffer != nullptr)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

// 버텍스 버퍼와 인덱스 버퍼를 GPU의 Input assembler의 버퍼로서 활성화 시킨다
// -> 버텍스 버퍼와 인덱스 버퍼를 그래픽 파이프라인에 넣는다

// 흔히 말하는 그래픽 파이프라인의 입력 조립기 단계로
// 파이프라인에 리소스의 데이터를 제공한다
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// 버텍스 버퍼의 단위와 오프셋을 설정한다
	stride = sizeof(VertexType);
	offset = 0;

	// Input assembler에 버텍스,인덱스 버퍼를 활성화 하여 그려질 수 있게 한다
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 버텍스 버퍼로 그릴 기본형을 설정한다(삼각형)
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

	// 버텍스의 수 가져옴
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin >> m_vertexCount;

	// 인덱스의 수는 버텍스 수와 같게 지정
	m_indexCount = m_vertexCount;

	// 모델 데이터 초기화
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// 실제 모델 데이터 받아옴
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
