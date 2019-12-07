#ifndef _FRUSTUMCLASS_H_
#define _FRUSTUMCLASS_H_

#include <DirectXMath.h>

#include "./Utils/AlignedAllocationPolicy.h"
using namespace DirectX;

class FrustumClass : public AlignedAllocationPolicy<16>
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass& other);
	~FrustumClass();

	// ���ŵ� ī�޶� ��ġ�� ���ʷ� ����ü 6�� ����� ����Ѵ�
	void ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);

	bool Checkpoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	XMVECTOR m_planes[6];
};



#endif // !_FRUSTUMCLASS_H_

