
#include "GameObject.h"
#include "Transform.h"

Transform::~Transform()
{
}

// Component�� �����ڸ� ��������� ȣ���ؾ��Ѵ�
// �ȱ׷��� �⺻ �����ڰ� ȣ��Ǵµ� ������ �Լ����� ���� �߻���
Transform::Transform(GameObject* gameObject) : Component(gameObject)
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
