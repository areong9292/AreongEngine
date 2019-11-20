#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_

#include "ModelClass.h"
#include "d3dclass.h"

#include <DirectXMath.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

#include <string>

using namespace std;
using namespace DirectX;

// �� ����� ��� �𵨵鿡 ���� ������ ����, �����ϴ� Ŭ����
class ModelListClass
{
private:
	enum shaderType
	{
		Default,
		Texture,
		MultiTexture,
		Font,
		Light
	};

	enum frustumType
	{
		Point,
		Cube,
		Sphere,
		Rectangle
	};

	// ���� ����� ��ǥ ����� ����ü
	struct ModelInfoType
	{
		XMFLOAT4 color;
		float positionX, positionY, positionZ;
		char* modelFilePath;
		char* textureFilePath;
		shaderType shaderName;
		frustumType frustumType;
		ModelClass* m_model;
	};
public:
	ModelListClass();
	ModelListClass(const ModelListClass&);
	~ModelListClass();

	bool Initialize(D3DClass* m_D3D);
	void Shutdown();

	int GetModelCount();
	void GetData(int index, float& positionX, float& positionY, float& positionZ, XMFLOAT4& color);
	ModelClass* GetModel(int index);
	char* GetModelName(int index);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;

	wchar_t* ConverCtoWC(char* str);

	string modelName = "";
};

#endif // !_MODELLISTCLASS_H_