#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
using namespace std;

//---Actor Functions---

Actor::Actor(StudentWorld * world, int imageID, int x, int y, int direction, int depth) :
	GraphObject(imageID, x, y, direction, depth) {	//initialize the graphObject, and all the corresponding private members including pointer to student world
	studentWorld = world;
	m_state = 1;						// sets to "alive"
	name = imageID;
}

int Actor::getName() {					// return the image ID or name of the actor
	return name;
}

StudentWorld *Actor::getWorld() {		// returns the world
	return studentWorld; 
}

int Actor::getState() {					// get the actor's state
	return m_state;
}

void Actor::setState(int state) {		// sets the actor's state
	m_state = state;
}

bool Actor::isDead() {					// returns true if the actor is dead, false otherwise
	return (m_state == 0);
}

//---Wall Functions---

void Wall::doSomething() {				// do nothing

}

Wall::~Wall()
{
}

//---Penelope Functions---

Penelope::Penelope(StudentWorld * world, int x, int y) :
	Actor(world, IID_PLAYER, x, y, right, 0) { 
	//initializes private members of Penelope and calls the constructor on actor
	int m_landmines = 0;
	int m_flamethrowerCharges = 0;
	int m_vaccines = 0;
	int m_infectionCount = 0;
	bool m_infectionStatus = false;
}

void Penelope::doSomething() {
	if (this->getState() == 0) {
		return;
	}
	if (this->m_infectionStatus) {
		this->incrementInfectionCount();
		if (this->getInfectionCount() >= 500) {		// PENELOPE BECOMES A ZOMBIE
			this->setState(0);							// set dead
			getWorld()->playSound(SOUND_PLAYER_DIE);
			return;
		}
	}
	// TOOD: CHANGE THIS TO CHECK BEFORE MOVE
	int key;
	if (getWorld()->getKey(key))
	{
		int destX = getX();
		int destY = getY();
		switch (key)
		{
		case KEY_PRESS_LEFT:
			destX -= 4;
			setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			destX += 4;
			setDirection(right);
			break;
		case KEY_PRESS_UP:
			destY += 4;
			setDirection(up);
			break;
		case KEY_PRESS_DOWN:
			destY -= 4;
			setDirection(down);
			break;
		}
		if (!getWorld()->isBlocked(destX, destY, this)) {
			moveTo(destX, destY);
		}
	}
	// check for key press
}

void Penelope::sufferCollision(Actor * object)
{
}

int Penelope::getInfectionCount()
{
	return 0;
}

bool Penelope::getInfectionStatus()
{
	return false;
}

Penelope::~Penelope()
{
}

void Penelope::useLandmine()
{
}

void Penelope::useFlamethrower()
{
}

void Penelope::useVaccine()
{
}

void Penelope::incrementInfectionCount()
{
}

