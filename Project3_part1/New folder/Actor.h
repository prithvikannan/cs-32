#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld * world, int imageID, int x, int y, int direction, int depth);
	virtual ~Actor() {}
	int getName();
	StudentWorld * getWorld();
	int getState();
	void setState(int state);
	bool isDead();
	virtual void sufferCollision(Actor * object) = 0;
	virtual void doSomething() = 0;
	virtual bool canBlock() = 0;
private:
	StudentWorld * studentWorld;
	int m_state; // 0 = dead 1 = alive
	int name;
};

class Wall : public Actor {
public:
	Wall(StudentWorld * world, int x, int y) :
		Actor(world, IID_WALL, x, y, 0, 0) {} // TODO: MAKE THIS RIGHT
	virtual void doSomething();
	virtual void sufferCollision(Actor * object) {};
	virtual ~Wall();
	bool canBlock() {
		return true;
	}
};

class DamageableObject : public Actor {

};

class DamagingObject : public Actor {

};

class MovableObject:public DamageableObject {

};

class Goodie :public DamageableObject {

};

class Projectile : public DamagingObject {

};

class Human :public MovableObject {

};

class Zombie :public MovableObject {

};


class Penelope : public Actor
{
public:
	Penelope(StudentWorld * world, int x, int y);
	virtual void doSomething();
	virtual void sufferCollision(Actor * object);
	int getInfectionCount();
	bool getInfectionStatus();
	virtual ~Penelope();
	bool canBlock() {
		return true;
	}
private:
	void useLandmine();
	void useFlamethrower();
	void useVaccine();
	void incrementInfectionCount();
	int m_landmines;
	int m_flamethrowerCharges;
	int m_vaccines;
	int m_infectionCount;
	bool m_infectionStatus;
};


#endif // ACTOR_H_