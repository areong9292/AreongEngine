#ifndef  _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <DirectXMath.h>
#include "./Utils/AlignedAllocationPolicy.h"
using namespace DirectX;

class CameraClass : public AlignedAllocationPolicy<16>
{
public:
	CameraClass();
	CameraClass(const CameraClass& other);
	~CameraClass();

	// ī�޶� ��ü�� ��ġ�� ȸ�� ���� ����
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	// ī�޶��� ��ġ�� ȸ�� ���¿� ����� �� ����� ����
	void Render();

	// ���̴����� �������� ����� �� �ֵ���
	// ī�޶� �� ��� ��ȯ
	void GetViewMatrix(XMMATRIX& viewMatrix);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
};

#endif