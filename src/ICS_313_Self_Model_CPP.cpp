//============================================================================
// Name        : ICS_313_Self_Model_CPP.cpp
// Author      : Liam Tapper
// Version     :
// Copyright   :
// Description : SELF Model Demonstration in C++
//============================================================================

#include <iostream>
#include "Object.h"
using namespace std;

int main() {
  Object obj1 = Object((int)42);
  Object obj2 = Object();
  Object obj3 = Object();
  Object obj4 = Object();

  obj2.assignSlot("print", &obj3);
  obj3.assignParentSlot("print", &obj2);
  obj3.assignSlot("squareOp", &obj1);
  obj1.assignSlot("tailSqrOp", &obj4);
  obj4.assignParentSlot("parent", &obj3);

  obj2.sendAMessage("print");
  obj3.sendAMessageWithParameters("squareOp", &obj1);

  obj4.sendAMessageWithParameters("squareOp", &obj1);
	return 0;
}
