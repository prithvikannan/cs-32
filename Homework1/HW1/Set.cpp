#include "Set.h"
#include <iostream>
#include <algorithm>
using namespace std;

Set::Set() {
	numItems = 0;
} // Create an empty set (i.e., one with no items).

bool Set::empty() const {
	if (numItems == 0) {
		return true;
	}
	else {
		return false;
	}
} // Return true if the set is empty, otherwise false.

int Set::size() const {
	return numItems;

}    // Return the number of items in the set.

bool Set::insert(const ItemType& value) {
	if (numItems >= DEFAULT_MAX_ITEMS) {
		return false;
	}
	if (contains(value)) {
		return false;
	}
	items[numItems] = value;
	numItems++;
	return true;
}
// Insert value into the set if it is not already present.  Return
// true if the value was actually inserted.  Leave the set unchanged
// and return false if the value was not inserted (perhaps because it
// was already in the set or because the set has a fixed capacity and
// is full).

bool Set::erase(const ItemType& value) {
	for (int i = 0; i < numItems; i++) {
		if (value == items[i]) {
			for (int j = i; j < numItems - 1; j++) { // shifts 1 to the left
				items[j] = items[j + 1];
			}
			numItems--;
			return true;
		}
	}
	return false;
}
// Remove the value from the set if present.  Return true if the
// value was removed; otherwise, leave the set unchanged and
// return false.

bool Set::contains(const ItemType& value) const {
	for (int i = 0; i < numItems; i++) {
		if (value == items[i]) {
			return true;
		}
	}
	return false;
}
// Return true if the value is in the set, otherwise false.

bool Set::get(int i, ItemType& value) const {
	if (i >= 0 && i < size()) {
		for (int j = 0; j < size(); j++) {
			int count = 0;
			for (int k = 0; k < size(); k++) {
				if (j != k) { // not comparing to itself
					if (items[j] > items[k]) {
						count++;
					}
				}
			}
			if (count == i) {
				value = items[j];
				return true;

			}
		}
	}
	return false;
}
// If 0 <= i < size(), copy into value the item in the set that is
// strictly greater than exactly i items in the set and return true.
// Otherwise, leave value unchanged and return false.

void Set::swap(Set& other) {
	std::swap(*this, other);

}

void Set::dump() const {
	cerr << "Contents of set: ";
	for (int i = 0; i < numItems; i++) {
		cerr << items[i] + " ";
	}
	cerr << endl;
	cerr << "Size of set: " + to_string(size()) << endl;
}
// Exchange the contents of this set with the other one.
