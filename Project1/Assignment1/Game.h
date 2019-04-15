#ifndef GAME_H
#define GAME_H
class Arena;
class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nZombies);
	~Game();
	int decodeDirection(char dir);
	// Mutators
	void play();

private:
	Arena* m_arena;
};

#endif
