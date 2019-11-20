#include "FontClass.h"

FontClass::FontClass()
{
	m_Font = nullptr;
	m_Texture = nullptr;
}

FontClass::FontClass(const FontClass &)
{
}

FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;

	// 글꼴 데이터 파일을 로드한다
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// 폰트 텍스쳐 파일을 로드한다
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

// 글꼴 데이터와 텍스쳐를 해제한다
void FontClass::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();

	return;
}

ID3D11ShaderResourceView * FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

// 인자로 받은 문장으로 버텍스 버퍼를 생성한다
void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	index = 0;

	for (i = 0; i < numLetters; i++)
	{
		// 문장의 한 글자를 포멧에 맞게(글꼴 인덱스 데이터) 변환
		letter = ((int)sentence[i]) - 32;

		// 공백일 경우 옆으로 3픽셀
		if (letter == 0)
		{
			drawX += 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;
			
			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f); // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;
			
			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f); // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;
			
			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;
			
			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f); // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;
			
			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f); // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;
			
			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}

	return;
}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i = 0;
	char inputLine[255];

	char* tok1;
	char* context1 = NULL;

	// 글꼴의 텍스쳐 인덱스를 저장할 FontType
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	while (fin)
	{
		fin.getline(inputLine, 255);
		if (fin)
		{
			tok1 = strtok_s(inputLine, " ", &context1);
			tok1 = strtok_s(NULL, " ", &context1);
			m_Font[i].left = atof(strtok_s(NULL, " ", &context1));
			m_Font[i].right = atof(strtok_s(NULL, " ", &context1));
			m_Font[i].size = atof(strtok_s(NULL, " ", &context1));
			i++;
		}
	}

	fin.close();

	return true;
}

// 글꼴 데이터에서 읽은 글꼴 인덱스 데이터를 해제한다
void FontClass::ReleaseFontData()
{
	if (m_Font != nullptr)
	{
		delete[] m_Font;
		m_Font = nullptr;
	}

	return;
}

// 텍스쳐를 로드한다
bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

// 사용한 텍스쳐 해제
void FontClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}
