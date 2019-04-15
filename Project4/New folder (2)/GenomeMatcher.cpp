#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include "Trie.h"
using namespace std;
class Genome;

class GenomeMatcherImpl
{
public:
	GenomeMatcherImpl(int minSearchLength);
	void addGenome(const Genome& genome);
	int minimumSearchLength() const;
	bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
	bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	int m_minSearchLength;
	Trie<DNAMatch> t;
	int m_numGenomes;
	vector<Genome> m_genomes;
	unordered_map<string, int> nameToPosition;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	m_minSearchLength = minSearchLength;
	m_numGenomes = 0;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	m_genomes.push_back(genome);
	for (int i = 0; i < genome.length(); i++) {
		string s;
		if (genome.extract(i, m_minSearchLength, s)) {
			DNAMatch d;
			d.genomeName = genome.name();
			d.position = i;
			d.length = m_minSearchLength;
			t.insert(s, d);
			nameToPosition.insert(pair<string, int>(genome.name(), m_numGenomes));
		}
	}
	m_numGenomes++;
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return m_minSearchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	unordered_map<string, int> lengthOfLongestInGenome;
	unordered_map<string, int> posOfLongestInGenome;
	cout << "searching for " << fragment.substr(0, m_minSearchLength) << endl;
	vector<DNAMatch> hits = t.find(fragment.substr(0, m_minSearchLength), exactMatchOnly);
	cerr << "found " << hits.size() << " hits" << endl;
	if (fragment.size() < minimumLength) { // fragment's length is less than minimumLength, or
		return false;
	}
	if (minimumLength < m_minSearchLength) { // minimumLength is less than the minSearchLength value 
		return false;
	}
	if (hits.size() == 0) { // no matches between fragment and any segment of any genome in the GenomeMatcher object's library.
		return false;
	}
	for (int i = 0; i != hits.size(); i++) { // loop through each hit
		bool mistakeNotAllowed = exactMatchOnly; // create temp variable to manage mismatches on this hit
		unordered_map<string, int>::const_iterator it = nameToPosition.find(hits[i].genomeName);
		if (it == nameToPosition.end()) {
			continue;
		}
		int j = it->second;
		cout << "hit num " << i << " corresponds to genome " << j << endl;
		if (m_genomes[j].name() == hits[i].genomeName) { // if this hit and genome are matching
			cerr << "	hit num " << i << endl;
			int count = 0;
			string maxFrag;
			if (!m_genomes[j].extract(hits[i].position, fragment.size(), maxFrag) && !m_genomes[j].extract(hits[i].position, m_genomes.size() - hits[i].position, maxFrag)) { // extract the longest possible match
				break;
			}
			cerr << "	max fragment is " << maxFrag << endl;
			cerr << "	the fragment is " << fragment << endl;
			for (int k = 0; k != maxFrag.size(); k++) {
				if (maxFrag[k] == fragment[k]) {	// if the letters match up
					cerr << "	found matching letter" << endl;
					count++;
				}
				else if (mistakeNotAllowed == false) {	// if the letters don't match and you can make a mistake
					cerr << "	mistake found and exact false" << endl;
					count++;
					mistakeNotAllowed = true;
				}
				else {	// if the letters don't match and you cannot make a mistake
					break;
				}
			}
			cerr << "	ending length " << count << endl;
			unordered_map<string, int>::iterator itLength = lengthOfLongestInGenome.find(hits[i].genomeName);
			unordered_map<string, int>::iterator itPos = posOfLongestInGenome.find(hits[i].genomeName);
			if (itLength == lengthOfLongestInGenome.end()) {
				lengthOfLongestInGenome.insert(make_pair(hits[i].genomeName, count));
				posOfLongestInGenome.insert(make_pair(hits[i].genomeName, hits[i].position));
			}
			else if (count > itLength->second) { // if the length of this hit was the longest, update
				itLength->second = count;
				itPos->second = hits[i].position;
			}
		}
	}
	for (int i = 0; i < m_genomes.size(); i++) {
		unordered_map<string, int>::iterator itLength = lengthOfLongestInGenome.find(m_genomes[i].name());
		unordered_map<string, int>::iterator itPos = posOfLongestInGenome.find(m_genomes[i].name());
		if (itLength != lengthOfLongestInGenome.end() || itPos != posOfLongestInGenome.end()) {
			DNAMatch d;
			d.genomeName = m_genomes[i].name();
			d.position = itPos->second;
			d.length = itLength->second;
			if (itLength->second >= minimumLength) { // if the fragment found is larger than the min fragment length
				matches.push_back(d);
			}
		}
	}
	if (matches.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	if (fragmentMatchLength < m_minSearchLength) {
		return false;
	}
	unordered_map<string, int> matchesPerGenome;
	for (int i = 0; i < query.length(); i = i + fragmentMatchLength) {
		string substring;
		if (!query.extract(i, fragmentMatchLength, substring)) {
			continue;
		}
		vector<DNAMatch> matches;
		findGenomesWithThisDNA(substring, fragmentMatchLength, exactMatchOnly, matches);
		for (int j = 0; j < matches.size(); j++) {
			unordered_map<string, int>::iterator it = matchesPerGenome.find(matches[j].genomeName);
			if (it != matchesPerGenome.end()) {
				it->second++;
			}
			else {
				matchesPerGenome.insert(make_pair(matches[j].genomeName, 1));
			}
		}
	}
	int totalSequences = query.length() / fragmentMatchLength;
	unordered_map<string, int>::iterator iter = matchesPerGenome.begin();
	for (; iter != matchesPerGenome.end(); iter++) {
		int num = iter->second;
		double percent = (1.00*num / totalSequences) * 100;
		if (percent >= matchPercentThreshold) {
			GenomeMatch m;
			m.genomeName = iter->first;
			m.percentMatch = percent;
			results.push_back(m);
		}
	}
	sort(results.begin(), results.end(), greater<GenomeMatch>());
	if (results.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool operator>(const GenomeMatch &m1, const GenomeMatch &m2) {
	if (m1.percentMatch != m2.percentMatch) {
		if (m1.percentMatch > m2.percentMatch) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (m1.genomeName < m2.genomeName) {
			return true;
		}
		else {
			return false;
		}
	}
}
//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
	m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
	delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
	m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
	return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
