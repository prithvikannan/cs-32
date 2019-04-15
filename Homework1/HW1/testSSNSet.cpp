#include <iostream>
#include "SSNSet.h"

using namespace std;
int main() {
	SSNSet a;
	a.add(234);
	a.add(4453);
	a.add(345);
	a.add(333);
	a.add(453);
	cout << a.size() << endl;
	a.print();

	cout << endl;

	SSNSet b;
	b.add(123);
	b.add(456);
	b.add(789);
	cout << b.size() << endl;
	b.print();

	cout << endl;

	SSNSet c(b);
	cout << c.size() << endl;
	c.print();

	cout << "reached" << endl;

}
