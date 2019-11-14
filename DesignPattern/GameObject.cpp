#include "Component.h"
#include "GameObject.h"

GameObject::GameObject(): transform(new Transform(this))
{
	_components.push_back(transform);
}

GameObject::~GameObject()
{
	releaseComponents();
}

list<Component*> GameObject::getComponents()
{
	return _components;
}

void GameObject::releaseComponents()
{
	for (auto component : _components)
	{
		component->onDestroy();
	}

	for (auto component : _components)
	{
		delete component;
	}
}