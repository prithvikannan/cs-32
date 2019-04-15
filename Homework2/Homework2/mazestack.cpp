/*
#include <iostream>
#include <string>
#include <stack>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};
int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X....X...X",
		"X.XX.XX..X",
		"XXX....X.X",
		"X.XXX.XXXX",
		"X.X...X..X",
		"X...X.X..X",
		"XXXXX.X.XX",
		"X........X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 3, 5, 8, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	stack<Coord> coordStack;
	coordStack.push(Coord(sr,sc));						// pushes the starting coord onto the stack
	maze[sr][sc] = '!';									// marks that coord as already visited
	while (!coordStack.empty()) {
		Coord top = coordStack.top();					// gets coord at top of stack
		coordStack.pop();
		// cout << "(" + to_string(top.r()) + "," + to_string(top.c()) + ")" << endl;
		int r = top.r();
		int c = top.c();
		if (c == er && r == ec) {						// if we are at the end location, return true
			return true;
		}
		if (r + 1 < nRows && maze[r + 1][c] == '.') {	//  SOUTH is valid
			maze[r+1][c] = '!';							// marks that coord as already visited
			coordStack.push(Coord(r + 1, c));
		}
		if (c -1 >= 0 && maze[r][c-1] == '.') {			// WEST is valid
			maze[r][c-1] = '!';							// marks that coord as already visited
			coordStack.push(Coord(r, c-1));
		}
		if (r - 1 >= 0 && maze[r - 1][c] == '.') {		// NORTH is valid
			maze[r - 1][c] = '!';						// marks that coord as already visited
			coordStack.push(Coord(r - 1, c));
		}
		if (c + 1 < nCols && maze[r][c+1] == '.') {		// EAST is valid
			maze[r][c + 1] = '!';						// marks that coord as already visited
			coordStack.push(Coord(r, c+1));
		}
	}
	return false;

}
*/