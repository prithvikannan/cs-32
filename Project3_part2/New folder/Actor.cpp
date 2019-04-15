#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>

using namespace std;

// ----------------------------------------ACTOR -------------------------------------------------

Actor::Actor(StudentWorld * w, int imageID, double x, double y, int dir, int depth):
	GraphObject(imageID, x, y, dir, depth) 
{
		m_studentWorld = w;
		m_state = true;						// SETS TO ALIVE
		m_name = imageID;
}

bool Actor::isDead() const
{
	return !m_state;
}

void Actor::setDead()
{
	m_state = false;
}

StudentWorld * Actor::world() const
{
	return m_studentWorld;
}

void Actor::activateIfAppropriate(Actor * a)
{
}

void Actor::useExitIfAppropriate()
{
}

void Actor::dieByFallOrBurnIfAppropriate()
{
}

void Actor::beVomitedOnIfAppropriate()
{
}

void Actor::pickUpGoodieIfAppropriate(Goodie * g)
{
}

bool Actor::blocksMovement() const							// ACTORS DO NOT BLOCK MOVEMENT BY DEFAULT
{
	return false;
}

bool Actor::blocksFlame() const								// ACTORS DO NOT BLOCK FLAME BY DEFAULT
{
	return false;
}

bool Actor::triggersOnlyActiveLandmines() const				// ACTORS DO NOT TRIGGER LANDMINE BY DEFAULT
{
	return false;
}

bool Actor::triggersZombieVomit() const						// ACTORS DO NOT TRIGGER VOMIT BY DEFAULT
{
	return false;
}

bool Actor::threatensCitizens() const						// ACTORS DO NOT THREATEN CITIZENS BY DEFAULT
{
	return false;
}

bool Actor::triggersCitizens() const						// ACTORS DO NOT TRIGGER CITIZENS BY DEFAULT
{
	return false;
}

// ----------------------------------------WALL -------------------------------------------------

Wall::Wall(StudentWorld * w, double x, double y) :
	Actor(w, IID_WALL, x, y, 0, 0) 
{
}

void Wall::doSomething()
{
}

bool Wall::blocksMovement() const
{
	return true;
}

bool Wall::blocksFlame() const
{
	return true;
}

// ----------------------------------------ACTIVATINGOBJECT -------------------------------------------------

ActivatingObject::ActivatingObject(StudentWorld * w, int imageID, double x, double y, int depth, int dir):
	Actor(w, imageID, x, y, dir, depth)
{
}

// ----------------------------------------AGENT -------------------------------------------------

Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir): 
	Actor(w, imageID, x, y, dir, 0)
{
}

bool Agent::blocksMovement() const
{
	return true;
}

bool Agent::triggersOnlyActiveLandmines() const
{
	return true;
}

// ----------------------------------------HUMAN -------------------------------------------------

Human::Human(StudentWorld * w, int imageID, double x, double y):
	Agent(w, imageID, x, y, right)
{
}

void Human::beVomitedOnIfAppropriate()
{
	if (m_infected == 0 && !triggersCitizens()) {
		world()->playSound(SOUND_CITIZEN_INFECTED);
	}
	incrementInfectionDuration();
}

bool Human::triggersZombieVomit() const
{
	return true;
}

void Human::clearInfection()
{
	m_infected = 0;
	m_infected = false;
}

int Human::getInfectionDuration() const
{
	return m_infected;
}

void Human::incrementInfectionDuration() {
	m_infected++;
}

bool Human::isInfected() const {
	if (m_infected > 0) {			
		return true;
	}
	return false;
}

void Human::handleInfection()
{
	if (isInfected()) {							// IF IT IS INFECTED
		incrementInfectionDuration();
	}
	if (getInfectionDuration() >= 500) {		// IF THE HUMAN HAS REACHED THE MAX INFECTION
		if (randInt(1, 10) > 8) {				
			world()->addActor(new DumbZombie(world(), getX(), getY()));		// 70% DUMB
		}
		else {
			world()->addActor(new SmartZombie(world(), getX(), getY()));	// 30% DUMb
		}
		dieByInfection();		// RUNS EITHER CITIZEN'S OR PENELOPE'S IMPLEMENTATION
	}
}

