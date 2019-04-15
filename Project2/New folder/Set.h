#ifndef Set_h
#define Set_h
#include <string>
using namespace std;
typedef string ItemType;

class Set
{
public:
	Set();
	~Set();
	Set(const Set &src);
	Set &operator=(const Set &src);
	bool empty() const;
	int size() const;
	bool insert(const ItemType& value);
	bool erase(const ItemType& value);
	bool contains(const ItemType& value) const;
	bool get(int pos, ItemType& value) const;
	void swap(Set& other);
	void dump() const;
private:
	struct Node
	{
		ItemType value;
		Node* next;
		Node* previous;
	};
	Node *head;
};

void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);


#endif