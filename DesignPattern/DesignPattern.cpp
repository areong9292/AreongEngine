#include "Singleton.h"
#include "Transform.h"
#include "GameObject.h"
int main()
{
	GameObject* testComponent = new GameObject();

	for (auto component : testComponent->getComponents())
	{
		cout << component->getTypeName();
	}

	delete testComponent;
	return 0;
}