#pragma once
#include "D3DApp.h"

// 한 모델의 정보를 가지고 있는 메소드
class ModelClass
{
private:
	// 한 점의 정보를 담는 버텍스
	struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float z)
			: pos(x, y, z) {}

		XMFLOAT3 pos;
	};

public:
	ModelClass();
	~ModelClass();

private:
	// 버텍스 생성
	Vertex v[] =
	{
		Vertex(0.0f, 0.5f, 0.5f),
		Vertex(0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, -0.5f, 0.5f),
	};
};

