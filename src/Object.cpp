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

Object Object::evaluate() const{
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
	performPrimitiveFunction(copy);
	return copy;
	}
	if (!slots.empty()) {
		for (const auto& slot : slots) {
				copy = copy.evaluateSlot(slot);
		}
	}
	if (!msg.empty()) {
	Object lastResult;
	for (const auto& message : msg) {
	for (const auto& slot : slots) {
		if (message.message == slot.name) {
			/* Print the parameter's primitive data value
			* there would be more functionality (I.E. basically function defs)
			* but I don't have time to more a bunch
			*/
			if(message.message == 'print'){
				cout << "Printing: " << slot.reference->pVal.i << endl;
				lastResult = copy;
			}
		}
	}
	
	}
	return lastResult;
	}
	return copy;
}


Object Object::copy() const{
	/*
	 *  given an object (this), return a copy of it
	 */
	Object copy = new Object();
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
    for (const auto& slot : slots) {
        if (slot.name == message) {
            return evaluateSlot(slot);
        }
    }

    for (const auto& slot : slots) {
        if (slot.parent) {
            Object parentResult = slot.reference->sendAMessage(message);
            if (parentResult.pVal.i != -1 || parentResult.pVal.f != -1 || parentResult.pVal.usingBool) {
                return parentResult;
            }
        }
    }

    return Object();  // I dont know what to return here
}

/*
 * given an object(this) and a string and a second object (the “parameter”),
 * send the message to the first object,
 * passing the second object as a parameter to the message by setting the “parameter” slot on the object.
 * If the object doesn’t directly have a slot with that name, recursively look in the parent slots via a breadth-first search.
 */

Object Object::sendAMessageWithParameters(const string& message, const Object& parameter) const{
    for (const auto& slot : slots) {
        if (slot.name == message) {
            Object copy = *this;
            copy.assignSlot("parameter", parameter);
            return copy.evaluateSlot(slot);
        }
    }

    for (const auto& slot : slots) {
         if (slot.parent) {
             Object parentResult = slot.reference->sendAMessageWithParameters(message, parameter);
             if (parentResult.pVal.i != -1 || parentResult.pVal.f != -1 || parentResult.pVal.usingBool) {
                 return parentResult;
             }
         }
     }
    return Object(); // i still dont know what to return if nothing is found
}
void Object::assignSlot(const string& name, const Object& reference){
	/* loop through all slots in object or use hash lookup,
	 * if name is found in one of the slots
	 * set the slot reference to the second object
	 */
    for (auto& slot : slots) {
        if (slot.name == name) {
            // Handle case where the slot already exists
            slot.reference = new Object(reference);
            return;
        }
    }
	slots.push_back({name, new Object(reference), false});
}
void Object::makeParent(const string& name){
	/* given an object (this) and a string, designate
	 * the slot named by the string (if it exists) as a parent slot.
	 */
    for (auto& slot : slots) {
        if (slot.name == name) {
            slot.parent = true;
            return;
        }
    }
    cout << "Can't make parent as slot does not exist" << endl;
}
void Object::assignParentSlot(const string& name, const Object& reference){
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

void Object::performPrimitiveFunction(const Object& obj) const{
	pVal.i = obj.pVal.i * obj.pVal.i;
	return;
}

