
#include "GameObject.h"
#include "Transform.h"

Transform::~Transform()
{
	cout << "del Transform" << endl;
}

// Component�� �����ڸ� ��������� ȣ���ؾ��Ѵ�
// �ȱ׷��� �⺻ �����ڰ� ȣ��Ǵµ� ������ �Լ����� ���� �߻���
Transform::Transform(GameObject* gameObject) :
	Component(gameObject)
{
	cout << "make Transform" << endl;
}

void Transform::onCreate()
{
	cout << "Init Transform"<<endl;
}

void Transform::onDestroy()
{
	cout << "destroy in transform resource" << endl;
}

void Transform::UpdateComponent()
{
	cout << "transform loop" << endl;
}
