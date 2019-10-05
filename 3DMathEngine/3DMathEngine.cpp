#include <iostream>
#include "Math/AreongMath.h"

using namespace AreongEngine;

int main()
{
	cout << endl;

	Matrix m(2,1,4,0,
				3,4,1,0,
				2,1,1,0,
				0,0,0,1);

	cout << endl;

	Matrix n = m.getInverseMatrix();
	
	n.show();
	//n.show();
	return 0;
}