void Human::setInfected(int n)
{
	m_infected = n;
}

// ----------------------------------------PENELOPE -------------------------------------------------

Penelope::Penelope(StudentWorld * w, double x, double y):
	Human(w,IID_PLAYER,x,y)
{
	m_vaccines = 0;
	m_flameCharges = 0; 
	m_landmines = 0;
	setInfected(0);
}

void Penelope::doSomething()
{
	if (this->isDead()) {	// IF ALREADY DEAD
		return;
	}
	world()->activateOnAppropriateActors(this);		// FIND OUT IF ANYTHING IS NEARBY
	handleInfection();	
	int key;
	if (world()->getKey(key))	// HANDLE THE KEY PRESS
	{
		int newX = getX();
		int newY = getY();
		switch (key)
		{
		case KEY_PRESS_LEFT:
			newX -= 4;
			setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			newX += 4;
			setDirection(right);
			break;
		case KEY_PRESS_UP:
			newY += 4;
			setDirection(up);
			break;
		case KEY_PRESS_DOWN:
			newY -= 4;
			setDirection(down);
			break;
		case KEY_PRESS_SPACE:		// FLAMETHROWER
			if (m_flameCharges > 0) {
				m_flameCharges--;
				world()->playSound(SOUND_PLAYER_FIRE);
				switch (getDirection()) {
				case left:
					for (int i = 1; i < 4; i++) {
						if (!world()->isFlameBlockedAt(getX() - i * SPRITE_WIDTH, getY()))
						{
							world()->addActor(new Flame(world(), getX() - i * SPRITE_WIDTH, getY(), left));
						}
						else {
							break;
						}
					}
					break;
				case right:
					for (int i = 1; i < 4; i++) {
						if (!world()->isFlameBlockedAt(getX() + i * SPRITE_WIDTH, getY()))
						{
							world()->addActor(new Flame(world(), getX() + i * SPRITE_WIDTH, getY(), right));
						}
						else {
							break;
						}

					}
					break;
				case up:
					for (int i = 1; i < 4; i++) {
						if (!world()->isFlameBlockedAt(getX(), getY() + i * SPRITE_WIDTH))
						{
							world()->addActor(new Flame(world(), getX(), getY() + i * SPRITE_WIDTH, up));
						}
						else {
							break;
						}
					}
					break;
				case down:
					for (int i = 1; i < 4; i++) {
						if (!world()->isFlameBlockedAt(getX(), getY() - i * SPRITE_WIDTH))
						{
							world()->addActor(new Flame(world(), getX(), getY() - i * SPRITE_WIDTH, down));
						}
						else {
							break;
						}
					}
					break;
				}
			}
			break;
		case KEY_PRESS_TAB:			// LANDMINE
			if (m_landmines > 0) {
				world()->addActor(new Landmine(world(), getX(), getY()));
				m_landmines--;
			}
			break;
		case KEY_PRESS_ENTER:		// VACCINE
			if (m_vaccines > 0) {
				m_vaccines--;
				clearInfection();
			}
		}
		if (!world()->isAgentMovementBlockedAt(newX, newY)) {		// IF THE NEW SPOT IS OPEN
			moveTo(newX, newY);
		}
	}
}

void Penelope::useExitIfAppropriate()
{
	if (world()->isLevelDone()) {
		world()->recordLevelFinishedIfAllCitizensGone();
	}
}

void Penelope::dieByFallOrBurnIfAppropriate()
{
	this->setDead();
	world()->playSound(SOUND_PLAYER_DIE);
}

void Penelope::pickUpGoodieIfAppropriate(Goodie * g)
{
	g->pickUp(this);
}

bool Penelope::triggersCitizens() const
{
	return true;
}

void Penelope::increaseVaccines()
{
	m_vaccines++;
	world()->increaseScore(50);
	world()->playSound(SOUND_GOT_GOODIE);
}

void Penelope::increaseFlameCharges()
{
	m_flameCharges += 5;
	world()->increaseScore(50);
	world()->playSound(SOUND_GOT_GOODIE);

}

void Penelope::increaseLandmines()
{
	m_landmines += 2;
	world()->increaseScore(50);
	world()->playSound(SOUND_GOT_GOODIE);

}

int Penelope::getNumVaccines() const
{
	return m_vaccines;
}

