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

/*
 * What this does:
 * 1) Creates a few objects and forms a cyclic graph
 *    - Hooks up connections
 *      - parent and child
 * 2) Sends a message with parameters to a direct child obj3->obj2
 * 3) Sends a message with an indirect link:
 *    - obj4 does not have same name in slot
 *    - obj4 looks for parents
 *        - obj4 finds obj3 with slot
 *    - Sends message
 */

int main() {
  Object obj1 = Object((int)42);
  Object obj2 = Object();
  Object obj3 = Object();
  Object obj4 = Object();

  obj2.assignSlot("print", &obj1);
  obj3.assignParentSlot("parent", &obj2);
  obj3.assignSlot("squareOp", &obj1);
  obj1.assignSlot("tailSqrOp", &obj4);
  obj4.assignParentSlot("parent", &obj3);
  obj2.assignSlot("clear", &obj1);
  obj2.sendAMessage("print");
  obj2.sendAMessage("clear");
  // squares 42
  obj3.sendAMessageWithParameters("squareOp", &obj1);
  obj2.sendAMessage("clear"); // clears obj1 message vector
  // this way requires a BTS through parents
  // idk if this counts as a 'loop' but it will send mult messages which will
  // get 42*42 = res then res * res = 2nd res
  obj4.sendAMessageWithParameters("squareOp", &obj1);
  obj4.sendAMessageWithParameters("squareOp", &obj1);
  obj2.sendAMessage("clear");
  //one for time for good measure
  obj4.sendAMessageWithParameters("squareOp", &obj1);
	return 0;
}
