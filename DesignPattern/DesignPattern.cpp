#include "Singleton.h"
#include "Transform.h"
#include "GameObject.h"
int main()
{
	GameObject* testComponent = new GameObject();

	delete testComponent;
	return 0;
}