int Penelope::getNumFlameCharges() const
{
	return m_flameCharges;
}

int Penelope::getNumLandmines() const
{
	return m_landmines;
}

void Penelope::dieByInfection()
{
	setDead();
	world()->playSound(SOUND_PLAYER_DIE);
}

// ----------------------------------------EXIT -------------------------------------------------

Exit::Exit(StudentWorld * w, double x, double y):
	ActivatingObject(w,IID_EXIT,x,y,1,right)
{
}

void Exit::doSomething()
{
	world()->activateOnAppropriateActors(this);
}

void Exit::activateIfAppropriate(Actor * a)
{
	a->useExitIfAppropriate();
}

bool Exit::blocksFlame() const
{
	return true;
}

// ----------------------------------------GOODIE -------------------------------------------------

Goodie::Goodie(StudentWorld * w, int imageID, double x, double y):
	ActivatingObject(w,imageID,x,y,1,right)
{
}

void Goodie::activateIfAppropriate(Actor * a)
{
	a->pickUpGoodieIfAppropriate(this);
}

void Goodie::dieByFallOrBurnIfAppropriate()
{
	setDead();
}

// ----------------------------------------VACCINEGOODIE -------------------------------------------------

VaccineGoodie::VaccineGoodie(StudentWorld * w, double x, double y):
	Goodie(w,IID_VACCINE_GOODIE,x,y)
{
}

void VaccineGoodie::doSomething()
{
	world()->activateOnAppropriateActors(this);
}

void VaccineGoodie::pickUp(Penelope * p)
{
	p->increaseVaccines();
	setDead();
}

// ----------------------------------------GASCANGOODIE -------------------------------------------------

GasCanGoodie::GasCanGoodie(StudentWorld * w, double x, double y):
	Goodie(w, IID_GAS_CAN_GOODIE, x, y)
{
}

void GasCanGoodie::doSomething()
{
	world()->activateOnAppropriateActors(this);

}

void GasCanGoodie::pickUp(Penelope * p)
{
	p->increaseFlameCharges();
	setDead();
}

// ----------------------------------------LANDMINEGOODIE -------------------------------------------------

LandmineGoodie::LandmineGoodie(StudentWorld * w, double x, double y):
	Goodie(w,IID_LANDMINE_GOODIE,x,y)
{
}

void LandmineGoodie::doSomething()
{
	world()->activateOnAppropriateActors(this);

}

void LandmineGoodie::pickUp(Penelope * p)
{
	p->increaseLandmines();
	setDead();
}

// ----------------------------------------ZOMBIE -------------------------------------------------

Zombie::Zombie(StudentWorld * w, double x, double y) :
	Agent(w, IID_ZOMBIE, x, y, right)
{
	m_movementPlanDist = 0;
	m_paralyzed = false;
}

void Zombie::vomitIfAppropriate() {
	double vomitX = getX();
	double vomitY = getY();
	switch (getDirection()) {									// TRIES TO VOMIT IN THE DIRECTION ITS FACING
	case up:
		vomitY += SPRITE_HEIGHT;
		break;
	case down:
		vomitY += SPRITE_HEIGHT;
		break;
	case left:
		vomitX -= SPRITE_HEIGHT;
		break;
	case right:
		vomitX += SPRITE_HEIGHT;
		break;
	}
	if (world()->isZombieVomitTriggerAt(vomitX, vomitY)) {		// IF AN OBJECT WOULD TRIGGER VOMIT AT THE NEW COORD
		int rand = randInt(0, 2);								// VOMITS 1/3 OF THE TIME
		if (rand == 0) {
			world()->playSound(SOUND_ZOMBIE_VOMIT);
			world()->addActor(new Vomit(world(), vomitX, vomitY));
		}
	}
}

bool Zombie::threatensCitizens() const
{
	return true;
}

void Zombie::doSomething()
{
	if (!isDead()) {
		if (isParalyzed()) {
			flipParalysis();
			return;
		}
		vomitIfAppropriate();
		world()->activateOnAppropriateActors(this);			// ASK IF WORLD IS INTERACTING WITH THE ZOMBIE

		// MOVEMENT PLAN
		if (getMovementPlanDist() == 0) {
			reCalculateMovementPath();
		}
		
		// MOVE ZOMBIE
		move();
	}
	flipParalysis();
}

