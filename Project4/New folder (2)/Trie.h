#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED
#include <iostream>
#include <string>
#include <vector>
using namespace std;
const int MAX_CHARS = 127;

template<typename ValueType>
class Trie
{
public:
	Trie();
	~Trie();
	void reset();
	void insert(const std::string& key, const ValueType& value);
	std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

	// C++11 syntax for preventing copying and assignment
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;

private:
	struct Node {
		Node(string label) {
			m_label = label;
			for (int i = 0; i < MAX_CHARS; i++) {
				m_children[i] = nullptr;
			}
		}
		Node* m_children[MAX_CHARS];
		string m_label;
		std::vector <ValueType> m_vals;

	};
	void reset(Node* n);
	void insert(std::string & key, const ValueType & value, Node* & n);
	void find(std::string key, bool exactMatchOnly, std::vector<ValueType> & v, Node * n) const;
	void addValuesFrom(std::vector<ValueType>& v, Node * n) const;

	Node* m_root;
};

#endif // TRIE_INCLUDED

template<typename ValueType>
inline Trie<ValueType>::Trie()
{
	m_root = new Node("");
}

template<typename ValueType>
inline Trie<ValueType>::~Trie()
{
	reset();
	delete m_root;
}

template<typename ValueType>
inline void Trie<ValueType>::reset()
{
	reset(m_root); // call the overload to recursively traverse the trie and reset
	m_root = new Node("");
}

template<typename ValueType>
inline void Trie<ValueType>::reset(Node * n)
{
	if (n == nullptr) {
		return;
	}
	for (int i = 0; i != MAX_CHARS; i++) {
		reset(n->m_children[i]);
	}
	delete n;

}

template<typename ValueType>
inline void Trie<ValueType>::insert(const std::string & key, const ValueType & value)
{
	std::string editableKey = key; // create an editable copy of key so that we can cut off the first letter during recursion
	insert(editableKey, value, m_root->m_children[editableKey[0]]); // call the overload to recursively traverse the trie and insert
}

template<typename ValueType>
inline void Trie<ValueType>::insert(std::string & key, const ValueType & value, Node* & n)
{
	if (key.size() == 0)
	{
		return;
	}
	if (n == nullptr) // if the node for this letter does not already exist
	{
		n = new Node(key.substr(0, 1)); 
	}
	if (key.size() == 1) // if we are at the end of the key, we also want to save the value
	{
		n->m_vals.push_back(value);
		return;
	}
	key = key.substr(1);
	insert(key, value, n->m_children[key[0]]);
}

template<typename ValueType>
inline std::vector<ValueType> Trie<ValueType>::find(const std::string & key, bool exactMatchOnly) const
{
	std::vector<ValueType> v;
	std::string editableKey = key; // create an editable copy of key so that we can cut off the first letter during recursion
	find(editableKey, exactMatchOnly, v, m_root->m_children[editableKey[0]]); // call the overload to recursively traverse the trie and find
	return v;
}

template<typename ValueType>
inline void Trie<ValueType>::find(std::string key, bool exactMatchOnly, std::vector<ValueType>& v, Node * n) const
{
	if (n == nullptr) {
		return;
	}
	if (key.size() == 0) { 
		return;
	}
	if (key.substr(0, 1) == n->m_label) { // the current node matches the string
		if (key.size() == 1) // if we are at a letter with a value
		{
			addValuesFrom(v, n); // copy the values from the node to the output vector
			return;
		}
		for (int i = 0; i != MAX_CHARS; i++) // recursively check the next layer
		{
			find(key.substr(1), exactMatchOnly, v, n->m_children[i]);
		}

	}
	else if (exactMatchOnly == false) { // the current node does not match the string, but a mistake is allowed
		if (key.size() == 1) // if we are at a letter with a value
		{
			addValuesFrom(v, n); // copy the values from the node to the output vector
			return;
		}
		exactMatchOnly = true; // since the one mismatch has already been found, from this point an exact match is needed
		for (int i = 0; i != MAX_CHARS; i++)
		{
			find(key.substr(1), exactMatchOnly, v, n->m_children[i]); // recursively check the next layer
		}
	}
	else { // the current node does not match the string, and a mistake is not allowed
		return;
	}
}

template<typename ValueType>
inline void Trie<ValueType>::addValuesFrom(std::vector<ValueType>& v, Node * n) const
{
	for (int i = 0; i != n->m_vals.size(); i++) // copy the values from the node to the output vector
	{
		v.push_back(n->m_vals[i]);
	}
}



