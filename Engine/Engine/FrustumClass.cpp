#include "FrustumClass.h"

FrustumClass::FrustumClass()
{
}

FrustumClass::FrustumClass(const FrustumClass& other)
{
}

FrustumClass::~FrustumClass()
{
}

// �� �����Ӹ��� ȣ��ȴ�
// GraphicsClass���� ȭ���� depth, �������, ������� ���� �޾�
// �ش� ������ ����ü�� ����Ѵ�
void FrustumClass::ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix)
{
	float zMinimum, r;
	D3DXMATRIX matrix;

	// ����İ� ��������� ���Ͽ� ����ü ����� �����
	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);

	// ��� ���������� 6���� ���(����ü ������ �ѷ��δ�)�� �����
	// Ư�� ��Ұ� �ش� ���� �ȿ� �ִ��� ���θ� �Ǵ��Ѵ�

	// ����ü ����� ���� �� ����ü ���� ȭ�� ���� ��ǥ�� ��ȯ�ؾ��Ѵ�

	// �� ����� �������͸� �����س���
	// �� ���Ϳ� ����İ� ��������� ���� ����ü ����� ��ġ����� ���ϸ� �ȴ�

	// ����ü�� ����� ����� ���
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);
	
	// ����ü�� �� ����� ���
	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);
	
	// ����ü�� ���� ����� ���
	m_planes[2].a = matrix._14 + matrix._11;
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);
	
	// ����ü�� ������ ����� ���
	m_planes[3].a = matrix._14 - matrix._11;
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);
	
	// ����ü�� �� ����� ���
	m_planes[4].a = matrix._14 - matrix._12;
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);
	
	// ����ü�� �Ʒ� ����� ���
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);
	
	return;
}

// �ϳ��� ���� ����ü ���ο� �ִ��� ����
bool FrustumClass::Checkpoint(float x, float y, float z)
{
	int i;
	for (int i = 0; i < 6; i++)
	{
		// ��� �ڿ� ������ ����ü ��� ��
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(x, y, z)) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

// ������ü�� ����ü �ȿ� �ִ��� ����
// ��� �������� ����ü ���� �־�� �Ѵ�
bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;

	for (int i = 0; i < 6; i++)
	{
		// �� ��鿡 ���� 8�� ������ ��� ����Ͽ�
		// �ϳ��� ��� �ڷ� �Ѿ�� false ����
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter - radius))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter - radius))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter - radius))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter - radius))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter + radius))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter + radius))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter + radius))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter + radius))) >= 0.0f) { continue; }
		
		return false;
	}

	return true;
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;

	for (i = 0; i < 6; i++)
	{
		// ��ü�� ��ŭ �� ������ ���� �ݰ游 �߰����ָ� �ȴ�
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3(xCenter, yCenter, zCenter)) < -radius)
			return false;
	}

	return true;
}

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	int i;

	for (int i = 0; i < 6; i++)
	{
		// �� ��鿡 ���� 8�� ������ ��� ����Ͽ�
		// �ϳ��� ��� �ڷ� �Ѿ�� false ����
		// �� ���� ���̰� �ٸ� �� ������ü�� ���� ����
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f) { continue; }
		if (D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f) { continue; }

		return false;
	}

	return true;
}
