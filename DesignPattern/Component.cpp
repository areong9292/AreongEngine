#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* gameObject) : gameObject(gameObject), transform(gameObject->transform)
{
}

Component::~Component()
{
	cout << "component delete" << endl;
}

void Component::setOwner(GameObject * pOwner)
{
}

GameObject * Component::GetOwner() const
{
	return m_pOwner;
}
