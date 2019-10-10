#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <iostream>
using namespace std;

class GameObject;
class Transform;
class Component
{
public:
	Component() = delete;
	Component(GameObject* gameObject);
	virtual ~Component();

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	void setOwner(GameObject* pOwner);
	GameObject* GetOwner() const;

	GameObject* const gameObject;
	Transform* const transform;

	// 모든 컴포넌트는 Update를 가지며
	// 매 프레임마다 컴포넌트의 동작을 처리한다
	void Update()
	{
		UpdateComponent();
	}

private:
	// 모든 컴포넌트 update 처리하는 가상함수
	virtual void UpdateComponent() = 0;

	// 해당 컴포넌트를 가지고 있는 게임 오브젝트 정보
	GameObject* m_pOwner;
};

#endif // !COMPONENT_H_