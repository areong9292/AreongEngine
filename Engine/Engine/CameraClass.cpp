#include "CameraClass.h"

CameraClass::CameraClass()
{
	// 카메라 위치, 회전 초기화
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

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

// 카메라의 위치와 회전 정보를 이용하여 뷰 행렬을 갱신한다
void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// 업벡터 초기화
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// 카메라 위치 갱신
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// 방향벡터 초기화
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// 현재 회전 값으로 yaw (Y축), pitch (X축), roll (Z축) 회전을 라디안으로 설정
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// 라디안 설정된 값으로 회전 행렬을 생성
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// 방향 벡터와 업벡터를 회전변환 한다
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// 회전된 카메라 위치를 현 위치로 이동
	lookAt = position + lookAt;
	
	// 뷰 행렬을 만든다
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	
	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;

	return;
}
