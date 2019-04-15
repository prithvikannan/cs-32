// Possible interfaces for actors.  You may use all, some, or none
// of this, as you wish.

// One of the design goals is to reduce the coupling between StudentWorld.h
// and Actor.h as much as possible.  Notice that in fact, Actor.h
// does not need to include Student.h at all.

#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"

class StudentWorld;
class Goodie;
class Penelope;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth);

	// Action to perform for each tick.
	virtual void doSomething() = 0;

	// Is this actor dead?
	bool isDead() const;

	// Mark this actor as dead.
	void setDead();

	// Get this actor's world
	StudentWorld* world() const;

	// If this is an activated object, perform its effect on a (e.g., for an
	// Exit have a use the exit).
	virtual void activateIfAppropriate(Actor* a);

	// If this object uses exits, use the exit.
	virtual void useExitIfAppropriate();

	// If this object can die by falling into a pit or burning, die.
	virtual void dieByFallOrBurnIfAppropriate();

	// If this object can be infected by vomit, get infected.
	virtual void beVomitedOnIfAppropriate();

	// If this object can pick up goodies, pick up g
	virtual void pickUpGoodieIfAppropriate(Goodie* g);

	// Does this object block agent movement?
	virtual bool blocksMovement() const;

	// Does this object block flames?
	virtual bool blocksFlame() const;

	// Does this object trigger landmines only when they're active?
	virtual bool triggersOnlyActiveLandmines() const;

	// Can this object cause a zombie to vomit?
	virtual bool triggersZombieVomit() const;

	// Is this object a threat to citizens?
	virtual bool threatensCitizens() const;

	// Does this object trigger citizens to follow it or flee it?
	virtual bool triggersCitizens() const;
private:
	StudentWorld * m_studentWorld;
	bool m_state; 
	int m_name;
};

class Wall : public Actor
{
public:
	Wall(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement() const;
	virtual bool blocksFlame() const;
};

class ActivatingObject : public Actor
{
public:
	ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir);
};

class Exit : public ActivatingObject
{
public:
	Exit(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual bool blocksFlame() const;
};

class Pit : public ActivatingObject
{
public:
	Pit(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
};

class Flame : public ActivatingObject
{
public:
	Flame(StudentWorld* w, double x, double y, int dir);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
private:
	int m_ticks;
};

class Vomit : public ActivatingObject
{
public:
	Vomit(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
private:
	int m_ticks;
};

class Landmine : public ActivatingObject
{
public:
	Landmine(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();
private:
	int m_safetyTicks;
	bool m_active;
};

class Goodie : public ActivatingObject
{
public:
	Goodie(StudentWorld* w, int imageID, double x, double y);
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUp(Penelope* p) = 0;
};

class VaccineGoodie : public Goodie
{
public:
	VaccineGoodie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class GasCanGoodie : public Goodie
{
public:
	GasCanGoodie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class LandmineGoodie : public Goodie
{
public:
	LandmineGoodie(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class Agent : public Actor
{
public:
	Agent(StudentWorld* w, int imageID, double x, double y, int dir);
	virtual bool blocksMovement() const;
	virtual bool triggersOnlyActiveLandmines() const;
};

class Human : public Agent
{
public:
	Human(StudentWorld* w, int imageID, double x, double y);
	virtual void beVomitedOnIfAppropriate();
	virtual bool triggersZombieVomit() const;

	// Make this human uninfected by vomit.
	int getInfectionDuration() const;

protected:
	void clearInfection();
	bool isInfected() const;
	void incrementInfectionDuration();
	void handleInfection();
	virtual void dieByInfection() = 0;
	void setInfected(int n);
private:
	int m_infected;
};

class Penelope : public Human
{
public:
	Penelope(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUpGoodieIfAppropriate(Goodie* g);

	bool triggersCitizens() const;

	// Increase the number of vaccines the object has.
	void increaseVaccines();
	
	// Increase the number of flame charges the object has.
	void increaseFlameCharges();

	// Increase the number of landmines the object has.
	void increaseLandmines();

	// How many vaccines does the object have?
	int getNumVaccines() const;

	// How many flame charges does the object have?
	int getNumFlameCharges() const;

	// How many landmines does the object have?
	int getNumLandmines() const;

private:
	void dieByInfection();
	int m_vaccines;
	int m_flameCharges;
	int m_landmines;

};

class Citizen : public Human
{
public:
	Citizen(StudentWorld* w, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
private:
	void dieByInfection();
	bool isParalyzed() const;
	void flipParalysis();
	void moveAppropriately(double targetX, double targetY, double distance, double isThreat);
	void flee(double distance, double & destX, double & destY);
	bool attemptToMoveX(double x, double & destX);
	bool attemptToMoveY(double y, double & destY);
	bool m_paralyzed;
};

class Zombie : public Agent
{
public:
	Zombie(StudentWorld* w, double x, double y);
	void vomitIfAppropriate();
	bool threatensCitizens() const;
	virtual void doSomething();
protected:
	void flipParalysis();
	bool triggersCitizens() const;
	bool isParalyzed() const;
	void updateMovementPlanDist(int dist);
	int getMovementPlanDist() const;
	void move();
	virtual void reCalculateMovementPath() = 0;
private:
	int m_movementPlanDist;
	bool m_paralyzed;
};

class DumbZombie : public Zombie
{
public:
	DumbZombie(StudentWorld* w, double x, double y);
	virtual void dieByFallOrBurnIfAppropriate();
private:
	virtual void reCalculateMovementPath();
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(StudentWorld* w, double x, double y);
	virtual void dieByFallOrBurnIfAppropriate();
private:
	virtual void reCalculateMovementPath();
	int reCalculateDirection(int thisX, int thisY, int otherX, int otherY);
};

#endif // ACTOR_INCLUDED