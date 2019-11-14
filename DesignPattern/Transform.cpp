#include "GameObject.h"
#include "Transform.h"

BEGIN_COMPONENT_INIT(Transform);

// Component의 생성자를 명시적으로 호출해야한다
// 안그러면 기본 생성자가 호출되는데 삭제한 함수여서 에러 발생함
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