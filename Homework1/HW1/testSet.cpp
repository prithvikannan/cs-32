#include "Set.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;
int main()
{

	Set a;
	a.insert("1");
	a.insert("2");
	a.insert("3");

	Set b;
	b.insert("1");
	b.insert("2");
	b.insert("3");
	b.insert("4");
	b.insert("5");
	b.insert("6");
	b.insert("7");


	cout << "before swap " << endl;
	a.dump();
	b.dump();

	cerr << "after swap " << endl;
	a.swap(b);
	a.dump();
	b.dump();

	Set s;
	assert(s.empty());
	ItemType x = "arepa";
	assert(!s.get(42, x) && x == "arepa"); // x unchanged by get failure
	s.insert("chapati");
	assert(s.size() == 1);
	assert(s.get(0, x) && x == "chapati");
	cout << "Passed all tests" << endl;

}