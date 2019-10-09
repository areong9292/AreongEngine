#include "Singleton.h"
#include "Component.h"
int main()
{
	Component* ss1;
	Component* ss2;

	ss1 = Component::GetInstance();

	ss1->setVal(2);
	ss1->test();

	ss2 = Component::GetInstance();
	ss2->test();
	ss2->setVal(3);

	ss1->test();

	std::cout << ss1 << std::endl;
	std::cout << ss2 << std::endl;

	Component::GetInstance()->test();

	return 0;
}