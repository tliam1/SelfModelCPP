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
  isPrimitiveFunction = false;
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
    return copy;
  }
  else if (isPrimitiveFunction) {
    copy.isPrimitiveFunction = true;
    copy.isPrimitiveValue = true;
    copy.evaluate();
    PrimitiveValue pValClone;
    int preVal = copy.pVal.i;
    pValClone.i = performPrimitiveFunction(copy);
    copy.pVal = pValClone;

    //cout << "EVALUATE IsPrimitiveFunction: Arithmetic Result of Primitive Function: " << preVal << " * " << preVal << " = " << copy.pVal.i << endl;
    return copy;
  } else if (!msg.empty()) {
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
          print();
          // copy = slot.reference;
          lastResult = copy;
        }else if (message.message == "parameter" && message.function == "performPrimitiveFunction"){
          copy.isPrimitiveFunction = true;
          if(copy.pVal.i == -1){
            copy.pVal = message.param->pVal;
          }

          copy.pVal.i = copy.evaluate().pVal.i;
        }
        lastResult = copy;
        iterator++;
      }
      if(copy.isPrimitiveFunction){
        cout << "EVALUATE: Arithmetic Result of Primitive Function (sqr operations): " << copy.pVal.i << endl;
      }
      return lastResult;
    }else{
      cout << "EVALUATE: Error msg list is empty" << endl;
    }
  return copy;
}

/*
 *  given an object (this), return a copy of it
 */
Object Object::copy() const{
  Object copy;
  copy.slots = this->slots;
  copy.pVal = this->pVal;
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
  assert("Parent Not FOund");
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
        newMsg.param = parameter;

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
  assert("Parent Not FOund");
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

/*
 * given an object (this), a string,
 * and an object, call assignSlot then makeParent.
 */
void Object::assignParentSlot(const string& name, Object* reference){
    assignSlot(name, reference);
    makeParent(name);
}

void Object::print() const{
  cout << "" << endl;
  cout << "" << endl;
  cout << "" << endl;
  cout << "STARTING PRINT DUMP MESSAGE" << endl;
  cout << "Slots" << endl;
  int i = 0;
  for (auto& slot : slots) {
      cout << "Slot " << i << " with name " << slot.name << " has reference to " << slot.reference << endl;
      i++;
  }
  cout << "" << endl;
  cout << "Messages" << endl;
  i=0;
  for (const auto& message : msg) {
    cout << "message " << i << " with name " << message.message << " has reference to function (default: ~): " << message.function << endl;
    i++;
  }
  cout << "" << endl;
  cout << "Data Value" << endl;
  cout << "Has data value (-1 = default): " << pVal.i << endl;
  cout << "ENDING PRINT DUMP MESSAGE" << endl;
  cout << "" << endl;
  cout << "" << endl;
  cout << "" << endl;
}

/*
 * OLD CAN REMOVE
 */
Object Object::evaluateSlot(const Slot& slot) const {
    return slot.reference->evaluate();
}

/*
 * this is my primitive function
 */
int Object::performPrimitiveFunction(const Object& obj) const{
	return obj.pVal.i * obj.pVal.i;
}

