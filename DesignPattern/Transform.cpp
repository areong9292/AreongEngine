
#include "GameObject.h"
#include "Transform.h"

Transform::~Transform()
{
	cout << "del Transform" << endl;
}

// Component의 생성자를 명시적으로 호출해야한다
// 안그러면 기본 생성자가 호출되는데 삭제한 함수여서 에러 발생함
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
