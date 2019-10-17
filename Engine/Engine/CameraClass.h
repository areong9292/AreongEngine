#ifndef  _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <d3dx10math.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass& other);
	~CameraClass();

	// ī�޶� ��ü�� ��ġ�� ȸ�� ���� ����
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	// ī�޶��� ��ġ�� ȸ�� ���¿� ����� �� ����� ����
	void Render();

	// ���̴����� �������� ����� �� �ֵ���
	// ī�޶� �� ��� ��ȯ
	void GetViewMatrix(D3DXMATRIX& viewMatrix);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

#endif