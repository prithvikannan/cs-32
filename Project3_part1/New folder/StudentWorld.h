#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

class Actor;
class Penelope;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	~StudentWorld();
	bool isBlocked(int x, int y, Actor *a);
private:
	std::vector<Actor*> actors;
	Penelope * m_penelope;
};

#endif // STUDENTWORLD_H_
