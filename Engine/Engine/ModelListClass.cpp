#include "ModelListClass.h"

ModelListClass::ModelListClass()
{
	m_ModelInfoList = nullptr;
}

ModelListClass::ModelListClass(const ModelListClass& other)
{
}


ModelListClass::~ModelListClass()
{
}

bool ModelListClass::Initialize(D3DClass* m_D3D)
{
	int i;
	float red, green, blue;

	// ���� ������ ��� ������ �о�´�
	ifstream fin;
	char inputLine[255];
	char* tok1;
	char* context1 = NULL;
	char tn[] = " \t\n";

	int index;

	fin.open("../Engine/data/ModelListInfo.txt");
	if (fin.fail())
	{
		return false;
	}

	while (fin)
	{
		fin.getline(inputLine, 255);
		if (fin)
		{
			// �ݷ��� �����Ѵ�
			tok1 = strtok_s(inputLine, ":", &context1);

			if (tok1 != NULL)
			{
				// �� ī��Ʈ ����
				if (strstr(tok1, "ModelCount"))
				{
					tok1 = strtok_s(NULL, ":", &context1);
					if (tok1 != NULL)
					{
						m_modelCount = atoi(tok1);

						// ���Ͽ��� ���� �� ī��Ʈ ��ŭ �迭 ����
						m_ModelInfoList = new ModelInfoType[m_modelCount];
						if (m_ModelInfoList == nullptr)
						{
							return false;
						}
					}
				}
				else if (strstr(tok1, "Model_"))
				{
					index = atoi(tok1 + 6) - 1;
				}
				else
				{
					// �ǰ� ��������
					tok1 = strtok_s(tok1, tn, &context1);

					if (strstr(tok1, "color"))
					{
						// ���� ���� �޴´�
						fin.getline(inputLine, 255);
						if (fin)
						{
							// ����� �ǰ� ��������
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{
								red = atof(tok1);
							}
							tok1 = strtok_s(NULL, tn, &context1);
							if (tok1 != NULL)
							{
								green = atof(tok1);
							}
							tok1 = strtok_s(NULL, tn, &context1);
							if (tok1 != NULL)
							{
								blue = atof(tok1);
							}

							// ���Ͽ��� ���� �÷� ���� ����
							m_ModelInfoList[index].color = D3DXVECTOR4(red, green, blue, 1.0f);
						}
					}
					else if (strstr(tok1, "position"))
					{
						// ���� ���� �޴´�
						fin.getline(inputLine, 255);
						if (fin)
						{
							// ����� �ǰ� ��������
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{
								m_ModelInfoList[index].positionX = atof(tok1);
							}
							tok1 = strtok_s(NULL, tn, &context1);
							if (tok1 != NULL)
							{
								m_ModelInfoList[index].positionY = atof(tok1);
							}
							tok1 = strtok_s(NULL, tn, &context1);
							if (tok1 != NULL)
							{
								m_ModelInfoList[index].positionZ = atof(tok1);
							}
						}
					}
					else if (strstr(tok1, "modelFilePath"))
					{
						// ���� ���� �޴´�
						fin.getline(inputLine, 255);
						if (fin)
						{
							// ����� �ǰ� ��������
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{
								// ���� ���ڿ� ��ü ����
								m_ModelInfoList[index].modelFilePath = new char[strlen(tok1)];
								strcpy_s(m_ModelInfoList[index].modelFilePath, strlen(m_ModelInfoList[index].modelFilePath), tok1);
							}
						}
					}
					else if (strstr(tok1, "textureFilePath"))
					{
						// ���� ���� �޴´�
						fin.getline(inputLine, 255);
						if (fin)
						{
							// ����� �ǰ� ��������
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{
								// ���� ���ڿ� ��ü ����
								m_ModelInfoList[index].textureFilePath = new char[strlen(tok1)];

								strcpy_s(m_ModelInfoList[index].textureFilePath, strlen(m_ModelInfoList[index].textureFilePath), tok1);
							}
						}
					}
					else if (strstr(tok1, "shaderName"))
					{
						// ���� ���� �޴´�
						fin.getline(inputLine, 255);
						if (fin)
						{
							// ����� �ǰ� ��������
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{
								//m_ModelInfoList[index].shaderName = tok1;
							}
						}
					}
					else if (strstr(tok1, "frustum"))
					{
						// ���� ���� �޴´�
						fin.getline(inputLine, 255);
						if (fin)
						{
							// ����� �ǰ� ��������
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{

							}
						}
					}
				}
			}
		}
	}

	bool result;

	// ������ �� ��η� �� Ŭ������ �ʱ�ȭ�Ѵ�
	for (int i = 0; i < m_modelCount; i++)
	{
		m_ModelInfoList[i].m_model = new ModelClass;
		if (!m_ModelInfoList[i].m_model)
		{
			return false;
		}
		WCHAR* lTexturePath = ConverCtoWC(m_ModelInfoList[i].textureFilePath);

		result = m_ModelInfoList[i].m_model->Initialize(m_D3D->GetDevice(),					// device ��ü
														m_ModelInfoList[i].modelFilePath,	// �� ���� ���
														lTexturePath);						// �ش� �𵨿� ���� �ؽ��� ���
		if (!result)
		{
			return false;
		}
	}

	return true;
}

wchar_t* ModelListClass::ConverCtoWC(char* str)
{
	//wchar_t�� ���� ����
	wchar_t* pStr;
	//��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);

	//wchar_t �޸� �Ҵ�
	pStr = new WCHAR[strSize];
	//�� ��ȯ
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, pStr, strSize);

	return pStr;
}


void ModelListClass::Shutdown()
{
	// ����� �� ����Ʈ ����
	if (m_ModelInfoList != nullptr)
	{
		delete[] m_ModelInfoList;
		m_ModelInfoList = nullptr;
	}

	return;
}

int ModelListClass::GetModelCount()
{
	return m_modelCount;
}

// �ε����� ���� ��ġ�� ������ �����´�
void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ, D3DXVECTOR4& color)
{
	positionX = m_ModelInfoList[index].positionX;
	positionY = m_ModelInfoList[index].positionY;
	positionZ = m_ModelInfoList[index].positionZ;

	color = m_ModelInfoList[index].color;

	return;
}

ModelClass* ModelListClass::GetModel(int index)
{
	return m_ModelInfoList[index].m_model;
}

char * ModelListClass::GetModelName(int index)
{
	return m_ModelInfoList[index].modelFilePath;
}
