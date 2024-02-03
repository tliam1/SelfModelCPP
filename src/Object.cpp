/*
 * Object.cpp
 *
 *  Created on: Feb 1, 2024
 *      Author: liamk
 */

#include "Object.h"
/*
 * can implement slot searches with a hash table
 */
Object::Object(){
  slots.clear();
  pVal=PrimitiveValue();
  isPrimitiveValue = false;
}

Object::Object(int val){
  slots.clear();
  pVal.i = val;
  pVal.usingBool = false;
  isPrimitiveValue = true;
}
Object::Object(float val){
  slots.clear();
  pVal.f = val;
  pVal.usingBool = false;
  isPrimitiveValue = true;
}
Object::Object(bool val){
  slots.clear();
  pVal.b = val;
  pVal.usingBool = true;
  isPrimitiveValue = true;
}
Object::Object(char val){
  slots.clear();
  pVal.c = val;
  pVal.usingBool = false;
  isPrimitiveValue = true;
}

Object::Object(const Object& obj){
  slots.clear();
  slots = obj.slots;
  pVal = obj.pVal;
  isPrimitiveValue = obj.isPrimitiveValue;

}

Object Object::evaluate() {
  /*
   * given an object (this), evaluate it and return the result.
   * If the object has a list of messages,
   * the object copies itself, sends the messages to the copy, and returns the last result.
   */
  Object copy = this->copy();
  if (isPrimitiveValue) {
    copy.isPrimitiveValue = true;
  }
  else if (isPrimitiveFunction) {
    copy.isPrimitiveFunction = true;
    PrimitiveValue pValClone;
    int preVal = copy.pVal.i;
    pValClone.i = performPrimitiveFunction(copy);
    copy.pVal = pValClone;
    cout << "EVALUATE IsPrimitiveFunction: Arithmetic Result of Primitive Function: " << preVal << " * " << preVal << " = " << copy.pVal.i << endl;
    return copy;
  }
  if (!msg.empty()) {
    Object lastResult;
    int iterator = 0;
    for (const auto& message : msg) {
      if (message.message == "clear"){
           msg.clear();
           cout << "EVALUATE MESSAGE: CLEARED" << endl;
           return lastResult;
      }
    }
    for (const auto& message : msg) {
        /* Print the parameter's primitive data value
        * there would be more functionality (I.E. basically function defs)
        * but I don't have time to more a bunch
        */

        // TODO: INSTEAD OF CHECKING LIKE THIS, FIND A WAY TO CALL THE FUNCTION USING THE STRING
        if(message.message == "print" || message.function == "print"){
          cout << "Printing slot pVal: " << pVal.i << endl;
          // copy = slot.reference;
          lastResult = copy;
        }else if (message.message == "parameter" && message.function == "performPrimitiveFunction"){
          copy.isPrimitiveFunction = true;
          PrimitiveValue pValClone;
          int preVal = copy.pVal.i;
          pValClone.i = performPrimitiveFunction(copy);
          copy.pVal = pValClone;
          cout << "EVALUATE MESSAGE: " << ": Arithmetic Result of Primitive Function: " << preVal << " * " << preVal << " = " << copy.pVal.i << endl;
          //copy = slot.reference;
        }
        lastResult = copy;
        iterator++;
      }
      return lastResult;
    }else{
      cout << "EVALUATE: Error msg list is empty" << endl;
    }
  return copy;
}


Object Object::copy() const{
  /*
   *  given an object (this), return a copy of it
   */
  Object copy;
  copy.slots = this->slots;
  copy.pVal = this->pVal;
  // cout << "COPY: Copied pVal.i: " << copy.pVal.i << " pVal.f: " << copy.pVal.f << endl;
  copy.msg = this->msg;
  return copy;

}

/*
 *  given an object and a string, send the message
 *  to the object. The object corresponding
 *  to the message
 *  (I.e., in the slot with the same name as the message)
 *  is evaluated and returned.
 *  If the object doesn’t directly have a slot with that name,
 *  recursively look in the parent slots via a breadth-first search.
 */
