#include "GameObject.h"
#include "Transform.h"

BEGIN_COMPONENT_INIT(Transform);

// Component�� �����ڸ� ��������� ȣ���ؾ��Ѵ�
// �ȱ׷��� �⺻ �����ڰ� ȣ��Ǵµ� ������ �Լ����� ���� �߻���
Transform::Transform(GameObject* gameObject) : Component(gameObject)
{
}

Transform::~Transform()
{
}

void Transform::onCreate()
{
}

void Transform::onDestroy()
{
}

void Transform::UpdateComponent()
{
}