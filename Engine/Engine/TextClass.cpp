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

// �ؽ�Ʈ Ŭ���� �ʱ�ȭ
bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// ȭ�� ũ�� ����
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// �⺻ �� ���(ī�޶��� �� ���) ����
	m_baseViewMatrix = baseViewMatrix;

	// ��Ʈ Ŭ������ �����ϰ�
	m_Font = new FontClass;
	if (m_Font == nullptr)
	{
		return false;
	}

	// ��Ʈ uv ������ �ؽ��� �����ͼ� �ʱ�ȭ�Ѵ�
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

	// ������ �����ϰ� �ʱ�ȭ�Ѵ�
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

	// ������ �����ϰ� �ʱ�ȭ�Ѵ�
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
	// ���� ����
	ReleaseSentence(&m_sentence1);
	ReleaseSentence(&m_sentence2);

	// ��Ʈ���̴� ����
	if (m_FontShader != nullptr)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = nullptr;
	}

	// ��Ʈ ����
	if (m_Font != nullptr)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}
}

// ������ ȭ�鿡 �׸���
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

	// x��ǥ ��Ʈ������ ����
	_itoa_s(mouseX, tempString, 10);

	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	result = UpdateSentence(m_sentence1, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// y��ǥ ��Ʈ������ ����
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

// ������ �����ϰ� �׸��� ���� �� ���ؽ� ���۷� �̷���� SentenceType ������ �����Ѵ�
// ���⼭�� TextClass�� �������� �ּҸ� ���޹޾Ƽ�(sentence) �� ��ġ�� �����Ѵ�
bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	// ���ؽ�, �ε��� �迭
	VertexType* vertices;
	unsigned long* indices;

	// ���ؽ�, �ε��� ���� desc
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;

	// ���ؽ�, �ε��� ������
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

	// ���ؽ�, �ε��� �迭 �ʱ�ȭ
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

	// ������ ������ ������ ���� �� �� �ֵ��� ����Ʈ desc�� dynamic���� ����
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// �迭 ���� ����
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// �ش� ������ ���ؽ� ���� ����� - �ٷ� sentence�� ���ۿ� ����
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ε��� ���۴� ���� �ȹٲ�Ƿ� �������� ����
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �迭 ���� ����
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ش� ������ �ε��� ���� ����� - �ٷ� sentence�� ���ۿ� ����
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �� �� ���ؽ�, �ε��� �迭�� �����Ѵ�
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

// �Էµ� ����(text)�� �°� ���ؽ� ���۸� �����Ѵ�
bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// ������ ���� ����
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// ������ ���� ����
	numLetters = (int)strlen(text);
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// ���ؽ� �迭 �ʱ�ȭ
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// ȭ�� ���� ���� ��ġ�� ����Ѵ�
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// FontClass�� ���������� ���ؽ� �迭�� �����
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// �ش� �迭 ������ ���ؽ� ���ۿ� ���� ����
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	// verticesPtr�� vertices�� ������ vertexCount��ŭ �����Ѵ�
	// verticesPtr - ���ؽ� ����, vertices - ���ؽ� �迭(FontClass���� ���� ���ؽ� �迭)
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*sentence->vertexCount));

	deviceContext->Unmap(sentence->vertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

// SentenceType �޸� ����
// SentenceType���� ���ؽ�, �ε��� ���۵� �����Ѵ�
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

// ���ؽ�, �ε��� ���۸� �Է� ������� �ְ� ���̴��� ȣ���Ͽ�
// �Էµ� ���۵��� �׸��� �Ѵ�
bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatirx)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	stride = sizeof(VertexType);
	offset = 0;

	// ���ؽ� ���۸� �Է� ������� �ִ´�
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// �ε��� ���۸� �Է� ������� �ִ´�
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �׸� �� �ﰢ������ �׸�
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �ȼ��� ������ ���ڿ� ������ ������ �����Ѵ�
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// ��Ʈ ���̴��� ������ �������Ѵ�
	// �׸� �� ������ ����Ʈ�� �ƴ� �ʱ�ȭ �� ������ �ʱ� ī�޶��� ����Ʈ(m_baseViewMatrix)�� �׸���
	// �̷� ���� ī�޶� ��ġ�� ȸ���� ����Ǿ �׻� ���� ��ġ�� ���ڰ� �׷�����
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatirx, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		return false;
	}

	return true;
}