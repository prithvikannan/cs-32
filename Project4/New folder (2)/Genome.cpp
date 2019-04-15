#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
	GenomeImpl(const string& nm, const string& sequence);
	static bool load(istream& genomeSource, vector<Genome>& genomes);
	int length() const;
	string name() const;
	bool extract(int position, int length, string& fragment) const;
private:
	int m_size;
	string m_name;
	string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	m_name = nm;
	m_sequence = sequence;
	m_size = m_sequence.size();
	// This compiles, but may not be correct
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
	// Example 2 - read and process each line of a file until end
	string name = "";
	string sequence = "";

	string s;
	// getline returns infile; the while tests its success/failure state
	while (getline(genomeSource, s))
	{
		if (s[0] == '>') {
			if (s.substr(1).size() == 0) { // has greater than but no other characters
				return false;
			}
			if (name != "" && sequence == "") { // no sequence after the name line
				return false;
			}
			if (sequence.size() != 0) {
				genomes.push_back(Genome(name, sequence));
				name = "";
				sequence = "";
			}
			name = s.substr(1);
		}
		else {
			for (int i = 0; i < s.size(); i++) {
				sequence += toupper(s[i]); // convert to uppercase
				switch (toupper(s[i])) { // check if A,C,G,N,T
				case 'A':
					break;
				case 'C':
					break;
				case 'G': 
					break;
				case 'N':
					break;
				case 'T':
					break;
				default:
					return false;
				}
			}
		}
	}
	if (sequence.size() != 0) {
		genomes.push_back(Genome(name, sequence));
		name = "";
		sequence = "";
	}

	return true;
}

int GenomeImpl::length() const
{
	return m_size;  // This compiles, but may not be correct
}

string GenomeImpl::name() const
{
	return m_name;  // This compiles, but may not be correct
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (position + length > m_size) {
		return false;
	}
	else {
		fragment = m_sequence.substr(position, length);
		return true;
	}
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
	m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
	delete m_impl;
}

Genome::Genome(const Genome& other)
{
	m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
	GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
	delete m_impl;
	m_impl = newImpl;
	return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
	return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
	return m_impl->length();
}

string Genome::name() const
{
	return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
	return m_impl->extract(position, length, fragment);
}
