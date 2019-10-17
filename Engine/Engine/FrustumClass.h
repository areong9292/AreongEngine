#ifndef _FRUSTUMCLASS_H_
#define _FRUSTUMCLASS_H_

#include <d3dx10math.h>

class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass& other);
	~FrustumClass();

	// ���ŵ� ī�޶� ��ġ�� ���ʷ� ����ü 6�� ����� ����Ѵ�
	void ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);

	bool Checkpoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	D3DXPLANE m_planes[6];
};



#endif // !_FRUSTUMCLASS_H_

