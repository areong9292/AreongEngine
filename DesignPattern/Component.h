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

	// ��� ������Ʈ�� Update�� ������
	// �� �����Ӹ��� ������Ʈ�� ������ ó���Ѵ�
	void Update()
	{
		UpdateComponent();
	}

private:
	// ��� ������Ʈ update ó���ϴ� �����Լ�
	virtual void UpdateComponent() = 0;

	// �ش� ������Ʈ�� ������ �ִ� ���� ������Ʈ ����
	GameObject* m_pOwner;
};

#endif // !COMPONENT_H_