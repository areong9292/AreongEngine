#include "CameraClass.h"

CameraClass::CameraClass()
{
	// ī�޶� ��ġ, ȸ�� �ʱ�ȭ
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	return;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

// ī�޶��� ��ġ�� ȸ�� ������ �̿��Ͽ� �� ����� �����Ѵ�
void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;

	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// ������ �ʱ�ȭ
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// ī�޶� ��ġ ����
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	positionVector = XMLoadFloat3(&position);

	// ���⺤�� �ʱ�ȭ
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// ���� ȸ�� ������ yaw (Y��), pitch (X��), roll (Z��) ȸ���� �������� ����
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// ���� ������ ������ ȸ�� ����� ����
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// ���� ���Ϳ� �����͸� ȸ����ȯ �Ѵ�
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// ȸ���� ī�޶� ��ġ�� �� ��ġ�� �̵�
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);
	
	// �� ����� �����
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
	
	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;

	return;
}
