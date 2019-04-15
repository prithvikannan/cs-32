#include "Set.h"
#include <iostream>
using namespace std;

Set::Set() {
	head = nullptr;
}

Set::~Set() {
	Node *p;
	p = head;
	while (p != nullptr) { // loop until the last node and delete each one
		Node *n = p->next; 
		delete p;
		p = n;
	}
}

Set::Set(const Set &src)
{
	head = nullptr;
	Node *ptr = src.head;
	while (ptr != nullptr) { // loop over all nodes and insert the values of src to this set
		insert(ptr->value);
		ptr = ptr->next;
	}
}

Set & Set::operator=(const Set & src)
{
	if (this == &src) { // do nothing if they are the same 
		return *this;
	}
	Node *ptr = head;
	while (ptr != nullptr) { // loop over all the nodes in this set and delete them all
		erase(ptr->value);
		ptr = ptr->next;
	}
	ptr = src.head;
	while (ptr != nullptr) { // loop over all nodes and insert the values of src to this set
		insert(ptr->value);
		ptr = ptr->next;
	}
	return *this;
}

bool Set::empty() const {
	if (size() == 0) {
		return true;
	}
	else {
		return false;
	}
}

int Set::size() const {
	Node *ptr = head;
	int count = 0;
	while (ptr != nullptr) // loop over each node and count number of nodes
	{
		count++;
		ptr = ptr->next;
	}
	return count;
}

bool Set::insert(const ItemType& value) {
	if (contains(value)) { // if node is already in the list, return false
		return false;
	}
	Node *p = new Node;
	// assign the new node to point to the appropriate nodes
	p->next = head; 
	p->previous = nullptr;
	p->value = value;
	if (head != nullptr) {
		head->previous = p;
	}
	head = p;
	return true;
}

bool Set::erase(const ItemType& value) {
	Node *ptr = head;
	while (ptr != nullptr)
	{
		if (ptr->value == value) { // if there is only 1 item
			if (size() == 1) {
				delete ptr;
				head = nullptr;
			}
			else if (ptr == head) { // if at the first item
				Node *temp;
				temp = head;
				head = ptr->next;
				head->previous = nullptr;
				delete temp;
			}
			else if (ptr->next == nullptr) { // if at the last item
				ptr->previous->next = nullptr;
				delete ptr;
			}
			else { // if in the middle
				ptr->next->previous = ptr->previous;
				ptr->previous->next = ptr->next;
				delete ptr;
			}
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

bool Set::contains(const ItemType& value) const {
	Node *ptr = head;
	while (ptr != nullptr) // loop over all nodes
	{
		if (value == ptr->value) {
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

void Set::dump() const {
	Node *ptr = head;
	while (ptr != nullptr) { // loop over all nodes
		cerr << ptr->value << endl;
		ptr = ptr->next;
	}
}

bool Set::get(int pos, ItemType& value) const {
	if (pos < 0 || pos > size()) {
		return false;
	}
	Node *p = head;
	while (p != nullptr) { // loop over all nodes
		int count = 0;
		Node *q = head;
		while (q != nullptr) { // loop over all nodes again 
			if (p->value > q->value) { // add to count if the value in outside loop is greater than value of inside loop
				count++;
			}
			q = q->next;
		}
		if (count == pos) { // if the count is the target, then the value is found
			value = p->value;
			return true;
		}
		p = p->next;
	}
	return false;
}

void Set::swap(Set& other) {
	// switch head pointers
	Node *tempPtr = head; 
	head = other.head;
	other.head = tempPtr;
}

void unite(const Set & s1, const Set & s2, Set & result)
{
	if (&s1 == &result && &s2 == &result) { // if s1 and s2 are the same 
		return;
	}
	if (&s1 == &result) { // if s1 and result are same  then add values from s2 to result
		for (int i = 0; i < s2.size(); i++) {
			ItemType value;
			s2.get(i, value);
			result.insert(value);
		}
	}
	else if (&s2 == &result) { // if s2 and result are same, then just add values from s1 to result
		for (int i = 0; i < s1.size(); i++) {
			ItemType value;
			s1.get(i, value);
			result.insert(value);
		}
	}
	else {
		int j = 0;
		while (j != result.size()) { // clear out result
			ItemType value;
			result.get(j, value);
			result.erase(value);
		}
		for (int i = 0; i < s1.size(); i++) { // add values from s1
			ItemType value
			s1.get(i, value);
			result.insert(value);
		}
		for (int i = 0; i < s2.size(); i++) { // add values from s2
			ItemType value;
			s2.get(i, value);
			result.insert(value);
		}
	}
}

void subtract(const Set & s1, const Set & s2, Set & result)
{
	if (&s1 == &s2) {		// if all 3 are the same, then result should be empty
		int j = 0;
		while (j != result.size()) {
			ItemType value;
			result.get(j, value);
			result.erase(value);
		}
	} else if (&s1 == &result) { // if s1 and result are the same, remove anything from result that exists in s2
		int j = 0;
		while (j != result.size()) {
			ItemType value;
			result.get(j, value);
			if (s2.contains(value)) {
				result.erase(value);
			}
			else {
				j++;
			}
		}
	}
	else if (&s2 == &result) { // if s2 and result are the same, make a copy of result, and subtract s2 from s1 and store in copy, replace result with copy
		for (int i = 0; i < s1.size(); i++) {
			ItemType value;
			s1.get(i,value);
			if (s2.contains(value)) {
				result.erase(value);
			}
			else {
				result.insert(value);
			}
		}
		int j = 0;
		while (j != result.size()) {
			ItemType value;
			result.get(j, value);
			if (!s1.contains(value)) {
				result.erase(value);
			}
			else {
				j++;
			}
		}
	}
	else {
		int j = 0;
		while (j != result.size()) { // clear out result
			ItemType value;
			result.get(j, value);
			result.erase(value);
		}
		for (int i = 0; i < s1.size(); i++) { // add values from s1 that are not in s2
			ItemType value;
			s1.get(i, value);
			if (!s2.contains(value)) {
				result.insert(value);
			}
		}
	}
}
