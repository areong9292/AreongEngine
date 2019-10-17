#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_

#include "ModelClass.h"
#include "d3dclass.h"

#include <d3dx10math.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

// 한 장면의 모든 모델들에 대한 정보를 유지, 관리하는 클래스
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

	// 모델의 색상과 좌표 저장용 구조체
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
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
	void GetData(int index, float& positionX, float& positionY, float& positionZ, D3DXVECTOR4& color);
	ModelClass* GetModel(int index);
	char* GetModelName(int index);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;

	wchar_t* ConverCtoWC(char* str);
};

#endif // !_MODELLISTCLASS_H_