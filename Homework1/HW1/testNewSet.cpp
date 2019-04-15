#include "newSet.h"
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

	Set a(1000);   // a can hold at most 1000 distinct items
	Set b(5);      // b can hold at most 5 distinct items
	Set c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
	ItemType v[6] = {"adf","dsfsd","dfsd", "sfg", "sdfd", "wer"};

	// No failures inserting 5 distinct items into b
	for (int k = 0; k < 5; k++)
		assert(b.insert(v[k]));

	// Failure if we try to insert a sixth distinct item into b
	assert(!b.insert(v[5]));

	// When two Sets' contents are swapped, their capacities are swapped
	// as well:
	a.swap(b);
	assert(!a.insert(v[5]) && b.insert(v[5]));
	cerr << "Passed all tests" << endl;

}
