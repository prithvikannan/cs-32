bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	/*
	If the start location is equal to the ending location, then we've
	    solved the maze, so return true.
	Mark the start location as visted.
	For each of the four directions,
	    If the location one step in that direction (from the start
		location) is unvisited,
		    then call pathExists starting from that location (and
				ending at the same ending location as in the
				current call).
			 If that returned true,
			     then return true.
	Return false.
	*/
	if (sr == er && sc == ec) {
		return true;
	}
	maze[sr][sc] = '!';

	if (maze[sr][sc + 1] != '!' && maze[sr][sc + 1] != 'X') {		// check if there is valid and unvisited space to the right
		if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec)) return true;
	}

	if (maze[sr + 1][sc] != '!' && maze[sr + 1][sc] != 'X') {		// check if there is valid and unvisited space to the bottom
		if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec)) return true;
	}
	
	if (maze[sr][sc - 1] != '!' && maze[sr][sc - 1] != 'X') {		// check if there is valid and unvisited space to the left
		if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec)) return true;
	}
	
	if (maze[sr - 1][sc] != '!' && maze[sr - 1][sc] != 'X') {		// check if there is valid and unvisited space to the top
		if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec)) return true;
	}
	
	return false;
}