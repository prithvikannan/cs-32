#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols) {
	m_nCols = nCols;
	m_nRows = nRows;
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			m_kills[r][c] = 0;
		}
	}
}

bool History::record(int r, int c) {
	if (r-1 > MAXROWS || r-1 < 0 || c-1 < 0 || c-1 > MAXCOLS) {
		return false;
	}
	m_kills[r-1][c-1]++;
	return true;
}

void History::display() const {
	clearScreen();
	for (int r = 0; r < m_nRows; r++) {
		for (int c = 0; c < m_nCols; c++) {
			if (m_kills[r][c] == 0) {
				cout << '.';
			}
			else if (m_kills[r][c] < 26)
			{
				char number = 'A' + m_kills[r][c] - 1;
				cout << number;
			}
			else {
				cout << 'Z';
			}
		}
		cout << endl;
	}
}