void Zombie::flipParalysis()
{
	if (m_paralyzed == true) {
		m_paralyzed = false;
	}
	else {
		m_paralyzed = true;
	}
}

bool Zombie::triggersCitizens() const
{
	return true;
}

bool Zombie::isParalyzed() const
{
	return m_paralyzed;
}

void Zombie::updateMovementPlanDist(int dist)
{
	m_movementPlanDist = dist;
}

int Zombie::getMovementPlanDist() const
{
	return m_movementPlanDist;
}

void Zombie::move()
{
	// CALCULATE NEW COORDINATES
	int newX = getX();
	int newY = getY();
	int dir = getDirection();
	switch (dir) {
	case up:
		newY += 1;
		break;
	case down:
		newY -= 1;
		break;
	case right:
		newX += 1;
		break;
	case left:
		newX -= 1;
		break;
	}
	
	if (!world()->isAgentMovementBlockedAt(newX, newY, this)) {		
		// CHECK IF WE CAN MOVE TO NEW COORDINATES
		moveTo(newX, newY);
		updateMovementPlanDist(getMovementPlanDist() - 1);
	}
	else {															
		// BLOCKED FROM MOVING TO NEW COORDINATES
		updateMovementPlanDist(0);
	}
}

// ----------------------------------------DUMBZOMBIE -------------------------------------------------

DumbZombie::DumbZombie(StudentWorld * w, double x, double y) :
	Zombie(w, x, y)
{
}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{
	this->setDead();
	world()->increaseScore(1000);
	world()->playSound(SOUND_ZOMBIE_DIE);
	int rand = randInt(1, 10);
	if (rand == 1) {	// 1/10 OF THE TIME THE DUMB ZOMBIE DROPS A VACCINEE GOODIE
		int x = getX();
		int y = getY();
		int randDir = randInt(0, 3) * 90;
		switch (randDir) {
		case up:
			y += SPRITE_HEIGHT;
		case down:
			y -= SPRITE_HEIGHT;
		case left:
			x -= SPRITE_WIDTH;
		case right:
			x += SPRITE_WIDTH;
		}
		if (!world()->isOverlap(x, y)) {	// CHECKS IF THE DIRECTION THE ZOMBIE TRIES TO THROW TO IS OPEN
			Actor* f = new VaccineGoodie(world(), x, y);
			world()->addActor(f);
		}

	}
}

void DumbZombie::reCalculateMovementPath()
{
	updateMovementPlanDist(randInt(3, 10));		// RANDOM DISTANCE
	setDirection(randInt(0, 3) * 90);			// RANDOM DIRECTION
}

// ----------------------------------------SMARTZOMBIE-------------------------------------------------

SmartZombie::SmartZombie(StudentWorld * w, double x, double y):
	Zombie(w,x,y)
{
}

int SmartZombie::reCalculateDirection(int x, int y, int otherX, int otherY)
{
	if (x == otherX) // SAME X COORDS
	{
		if (otherY > y)
		{
			return up;
		}
		else
		{
			return down;
		}
	}
	if (y == otherY) // SAME Y COORDS
	{
		if (otherX > x)
		{
			return right;
		}
		else
		{
			return left;
		}
	}

	int random = randInt(1, 2);
	if (otherX > x && otherY > y) // TOP RIGHT
	{
		if (random == 1)
		{
			return up;
		}
		else
		{
			return right;
		}
	}
	else if (otherX < x && otherY > y) // TOP LEFT
	{
		if (random == 1)
		{
			return up;
		}
		else
		{
			return left;
		}
	}
	else if (otherY < y && otherX < x) // BOTTOM LEFT
	{
		if (random == 1)
		{
			return down;
		}
		else
		{
			return left;
		}
	}
	else								// BOTTOM RIGHT
	{
		if (random == 1)
		{
			return right;
		}
		else
		{
			return down;
		}
	}
}

void SmartZombie::dieByFallOrBurnIfAppropriate()
{
	this->setDead();
	world()->increaseScore(2000);
	world()->playSound(SOUND_ZOMBIE_DIE);
}

