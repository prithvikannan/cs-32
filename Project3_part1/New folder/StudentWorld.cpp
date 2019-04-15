#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h";
#include "Level.h";
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}


int StudentWorld::init()
{
	Level lev(assetPath());
	string levelFile = "level01.txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		for (int level_x = 0; level_x < 16; level_x++) {
			for (int level_y = 0; level_y < 16; level_y++) {
				Level::MazeEntry ge = lev.getContentsOf(level_x, level_y); // level_x=5, level_y=10
				switch (ge) // so x=80 and y=160
				{
				case Level::empty:
					// cout << "Location " << 16*i << ", " << 16*j << " is empty" << endl;
					break;
				case Level::smart_zombie:
					// cout << "Location " << 16 * i << ", " << 16 * j << " starts with a smart zombie" << endl;
					break;
				case Level::dumb_zombie:
					// cout << "Location " << 16 * i << ", " << 16 * j << " starts with a dumb zombie" << endl;
					break;
				case Level::player:
					// cout << "Location " << 16 * i << ", " << 16 * j << " is where Penelope starts" << endl;
					m_penelope = new Penelope(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y);
					actors.push_back(m_penelope); break;
				case Level::exit:
					// cout << "Location " << 16 * i << ", " << 16 * j << " is where an exit is" << endl;
					break;
				case Level::wall:
					// cout << "Location " << 16 * i << ", " << 16 * j << " holds a Wall" << endl;
					actors.push_back(new Wall(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::pit:
					// cout << "Location " << 16 * i << ", " << 16 * j << " has a pit in the ground" << endl;
					break;
					// etc…
				}
			}
		}
		
	}

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	for (vector<Actor *>::iterator iter = actors.begin(); iter != actors.end(); iter++)
		((*iter))->doSomething();
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (vector<Actor *>::iterator iter = actors.begin(); iter != actors.end(); )
	{
		delete *iter;
		iter = actors.erase(iter);
	}
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

bool StudentWorld::isBlocked(int x, int y, Actor * a)
{
	for (int i = 0; i < actors.size(); i++) {
		if (a != actors[i] && actors[i]->canBlock()) {
			int otherX = actors[i]->getX();
			int otherY = actors[i]->getY();
			if (x < otherX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 > otherX &&
				y < otherY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 > otherY) {
				return true;
			}
		}
	}
	/*
	for (vector<Actor *>::iterator iter = actors.begin(); iter != actors.end(); iter++) {
		if (a != (*iter) && (*iter)->canBlock()) {
			int otherX = (*iter)->getX();
			int otherY = (*iter)->getY();
			if (x < otherX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 > otherX &&
				y < otherY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 > otherY) {
				return true;
			}
		}
	}
	*/
	return false;
}
