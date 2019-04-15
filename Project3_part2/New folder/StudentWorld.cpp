#include "StudentWorld.h"
#include "GameConstants.h"
#include <sstream>
#include <iomanip> 
#include <string>
#include "Actor.h";
#include "Level.h";
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(std::string assetDir): 
	GameWorld(assetDir)
{
	m_penelope = nullptr;
	m_isFinished = false;
	m_numCitizensRemaining = 0;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::init()
{
	m_numCitizensRemaining = 0;
	m_isFinished = false;
	Level lev(assetPath());
	updateLevelString();
	string levelFile = m_levelString;
	Level::LoadResult result = lev.loadLevel(levelFile);					// LOAD LEVEL FILE
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		for (int level_x = 0; level_x < 16; level_x++) {
			for (int level_y = 0; level_y < 16; level_y++) {
				Level::MazeEntry ge = lev.getContentsOf(level_x, level_y);	// PARSE LEVEL FILE
				switch (ge) 
				{
				case Level::empty:
					break;
				case Level::smart_zombie:
					m_actors.push_back(new SmartZombie(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::dumb_zombie:
					m_actors.push_back(new DumbZombie(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::player:
					m_penelope = new Penelope(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y);
					break;
				case Level::exit:
					m_actors.push_back(new Exit(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::wall:
					m_actors.push_back(new Wall(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::pit:
					m_actors.push_back(new Pit(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::vaccine_goodie:
					m_actors.push_back(new VaccineGoodie(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::gas_can_goodie:
					m_actors.push_back(new GasCanGoodie(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::landmine_goodie:
					m_actors.push_back(new LandmineGoodie(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					break;
				case Level::citizen:
					m_actors.push_back(new Citizen(this, SPRITE_WIDTH * level_x, SPRITE_HEIGHT * level_y));
					m_numCitizensRemaining++;
					break;
				}
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_penelope->doSomething();
	setGameStatText(createHeaderString()); // DISPLAY SCORE LINE

	for (vector<Actor *>::iterator iter = m_actors.begin(); iter != m_actors.end(); iter++) { // LOOP OVER ALL ACTORS
		if ((*iter)->isDead()) {						// IF ACTOR IS DEAD, DELETE
			vector<Actor*> ::iterator temp = iter;
			iter--;
			delete *temp;
			m_actors.erase(temp);
		}
		else {
			((*iter))->doSomething();					// IF NOT DEAD, DO SOMETHING
		}
	}
	if (m_penelope->isDead()) {							// IF PENELOPE IS DEAD
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	if (m_isFinished) {									// IF THE LEVEL IS DONE
		playSound(SOUND_LEVEL_FINISHED);
		if (getLevel() == 6) {
			return GWSTATUS_PLAYER_WON;
		}
		else {
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (m_penelope != nullptr) {
		for (vector<Actor *>::iterator iter = m_actors.begin(); iter != m_actors.end();) // LOOP OVER ACTORS
		{
			// DELETE THE POINTER AND REMOVE FROM CONTAINER
			delete *iter;
			iter = m_actors.erase(iter);
		}
		delete m_penelope;
	}
}

void StudentWorld::addActor(Actor * a)
{
	m_actors.push_back(a);
}

void StudentWorld::recordCitizenGone()
{
	m_numCitizensRemaining--;
}

void StudentWorld::recordLevelFinishedIfAllCitizensGone()
{
	m_isFinished = true;
}

void StudentWorld::activateOnAppropriateActors(Actor * a)
{
	// CHECK IF DEAD
	if (!m_penelope->isDead() && a != m_penelope && overlap(a, m_penelope)) {
		a->activateIfAppropriate(m_penelope);
	}
	for (vector<Actor *>::iterator iter = m_actors.begin(); iter != m_actors.end(); iter++) {	// LOOP OVER ALL ACTORS
		if (!(*iter)->isDead() && a != *iter && overlap(a, *iter)) {
			a->activateIfAppropriate(*iter);
		}
	}
}

bool StudentWorld::overlap(Actor * a, Actor * b) {
	// BOUNDING BOX CHECK
	double dx = ((a->getX() + SPRITE_WIDTH / 2) - (b->getX() + SPRITE_WIDTH / 2));
	double dy = ((a->getY() + SPRITE_HEIGHT / 2) - (b->getY() + SPRITE_HEIGHT / 2)); 
	dx = dx * dx;
	dy = dy * dy;
	return (dx + dy <= 100);
}
bool StudentWorld::isAgentMovementBlockedAt(double x, double y) const
{
	for (int i = 0; i < m_actors.size(); i++) {
		if (m_actors[i]->blocksMovement()) {
			int otherX = m_actors[i]->getX();
			int otherY = m_actors[i]->getY();
			if (x < otherX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 > otherX &&
				y < otherY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 > otherY) { // CHECK BOUNDING BOXES
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isAgentMovementBlockedAt(double x, double y, Actor * a) const
{
	int otherX = m_penelope->getX();
	int otherY = m_penelope->getY();
	if (x < otherX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 > otherX &&
		y < otherY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 > otherY) { // CHECK BOUNDING BOXES
		return true;
	}
	for (int i = 0; i < m_actors.size(); i++) {
		if (a != m_actors[i] && m_actors[i]->blocksMovement()) {
			otherX = m_actors[i]->getX();
			otherY = m_actors[i]->getY();
			if (x < otherX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 > otherX &&
				y < otherY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 > otherY) {
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isOverlap(double x, double y) const
{
	int otherX = m_penelope->getX();
	int otherY = m_penelope->getY();
	if (x < otherX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 > otherX &&
		y < otherY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 > otherY) { // CHECK BOUNDING BOXES
		return true;
	}
	for (int i = 0; i < m_actors.size(); i++) { // LOOP OVER ALL ACTORS
		otherX = m_actors[i]->getX();
		otherY = m_actors[i]->getY();
		if (x < otherX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 > otherX &&
			y < otherY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 > otherY) {
			return true;
		}
	}
	return false;
}

double StudentWorld::euclidianDist(double x1, double y1, double x2, double y2) const
{
	// PYTHAGOREAN THEOREM
	double dx = ((x1 + SPRITE_WIDTH / 2) - (x2 + SPRITE_WIDTH / 2));
	double dy = ((y1 + SPRITE_HEIGHT / 2) - (y2 + SPRITE_HEIGHT / 2));
	dx = dx * dx;
	dy = dy * dy;

	return (dx + dy);
}



bool StudentWorld::isFlameBlockedAt(double x, double y) const
{
	for (int i = 0; i < m_actors.size(); i++) { // LOOPS OVER ALL ACTORS
		if (m_actors[i]->blocksFlame()) {		// THIS ACTOR BLOCKS FLAMES
			int otherX = m_actors[i]->getX();
			int otherY = m_actors[i]->getY();
			if (x < otherX + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 > otherX &&
				y < otherY + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 > otherY) {
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y) const
{
	if (euclidianDist(m_penelope->getX(), m_penelope->getY(), x, y) <= 100) {
		return true;
	}
	for (int i = 0; i < m_actors.size(); i++) {
		if (euclidianDist(m_actors[i]->getX(), m_actors[i]->getY(), x, y) <= 100 && m_actors[i]->triggersZombieVomit()) {
			return true;
		}
	}
	return false;
}

bool StudentWorld::locateNearestVomitTrigger(double x, double y, double & otherX, double & otherY, double & distance)
{
	double minimum = sqrt(euclidianDist(x, y, m_penelope->getX(), m_penelope->getX()));
	otherX = m_penelope->getX();
	otherY = m_penelope->getY();
	distance = minimum;

	for (int i = 0; i < m_actors.size(); i++)		// LOOP OVER ALL ACTORS
	{
		if (m_actors[i]->triggersZombieVomit())		// IF THIS ACTOR TRIGGERS ZOMBIE VOMIT
		{
			int x2 = m_actors[i]->getX();
			int y2 = m_actors[i]->getY();
			double tempDistance = sqrt(euclidianDist(x, y, x2, y2));
			if (tempDistance < minimum)
			{
				otherX = x2;
				otherY = y2;
				distance = tempDistance;
			}
		}
	}
	return false;
}

bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double & otherX, double & otherY, double & distance, bool & isThreat) const
{
	// COMPARES PENELOPE TO THE NEAREST ZOMBIE
	double zombieX = 0, zombieY = 0, zombieDistance = 0.0;
	double penelopeDistance = euclidianDist(x, y, m_penelope->getX(), m_penelope->getY());
	if (!locateNearestCitizenThreat(x, y, zombieX, zombieY, zombieDistance) && penelopeDistance <= 6400) // IF THERE IS NO CITIZEN THREAT
	{
		otherX = m_penelope->getX();
		otherY = m_penelope->getY();
		distance = penelopeDistance;
		isThreat = false;
		return true;
	} 
	else if (penelopeDistance < zombieDistance && penelopeDistance <= 6400)	// IF PENELOPE IS CLOSER THAN A ZOMBIE
	{
		otherX = m_penelope->getX();
		otherY = m_penelope->getY();
		distance = penelopeDistance;
		isThreat = false;
		return true;
	} 
	else if (zombieDistance <= 6400)		// IF ZOMBIE IS CLOSER THAN PENELOPE
	{
		otherX = zombieX;
		otherY = zombieY;
		distance = zombieDistance;
		isThreat = true;
		return true;
	}
	return false;
}


bool StudentWorld::locateNearestCitizenThreat(double x, double y, double & otherX, double & otherY, double & distance) const
{
	int zombieCount = 0;
	if (m_actors.size() == 0)	// IF THERE ARE NO ACTORS, RETURN IMMEDIATELY
	{
		return false;
	}
	distance = 100000;			// SET TO A MAX VALUE 
	if (m_actors[0]->threatensCitizens())		// IF A ZOMBIE IS FOUND
	{
		zombieCount++;
		otherX = m_actors[0]->getX();
		otherY = m_actors[0]->getY();
		distance = euclidianDist(x, y, m_actors[0]->getX(), m_actors[0]->getY());
	}

	for (int i = 1; i < m_actors.size(); i++)
	{
		if (m_actors[i]->threatensCitizens())	// IF A ZOMBIE IS FOUND
		{
			zombieCount++;
			int x2 = m_actors[i]->getX();
			int y2 = m_actors[i]->getY();
			double tempDistance = euclidianDist(x, y, x2, y2);
			if (tempDistance < distance)
			{
				distance = tempDistance;
				otherX = x2;
				otherY = y2;
			}
		}
	}
	if (zombieCount == 0) return false;		// INDICATES NO ZOMBIES FOUND
	return true;
}


bool StudentWorld::isLevelDone() {
	return m_numCitizensRemaining == 0;
}

std::string StudentWorld::createHeaderString()
{
	ostringstream oss;	// CREATE THE STRINGSTREAM
	oss << "Score: ";
	oss.fill('0');
	if (getScore() >= 0) {	
		oss << setw(6) << getScore() << "  ";
	}
	else {    // SPECIAL CASE TO HANDLE THE NEGATIVE SIGN SO IT SHOWS UP AS "-01000" INSTEAD OF "0-1000"
		oss << "-" << setw(5) << -getScore() << "  ";
	}
	oss << "Level: ";
	oss << getLevel() << "  ";
	oss << "Lives: ";
	oss << getLives() << "  ";
	oss << "Vaccines: ";
	oss << m_penelope->getNumVaccines() << "  ";
	oss << "Flames: ";
	oss << m_penelope->getNumFlameCharges() << "  ";
	oss << "Mines: ";
	oss << m_penelope->getNumLandmines() << "  ";
	oss << "Infected: ";
	oss << m_penelope->getInfectionDuration() << endl;

	return oss.str();
}

void StudentWorld::updateLevelString()
{
	if (getLevel() < 7)		// IF THERE ARE STILL MOVE LEVELS
	{
		ostringstream oss;
		oss << "level";
		oss.fill('0');
		oss << setw(2) << getLevel() << ".txt";
		m_levelString = oss.str();
	}
}