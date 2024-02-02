//============================================================================
// Name        : ICS_313_Self_Model_CPP.cpp
// Author      : Liam Tapper
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Object.h"
using namespace std;

int main() {
	Object obj1, obj2;
	obj1 = Object(42);
	obj2 = Object();

	obj1.assignParentSlot("obj2", obj2);
	return 0;
}
