#pragma once
#include "D3DApp.h"

// �� ���� ������ ������ �ִ� �޼ҵ�
class ModelClass
{
private:
	// �� ���� ������ ��� ���ؽ�
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
	// ���ؽ� ����
	Vertex v[3] =
	{
		Vertex(0.0f, 0.5f, 0.5f),
		Vertex(0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, -0.5f, 0.5f),
	};
};

