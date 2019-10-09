#pragma once

#include "Singleton.h"
#include <iostream>

class Component : public Singleton<Component>
{
public:
	Component();
	~Component();

	Component* testObject;

	void test() { std::cout << "test : " << _val << std::endl; }

	void setVal(int val) { _val = val; }
	int getVal() { return _val; }

private:
	int _val = 1;
};

