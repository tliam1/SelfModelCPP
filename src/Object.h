/*
 * Object.h
 *
 *  Created on: Jan 30, 2024
 *      Author: liamk
 */

#ifndef OBJECT_H_
#define OBJECT_H_
#pragma once
#include <iostream>
#include <cstring>
#include <cassert>  // For assert()
#include <vector>
using namespace std;


class Object;
struct Slot {
    string name;
    Object* reference;
    bool parent;
};

struct PrimitiveValue{
	// Default Values
	// only one used is integers for the
	// sake of time
	char c = '~';
	int i = -1;
	float f = -1;
	bool b = false;
	bool usingBool = false;
};

struct Messages{
	string message;
};
/*
 * TODO: A lot of these functions
 * can be derived from a higher class
 * and be abstract functions!!!
 */
class Object{
	Object();
	Object(int val);
	Object(float val);
	Object(bool val);
	Object(char val);
public:
	vector<Slot> slots;
	vector<Messages> msg;
	/*
	 * Obj may rep a primitve data val (num)
	 * obj my rep a primitive function
	 */
	PrimitiveValue pVal;
    Object evaluate() const;
    Object copy() const;
    Object sendAMessage(const string& message) const;
    Object sendAMessageWithParameters(const string& message, const Object& parameter) const;
    void assignSlot(const string& name, const Object& reference);
    void makeParent(const string& name);
    void assignParentSlot(const string& name, const Object& reference);
    void print() const;
    Object evaluateSlot(const Slot& slot) const;
    bool isPrimitiveValue = false;
    bool isPrimitiveFunction = false;
    void performPrimitiveFunction(const Object& obj) const;
protected:

private:

};



#endif /* OBJECT_H_ */
