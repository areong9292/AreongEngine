#include "TextClass.h"



TextClass::TextClass()
{
	m_Font = nullptr;
	m_FontShader = nullptr;

	m_sentence1 = nullptr;
	m_sentence2 = nullptr;

}

TextClass::TextClass(const TextClass &)
{
}

TextClass::~TextClass()
{
}

// 텍스트 클래스 초기화
bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// 화면 크기 저장
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 기본 뷰 행렬(카메라의 뷰 행렬) 저장
	m_baseViewMatrix = baseViewMatrix;

	// 폰트 클래스를 생성하고
	m_Font = new FontClass;
	if (m_Font == nullptr)
	{
		return false;
	}

	// 폰트 uv 정보와 텍스쳐 가져와서 초기화한다
	result = m_Font->Initialize(device, "../Engine/data/fontdata.txt", L"../Engine/data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	m_FontShader = new FontShaderClass;
	if (m_FontShader == nullptr)
	{
		return false;
	}

	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// 문장을 생성하고 초기화한다
	result = InitializeSentence(&m_sentence1, 100, device);
	if (!result)
	{
		return false;
	}

	result = UpdateSentence(m_sentence1, "Hello", 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// 문장을 생성하고 초기화한다
	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result)
	{
		return false;
	}

	result = UpdateSentence(m_sentence2, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	// 문장 해제
	ReleaseSentence(&m_sentence1);
	ReleaseSentence(&m_sentence2);

	// 폰트쉐이더 해제
	if (m_FontShader != nullptr)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = nullptr;
	}

	// 폰트 해제
	if (m_Font != nullptr)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}
}

// 문장을 화면에 그린다
bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];
	bool result;

	// x좌표 스트링으로 만듬
	_itoa_s(mouseX, tempString, 10);

	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	result = UpdateSentence(m_sentence1, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// y좌표 스트링으로 만듬
	_itoa_s(mouseY, tempString, 10);

	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	result = UpdateSentence(m_sentence2, mouseString, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	if (fps > 9999)
	{
		fps = 9999;
	}

	char tempString[16] = { 0, };
	_itoa_s(fps, tempString, 10);

	char fpsString[16] = { 0, };
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;

	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	else
	{
		if (fps < 30)
		{
			red = 1.0f;
			green = 0.0f;
			blue = 0.0f;
		}
		else if (fps < 60)
		{
			red = 1.0f;
			green = 1.0f;
			blue = 0.0f;
		}
	}

	return UpdateSentence(m_sentence1, fpsString, 20, 20, red, green, blue, deviceContext);
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16] = { 0, };
	_itoa_s(cpu, tempString, 10);

	char cpuString[16] = { 0, };
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	return UpdateSentence(m_sentence2, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
}

bool TextClass::SetRenderCount(int renderCount, ID3D11DeviceContext* deviceContext)
{
	char tempString[16] = { 0, };
	_itoa_s(renderCount, tempString, 10);

	char cpuString[16] = { 0, };
	strcpy_s(cpuString, "renderCount: ");
	strcat_s(cpuString, tempString);

	return UpdateSentence(m_sentence2, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
}

bool TextClass::SetIntersection(bool isPick, ID3D11DeviceContext * deviceContext, char* modelName)
{
	char cpuString[100] = { 0, };
	strcpy_s(cpuString, "isPicked: ");
	if (isPick)
	{
		strcat_s(cpuString, modelName);
	}
	else
		strcat_s(cpuString, "No");

	return UpdateSentence(m_sentence1, cpuString, 20, 20, 0.0f, 1.0f, 0.0f, deviceContext);
}

// 문장을 저장하고 그리기 위해 빈 버텍스 버퍼로 이루어진 SentenceType 변수를 생성한다
// 여기서는 TextClass의 데이터의 주소를 전달받아서(sentence) 그 위치에 생성한다
bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	// 버텍스, 인덱스 배열
	VertexType* vertices;
	unsigned long* indices;

	// 버텍스, 인덱스 버퍼 desc
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;

	// 버텍스, 인덱스 데이터
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	HRESULT result;
	int i;

	*sentence = new SentenceType;
	if (*sentence == nullptr)
	{
		return false;
	}

	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;

	(*sentence)->maxLength = maxLength;

	(*sentence)->vertexCount = 6 * maxLength;
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// 버텍스, 인덱스 배열 초기화
	vertices = new VertexType[(*sentence)->vertexCount];
	if (vertices == nullptr)
	{
		return false;
	}

	indices = new unsigned long[(*sentence)->indexCount];
	if (indices == nullptr)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// 언제든 문장의 내용을 변경 할 수 있도록 버덱트 desc를 dynamic으로 설정
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// 배열 정보 셋팅
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 해당 정보로 버텍스 버퍼 만든다 - 바로 sentence의 버퍼에 만듬
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 인덱스 버퍼는 내용 안바뀌므로 정적으로 생성
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 배열 정보 셋팅
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 해당 정보로 인덱스 버퍼 만든다 - 바로 sentence의 버퍼에 만듬
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 다 쓴 버텍스, 인덱스 배열은 해제한다
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

// 입력된 문장(text)에 맞게 버텍스 버퍼를 변경한다
bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// 문장의 색상 지정
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// 문장의 길이 지정
	numLetters = (int)strlen(text);
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// 버텍스 배열 초기화
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// 화면 상의 글자 위치를 계산한다
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// FontClass와 문장정보로 버텍스 배열을 만든다
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// 해당 배열 정보를 버텍스 버퍼에 직접 쓴다
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	// verticesPtr에 vertices의 내용을 vertexCount만큼 복사한다
	// verticesPtr - 버텍스 버퍼, vertices - 버텍스 배열(FontClass에서 만든 버텍스 배열)
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*sentence->vertexCount));

	deviceContext->Unmap(sentence->vertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

// SentenceType 메모리 해제
// SentenceType안의 버텍스, 인덱스 버퍼도 해제한다
void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		if ((*sentence)->vertexBuffer != nullptr)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = nullptr;
		}

		if ((*sentence)->indexBuffer != nullptr)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = nullptr;
		}

		delete *sentence;
		*sentence = nullptr;
	}

	return;
}

// 버텍스, 인덱스 버퍼를 입력 어셈블러에 넣고 쉐이더를 호출하여
// 입력된 버퍼들을 그리게 한다
bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatirx)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	stride = sizeof(VertexType);
	offset = 0;

	// 버텍스 버퍼를 입력 어셈블러에 넣는다
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// 인덱스 버퍼를 입력 어셈블러에 넣는다
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 그릴 때 삼각형으로 그림
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 픽셀의 색상을 문자열 정보를 가지고 생성한다
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// 폰트 쉐이더로 문장을 렌더링한다
	// 그릴 때 현재의 뷰포트가 아닌 초기화 때 저장한 초기 카메라의 뷰포트(m_baseViewMatrix)에 그린다
	// 이로 인해 카메라가 위치나 회전이 변경되어도 항상 같은 위치에 글자가 그려진다
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatirx, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		return false;
	}

	return true;
}