void SmartZombie::reCalculateMovementPath()
{
	updateMovementPlanDist(randInt(3, 10));		// RANDOM DISTANCE
	double x;
	double y;
	double dist;
	world()->locateNearestVomitTrigger(getX(), getY(), x, y, dist);
	if (dist <= 6400) {												// IF IN RANGE
		setDirection(reCalculateDirection(getX(), getY(), x, y));	// PICKS THE DIRECTION TO GET CLOSER TO HUMAN 
	}
	else {
		setDirection(randInt(0, 3) * 90);							// RANDOM DIRECTION
	}	
}

// ----------------------------------------PIT-------------------------------------------------

Pit::Pit(StudentWorld * w, double x, double y) :
	ActivatingObject(w, IID_PIT, x, y, 0, right)
{
}

void Pit::doSomething()
{
	world()->activateOnAppropriateActors(this);		// ASKS WORLD IF ANYTHING INTERACTS WITH PIT
}

void Pit::activateIfAppropriate(Actor * a)
{
	a->dieByFallOrBurnIfAppropriate();
}

// ----------------------------------------FLAME-------------------------------------------------

Flame::Flame(StudentWorld * w, double x, double y, int dir) :
	ActivatingObject(w, IID_FLAME, x, y, right, 0)
{
	m_ticks = 0;
}

void Flame::doSomething()
{
	if (!isDead()) {
		m_ticks++;
		if (m_ticks == 3) {
			this->setDead();
			return;
		}
		world()->activateOnAppropriateActors(this);		// ASKS WORLD IF ANYTHING INTERACTS WITH FLAME
	}
}

void Flame::activateIfAppropriate(Actor * a)
{
	a->dieByFallOrBurnIfAppropriate();
}

// ----------------------------------------LANDMINE-------------------------------------------------

Landmine::Landmine(StudentWorld * w, double x, double y):
	ActivatingObject(w, IID_LANDMINE, x, y, 1, right)
{
	m_safetyTicks = 30;
	m_active = false;
}

void Landmine::doSomething()
{
	if (isDead()) {
		return;
	}
	if (m_safetyTicks == 0) {
		m_active = true;
	}
	if (m_active) {
		world()->activateOnAppropriateActors(this); // ASKS WORLD IF ANYTHING INTERACTS WITH LANDMINE
	}
	m_safetyTicks--;
}


void Landmine::activateIfAppropriate(Actor * a)
{
	if (!this->isDead()) {
		if (a->triggersOnlyActiveLandmines()) {
			dieByFallOrBurnIfAppropriate();
		}
	}
}


