#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

// 텍스트 파일(모델 파일)을 읽기 위한 include
#include <fstream>

// 모델의 머테리얼
#include "Material.h"
#include "DesignPatternHeader.h"

// FBXimporter
#include "FBXImporter.h"

using namespace std;
using namespace DirectX;

// 3D모델들의 복잡한 기하학들을 캡슐화하는 클래스
class ModelClass
{
private:
	// 정점 버퍼에 사용할 정점의 구조체 선언
	// 버텍스 쉐이더의 입력과 형태가 같아야한다
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;	// 텍스쳐를 입힐 것이다
		XMFLOAT3 normal;	// 조명 지원을 위한 법선 벡터 추가
	};

	// 모델 포맷 구조체
	struct ModelType
	{
		float x, y, z;		// 위치 좌표
		float tu, tv;		// 텍스쳐 좌표
		float nx, ny, nz;	// 법선 벡터
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	// 3D 모델의 버텍스 버퍼, 인덱스 버퍼들의 초기화와 종료 과정을 제어
	bool Initialize(ID3D11Device*, char*, WCHAR*);		// 모델 파일명, 텍스쳐 경로
	void Shutdown();

	// 그래픽 카드에 모델들의 기하 정보를 넣고 컬러 쉐이더로 그릴 준비를 한다
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	Material* GetModelMaterial();

	void test(vector<FBXImporter::MyVertex> test);

private:
	// 3D 모델의 버텍스 버퍼, 인덱스 버퍼들의 초기화와 종료 과정을 제어
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	// 그래픽 카드에 모델들의 기하 정보를 넣고 컬러 쉐이더로 그릴 준비를 한다
	void RenderBuffers(ID3D11DeviceContext*);

	// 모델 데이터를 불러온다
	bool LoadModel(char*);

	// 모델 데이터를 반환한다
	void ReleaseModel();

private:
	// 버텍스 버퍼, 인덱스 버퍼, 각 버퍼의 크기 정보를 가지고 있는 변수
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	// 모델 데이터 저장용
	ModelType* m_model;

	// 모델의 머테리얼 오브젝트
	GameObject* m_Material;

	ID3D11Device* m_Device;
};

#endif