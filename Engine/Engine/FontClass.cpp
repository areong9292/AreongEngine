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

	// �۲� ������ ������ �ε��Ѵ�
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// ��Ʈ �ؽ��� ������ �ε��Ѵ�
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

// �۲� �����Ϳ� �ؽ��ĸ� �����Ѵ�
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

// ���ڷ� ���� �������� ���ؽ� ���۸� �����Ѵ�
void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	index = 0;

	for (i = 0; i < numLetters; i++)
	{
		// ������ �� ���ڸ� ���信 �°�(�۲� �ε��� ������) ��ȯ
		letter = ((int)sentence[i]) - 32;

		// ������ ��� ������ 3�ȼ�
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

	// �۲��� �ؽ��� �ε����� ������ FontType
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

// �۲� �����Ϳ��� ���� �۲� �ε��� �����͸� �����Ѵ�
void FontClass::ReleaseFontData()
{
	if (m_Font != nullptr)
	{
		delete[] m_Font;
		m_Font = nullptr;
	}

	return;
}

// �ؽ��ĸ� �ε��Ѵ�
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

// ����� �ؽ��� ����
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