void Landmine::dieByFallOrBurnIfAppropriate()
{
	this->setDead();		// KILL THE LANDMINE
	world()->playSound(SOUND_LANDMINE_EXPLODE);

	// MAKE RADIAL FLAMES
	if (!world()->isFlameBlockedAt(getX() + SPRITE_WIDTH, getY())) {
		Actor* f = new Flame(world(), getX() + SPRITE_WIDTH, getY(), up);
		world()->addActor(f);
	}
	if (!world()->isFlameBlockedAt(getX() + SPRITE_WIDTH, getY()) - SPRITE_HEIGHT) {
		Actor* f = new Flame(world(), getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
		world()->addActor(f);
	}
	if (!world()->isFlameBlockedAt(getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT)) {
		Actor* f = new Flame(world(), getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
		world()->addActor(f);
	}
	if (!world()->isFlameBlockedAt(getX() - SPRITE_WIDTH, getY())) {
		Actor* f = new Flame(world(), getX() - SPRITE_WIDTH, getY(), up);
		world()->addActor(f);
	}
	if (!world()->isFlameBlockedAt(getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT)) {
		Actor* f = new Flame(world(), getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
		world()->addActor(f);
	}
	if (!world()->isFlameBlockedAt(getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT)) {
		Actor* f = new Flame(world(), getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
		world()->addActor(f);
	}
	if (!world()->isFlameBlockedAt(getX(), getY() + SPRITE_HEIGHT)) {
		Actor* f = new Flame(world(), getX(), getY() + SPRITE_HEIGHT, up);
		world()->addActor(f);
	}
	if (!world()->isFlameBlockedAt(getX(), getY() - SPRITE_HEIGHT)) {
		Actor* f = new Flame(world(), getX(), getY() - SPRITE_WIDTH, up);
		world()->addActor(f);
	}

	// MAKE PIT WHERE LANDMINE WAS
	Actor* p = new Pit(world(), getX(), getY());
	world()->addActor(p);
}

// ----------------------------------------VOMIT-------------------------------------------------

Vomit::Vomit(StudentWorld * w, double x, double y) :
	ActivatingObject(w, IID_VOMIT, x, y, right, 0)
{
	m_ticks = 0;
}

void Vomit::doSomething()
{
	if (!isDead()) {
		world()->activateOnAppropriateActors(this);		// ASKS WORLD IF ANYTHING INTERACTS WITH VOMIT
		if (m_ticks == 2) {
			setDead();
			return;
		}
		m_ticks++;
	}

}

void Vomit::activateIfAppropriate(Actor * a)
{
	a->beVomitedOnIfAppropriate();
}

// ----------------------------------------CITIZEN-------------------------------------------------

Citizen::Citizen(StudentWorld * w, double x, double y):
	Human(w,IID_CITIZEN,x,y)
{
	setInfected(0);
}

void Citizen::doSomething()
{
	if (isDead()) {
		return;
	}
	if (isParalyzed())
	{
		flipParalysis();
		return;
	}
	if (getInfectionDuration() > 0)
	{
		incrementInfectionDuration();
		if (getInfectionDuration() == 500) // PENELOPE BECOMES A ZOMBIE
		{
			setDead();
			world()->playSound(SOUND_ZOMBIE_BORN);
			world()->increaseScore(-1000);

			Actor *temp;
			if (randInt(1, 10) <= 7)
			{
				temp = new DumbZombie(world(), getX(), getY());
			}
			else
			{
				temp = new SmartZombie(world(), getX(), getY());
			}
			world()->addActor(temp);
			return;
		}
	}
	double x = 0, y = 0, distance = 0.0;
	bool isThreat = false;
	if (!world()->locateNearestCitizenTrigger(getX(), getY(), x, y, distance, isThreat))	// TRIES TO FIND PENELOPE TO FOLLOW
	{
		return;
	}
	moveAppropriately(x, y, distance, isThreat);	// TRY TO GO TO NEW COORDINATES IF APPLICABLE
	flipParalysis();
}



void Citizen::moveAppropriately(double targetX, double targetY, double distance, double isThreat)
{

	// IF FOLLOWING PENELOPE
	double destX = getX();
	double destY = getY();
	if (!isThreat) // IF IS NOT A THREAT
	{
		if (getX() == targetX)			// IN A LINE IN X
		{
			attemptToMoveY(targetY, destY);
		}
		else if (getY() == targetY)		// IN A LINE IN Y
		{
			attemptToMoveX(targetX, destX);
		}
		else
		{
			int rand = randInt(0, 1);	// CHOOSE RANDOM DIRECTION BETWEEN TWO THAT MAKE IT CLOSER
			if (rand == 0)
			{
				if (!attemptToMoveX(targetX, destX))
				{
					if (!attemptToMoveY(targetY, destY))
					{
						flee(distance, destX, destY);		// CAN'T MOVE IN X OR Y SO FLEE
					}
				}
			}
			else
			{
				if (!attemptToMoveY(targetY, destY))
				{
					if (!attemptToMoveX(targetX, destX))
					{
						flee(distance, destX, destY);	// CAN'T MOVE IN X OR Y SO FLEE
					}
				}
			}
		}
	}
	else // IF IS A THREAT
	{
		flee(distance, destX, destY);
	}

	if (!world()->isAgentMovementBlockedAt(destX, destY, this)) // IF WE CAN MOVE TO THE POSITION
	{
		moveTo(destX, destY);
	}
}


void Citizen::flee(double distance, double &destX, double &destY)
{
	double maximum = distance;
	int maxDir = getDirection();
	double newDist = distance;
	double newX = 0;
	double newY = 0;
	for (int i = 0; i <= 3; i ++) {		// GO THROUGH DIRECTIONS TO FIND WHICH TAKES YOU FURTHEST AWAY
		int angle = 90*i;
		switch (angle) {
		case up:
			if (!world()->isAgentMovementBlockedAt(getX(), getY() + 1, this) && !world()->isAgentMovementBlockedAt(getX(), getY() + 2, this))
			{
				world()->locateNearestCitizenThreat(getX(), getY() + 2, newX, newY, newDist);
				if (newDist > maximum) {
					maximum = newDist;
					maxDir = angle;
				}
			}
			break;
		case down:
			if (!world()->isAgentMovementBlockedAt(getX(), getY() - 1, this) && !world()->isAgentMovementBlockedAt(getX(), getY() - 2, this)) {
				world()->locateNearestCitizenThreat(getX(), getY() - 2, newX, newY, newDist);
				if (newDist > maximum) {
					maximum = newDist;
					maxDir = angle;
				}
			}
			break;
		case right:
			if (!world()->isAgentMovementBlockedAt(getX() + 1, getY(), this) && !world()->isAgentMovementBlockedAt(getX() + 2, getY(), this)) {
				world()->locateNearestCitizenThreat(getX() + 2, getY(), newX, newY, newDist);
				if (newDist > maximum) {
					maximum = newDist;
					maxDir = angle;
				}
			}
			break;
		case left:
			if (!world()->isAgentMovementBlockedAt(getX() - 1, getY(), this) && !world()->isAgentMovementBlockedAt(getX() - 2, getY(), this)) {
				world()->locateNearestCitizenThreat(getX() - 2, getY(), newX, newY, newDist);
				if (newDist > maximum) {
					maximum = newDist;
					maxDir = angle;
				}
			}
			break;
		}
	}
	setDirection(maxDir);
	switch (maxDir) {
	case up:
		destY += 2;
		break;
	case down:
		destY -= 2;
		break;
	case right:
		destX += 2;
		break;
	case left:
		destX -= 2;
		break;
	}
}



bool Citizen::attemptToMoveX(double x, double& destX) {
	if (x - getX() > 0) {
		if ((!world()->isAgentMovementBlockedAt(getX() + 1, getY(), this) && (!world()->isAgentMovementBlockedAt(getX() + 2, getY(), this)))) {
			setDirection(right);
			destX += 2;
			return true;
		}
	}
	else {
		if ((!world()->isAgentMovementBlockedAt(getX() - 1, getY(), this) && (!world()->isAgentMovementBlockedAt(getX() - 2, getY(), this)))) {
			setDirection(left);
			destX -= 2;
			return true;
		}
	}
	return false;
}


bool Citizen::attemptToMoveY(double y, double& destY) {
	if (y - getY() > 0) {
		if ((!world()->isAgentMovementBlockedAt(getX(), getY() + 1, this) && (!world()->isAgentMovementBlockedAt(getX(), getY() + 2, this)))) {
			setDirection(up);
			destY += 2;
			return true;
		}
	}
	else {
		if ((!world()->isAgentMovementBlockedAt(getX(), getY() - 1, this) && (!world()->isAgentMovementBlockedAt(getX(), getY() - 2, this)))) {
			setDirection(down);
			destY -= 2;
			return true;
		}
	}
	return false;
}



void Citizen::useExitIfAppropriate()
{
	setDead();
	world()->playSound(SOUND_CITIZEN_SAVED);
	world()->recordCitizenGone();
	world()->increaseScore(500);
}

void Citizen::dieByFallOrBurnIfAppropriate()
{
	setDead();
	world()->recordCitizenGone();
	world()->playSound(SOUND_CITIZEN_DIE);
	world()->increaseScore(-1000);
}

void Citizen::flipParalysis()
{
	if (m_paralyzed) {
		m_paralyzed = false;
	}
	else {
		m_paralyzed = true;
	}
}

bool Citizen::isParalyzed() const
{
	return m_paralyzed;
}

void Citizen::dieByInfection()
{
	setDead();
	world()->recordCitizenGone();
	world()->playSound(SOUND_ZOMBIE_BORN);
	world()->increaseScore(-1000);

	Actor *temp;
	if (randInt(1, 10) <= 7)
	{
		temp = new DumbZombie(world(), getX(), getY());		// 70% DUMB ZOMBIE CREATED
	}
	else
	{
		temp = new SmartZombie(world(), getX(), getY());	// 30% SMART ZOMBIE CREATED
	}
	world()->addActor(temp);
	return;
}

