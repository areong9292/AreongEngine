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
void FrustumClass::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	// ���� ����� XMFLOAT4x4�� ��ȯ�Ѵ�
	XMFLOAT4X4 pMatrix;
	XMStoreFloat4x4(&pMatrix, projectionMatrix);

	// ����ü���� �ּ� z �Ÿ��� ����Ѵ�
	float zMinimum = -pMatrix._43 / pMatrix._33;
	float r = screenDepth / (screenDepth - zMinimum);

	// ����� ���� �ٽ� ���� ��Ŀ� �����Ѵ�
	pMatrix._33 = r;
	pMatrix._43 = -r * zMinimum;
	projectionMatrix = XMLoadFloat4x4(&pMatrix);

	// ����İ� ��������� ���Ͽ� ����ü ����� �����
	XMMATRIX finalMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);

	// ���� ����� XMFLOAT4x4�� ��ȯ�Ѵ�
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, finalMatrix);

	// ��� ���������� 6���� ���(����ü ������ �ѷ��δ�)�� �����
	// Ư�� ��Ұ� �ش� ���� �ȿ� �ִ��� ���θ� �Ǵ��Ѵ�

	// ����ü ����� ���� �� ����ü ���� ȭ�� ���� ��ǥ�� ��ȯ�ؾ��Ѵ�

	// �� ����� �������͸� �����س���
	// �� ���Ϳ� ����İ� ��������� ���� ����ü ����� ��ġ����� ���ϸ� �ȴ�

	// ����ü�� ����� ����� ���
	float x = (float)(matrix._14 + matrix._13);
	float y = (float)(matrix._24 + matrix._23);
	float z = (float)(matrix._34 + matrix._33);
	float w = (float)(matrix._44 + matrix._43);
	m_planes[0] = XMVectorSet(x, y, z, w);
	m_planes[0] = XMPlaneNormalize(m_planes[0]);

	// ����ü�� �� ����� ���
	x = (float)(matrix._14 - matrix._13);
	y = (float)(matrix._24 - matrix._23);
	z = (float)(matrix._34 - matrix._33);
	w = (float)(matrix._44 - matrix._43);
	m_planes[1] = XMVectorSet(x, y, z, w);
	m_planes[1] = XMPlaneNormalize(m_planes[1]);

	// ����ü�� ���� ����� ���
	x = (float)(matrix._14 + matrix._11);
	y = (float)(matrix._24 + matrix._21);
	z = (float)(matrix._34 + matrix._31);
	w = (float)(matrix._44 + matrix._41);
	m_planes[2] = XMVectorSet(x, y, z, w);
	m_planes[2] = XMPlaneNormalize(m_planes[2]);

	// ����ü�� ������ ����� ����մϴ�.
	x = (float)(matrix._14 - matrix._11);
	y = (float)(matrix._24 - matrix._21);
	z = (float)(matrix._34 - matrix._31);
	w = (float)(matrix._44 - matrix._41);
	m_planes[3] = XMVectorSet(x, y, z, w);
	m_planes[3] = XMPlaneNormalize(m_planes[3]);

	// ����ü�� �� ����� ���
	x = (float)(matrix._14 - matrix._12);
	y = (float)(matrix._24 - matrix._22);
	z = (float)(matrix._34 - matrix._32);
	w = (float)(matrix._44 - matrix._42);
	m_planes[4] = XMVectorSet(x, y, z, w);
	m_planes[4] = XMPlaneNormalize(m_planes[4]);

	// ����ü�� �Ʒ� ����� ���
	x = (float)(matrix._14 + matrix._12);
	y = (float)(matrix._24 + matrix._22);
	z = (float)(matrix._34 + matrix._32);
	w = (float)(matrix._44 + matrix._42);
	m_planes[5] = XMVectorSet(x, y, z, w);
	m_planes[5] = XMPlaneNormalize(m_planes[5]);
	
	return;
}

// �ϳ��� ���� ����ü ���ο� �ִ��� ����
bool FrustumClass::Checkpoint(float x, float y, float z)
{
	int i;
	for (int i = 0; i < 6; i++)
	{
		// ��� �ڿ� ������ ����ü ��� ��
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(x, y, z, 1.0f))) < 0.0f)
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
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f) { continue; }
		
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
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter, yCenter, zCenter, 1.0f))) < -radius)
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
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f) { continue; }
		if (XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f))) >= 0.0f) { continue; }

		return false;
	}

	return true;
}
