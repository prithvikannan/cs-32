
#include "SSNSet.h"
#include <iostream>

using namespace std;

SSNSet::SSNSet() {

}        // Create an empty SSN set.

bool SSNSet::add(unsigned long ssn) {
	return SSNs.insert(ssn);
}
// Add an SSN to the SSNSet.  Return true if and only if the SSN
// was actually added.

int SSNSet::size() const {
	return SSNs.size();
}  // Return the number of SSNs in the SSNSet.

void SSNSet::print() const {
	ItemType a;
	for (int i = 0; i < SSNs.size(); i++) {
		SSNs.get(i, a);
		cout << a << endl;
	}
}
// Write every SSN in the SSNSet to cout exactly once, one per
// line.  Write no other text.