Object Object::sendAMessage(const string& message) const{
  queue<const Object*> bfsQueue;
  bfsQueue.push(this);
  while (!bfsQueue.empty()) {
    const Object* currentObject = bfsQueue.front();
    bfsQueue.pop();


    for (const auto& slot : currentObject->slots) {
      if (slot.name == message) {
        // If found, evaluate and return the corresponding object
        Messages newMsg;
        newMsg.message = message;
        slot.reference->msg.push_back(newMsg);
        cout << "SendAMessage: Found matching slot name. Returning Evaluated slot ref" << endl;
        return slot.reference->evaluate();
      }
    }

    // If not found, enqueue parent
    for (const auto& slot : currentObject->slots) {
      cout << "Searching Parent Slots" << endl;
      if(slot.parent)
        bfsQueue.push(slot.reference);
    }
  }
  // idk what to return if we find nothing
  cout << "SendAMessage: Error! Nothing Found" << endl;
  return Object();
}

/*
 * given an object(this) and a string and a second object (the “parameter”),
 * send the message to the first object,
 * passing the second object as a parameter to the message by setting the “parameter” slot on the object.
 * If the object doesn’t directly have a slot with that name, recursively look in the parent slots via a breadth-first search.
 */

Object Object::sendAMessageWithParameters(const string& message, Object* parameter) const{
  queue<const Object*> bfsQueue;
  bfsQueue.push(this);

  while (!bfsQueue.empty()) {
    const Object* currentObject = bfsQueue.front();
    bfsQueue.pop();

    for (auto& slot : currentObject->slots) {
      if (slot.name == message) {
        // Set the "parameter" slot
        Messages newMsg;
        newMsg.message = "parameter";
        newMsg.function = "performPrimitiveFunction";

        slot.reference->msg.push_back(newMsg);
        cout << "sendAMessageWithParameters:Message matched slot name -> creating parameter slot" << endl;
        // Evaluate and return the corresponding object
        return slot.reference->evaluate();
      }
    }

    // If not found, enqueue parent
    for (const auto& slot : currentObject->slots) {
      if(slot.parent){
        bfsQueue.push(slot.reference);
        cout << "sendAMessageWithParameters: Searching Parent slot reference" << endl;
      }
    }
  }
  cout << "SendMessageWithParameters: Error! Nothing Found" << endl;
  return Object(); // i still dont know what to return if nothing is found
}
void Object::assignSlot(const string& name, Object* reference){
  /* loop through all slots in object or use hash lookup,
   * if name is found in one of the slots
   * set the slot reference to the second object
   */
  for (auto& slot : slots) {
    if (slot.name == name) {
      // Handle case where the slot already exists
      cout << "ASSIGN SLOT: Slot with name " << name << " Exists. Changing the reference!" << endl;
      slot.reference = reference;
      return;
    }
  }
  cout << "Slot with name " << name << " does not exist. Making new slot!" << endl;
  slots.push_back({name, reference, false});
}
void Object::makeParent(const string& name){
  /* given an object (this) and a string, designate
   * the slot named by the string (if it exists) as a parent slot.
   */
    for (auto& slot : slots) {
        if (slot.name == name) {
            slot.parent = true;
            cout << "Slot with name " << slot.name << " is now a parent" << endl;
            return;
        }
    }
    cout << "Can't make parent as slot does not exist" << endl;
}
void Object::assignParentSlot(const string& name, Object* reference){
  /*
   * given an object (this), a string,
   * and an object, call assignSlot then makeParent.
   */
    assignSlot(name, reference);
    makeParent(name);
}
void Object::print() const{
	// TODO
}

Object Object::evaluateSlot(const Slot& slot) const {
    return slot.reference->evaluate();
}

int Object::performPrimitiveFunction(const Object& obj) const{
  cout << "Performing Primitive Function" << endl;
	return obj.pVal.i * obj.pVal.i;
}

