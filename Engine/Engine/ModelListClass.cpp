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

	// 모델의 정보가 담긴 파일을 읽어온다
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
			// 콜론을 제거한다
			tok1 = strtok_s(inputLine, ":", &context1);

			if (tok1 != NULL)
			{
				// 모델 카운트 저장
				if (strstr(tok1, "ModelCount"))
				{
					tok1 = strtok_s(NULL, ":", &context1);
					if (tok1 != NULL)
					{
						m_modelCount = atoi(tok1);

						// 파일에서 받은 모델 카운트 만큼 배열 생성
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
					// 탭과 개행제거
					tok1 = strtok_s(tok1, tn, &context1);

					if (strstr(tok1, "color"))
					{
						// 다음 줄을 받는다
						fin.getline(inputLine, 255);
						if (fin)
						{
							// 공백과 탭과 개행제거
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

							// 파일에서 받은 컬러 정보 저장
							m_ModelInfoList[index].color = D3DXVECTOR4(red, green, blue, 1.0f);
						}
					}
					else if (strstr(tok1, "position"))
					{
						// 다음 줄을 받는다
						fin.getline(inputLine, 255);
						if (fin)
						{
							// 공백과 탭과 개행제거
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
						// 다음 줄을 받는다
						fin.getline(inputLine, 255);
						if (fin)
						{
							// 공백과 탭과 개행제거
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{
								// 새로 문자열 객체 생성
								m_ModelInfoList[index].modelFilePath = new char[strlen(tok1)];
								strcpy_s(m_ModelInfoList[index].modelFilePath, strlen(m_ModelInfoList[index].modelFilePath), tok1);
							}
						}
					}
					else if (strstr(tok1, "textureFilePath"))
					{
						// 다음 줄을 받는다
						fin.getline(inputLine, 255);
						if (fin)
						{
							// 공백과 탭과 개행제거
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{
								// 새로 문자열 객체 생성
								m_ModelInfoList[index].textureFilePath = new char[strlen(tok1)];

								strcpy_s(m_ModelInfoList[index].textureFilePath, strlen(m_ModelInfoList[index].textureFilePath), tok1);
							}
						}
					}
					else if (strstr(tok1, "shaderName"))
					{
						// 다음 줄을 받는다
						fin.getline(inputLine, 255);
						if (fin)
						{
							// 공백과 탭과 개행제거
							tok1 = strtok_s(tok1, tn, &context1);
							if (tok1 != NULL)
							{
								//m_ModelInfoList[index].shaderName = tok1;
							}
						}
					}
					else if (strstr(tok1, "frustum"))
					{
						// 다음 줄을 받는다
						fin.getline(inputLine, 255);
						if (fin)
						{
							// 공백과 탭과 개행제거
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

	// 파일의 모델 경로로 모델 클래스를 초기화한다
	for (int i = 0; i < m_modelCount; i++)
	{
		m_ModelInfoList[i].m_model = new ModelClass;
		if (!m_ModelInfoList[i].m_model)
		{
			return false;
		}
		WCHAR* lTexturePath = ConverCtoWC(m_ModelInfoList[i].textureFilePath);

		result = m_ModelInfoList[i].m_model->Initialize(m_D3D->GetDevice(),					// device 객체
														m_ModelInfoList[i].modelFilePath,	// 모델 파일 경로
														lTexturePath);						// 해당 모델에 입힐 텍스쳐 경로
		if (!result)
		{
			return false;
		}
	}

	return true;
}

wchar_t* ModelListClass::ConverCtoWC(char* str)
{
	//wchar_t형 변수 선언
	wchar_t* pStr;
	//멀티 바이트 크기 계산 길이 반환
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);

	//wchar_t 메모리 할당
	pStr = new WCHAR[strSize];
	//형 변환
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, pStr, strSize);

	return pStr;
}


void ModelListClass::Shutdown()
{
	// 사용한 모델 리스트 해제
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

// 인덱스로 모델의 위치와 색상을 가져온다
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
