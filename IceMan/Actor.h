#ifndef ACTOR_H_
#define ACTOR_H_

/*
	Actor.h will define ALL class ctors/dtors.
	Will also declare functions and data members.
*/
#include "GraphObject.h"
#include <algorithm>
#include <queue>
using namespace std;

class StudentWorld;

/*
	CLASS: Actor
	Abstract Base Class for all other classes.
	Cannot be instantiated.
	Child Classes: Ice, Boulder, Squirt, Item, Character
*/
class Actor : public GraphObject
{
public:
	// ctors & dtors
	Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
		: GraphObject(imageID, startX, startY, dir, size, depth)
	{
		active_ = true;
		sw_ = sw;
	}
	virtual ~Actor()
	{}

	// behaviors
	virtual void doSomething() {};
	bool isActive();
	void setActive(bool b);
	StudentWorld* getWorld();
	double calcDistance(Actor &other);
	double calcDistance(Actor &other, int &tempID);
	double calcDistance(int x, int y);
	bool checkForBoulders(int dir);
	bool checkForIce();

private:
	// attributes
	bool active_;
	StudentWorld* sw_;

};

/*
	CLASS: Ice
	Doesn't do anything except populate iceField
*/
class Ice : public Actor
{
public:
	// ctors & dtors
	Ice(StudentWorld* sw, int x, int y) : Actor(sw, IID_ICE, x, y, right, .25, 3)
	{
		GraphObject::setVisible(true);
	}
	~Ice() {}

	// behaviors
	virtual void doSomething() {}	// ice doesn't do anything
};
/*
	CLASS: Boulder
	Starts off stable. Falls once ice below is mined by Iceman.
*/
class Boulder : public Actor
{
public:
	// ctors & dtors
	Boulder(StudentWorld* sw, int x, int y) : Actor(sw, IID_BOULDER, x, y, down, 1, 1)
	{
		GraphObject::setVisible(true);
		stable_ = true;
		waitingToFall_ = false;
		fallWaitCount_ = 20;
		falling_ = false;
		collided_ = false;
	}
	~Boulder() {}

	// behaviors
	virtual void doSomething();
	bool isStable();
	void setStable(bool b);
	bool isWaitingToFall();
	void setWaitingToFall(bool b);
	bool isFalling();
	void setFalling(bool b);
	bool hasCollided();
	void setCollided(bool b);
	bool checkForIceBelow();
	Actor* checkCollision(int &victimID);

private:
	// attributes
	bool stable_;
	bool waitingToFall_;
	int fallWaitCount_;
	bool falling_;
	bool collided_;
};
/*
	Squirt Class
*/
class Squirt : public Actor
{
public:
	// ctors & dtors
	Squirt(StudentWorld* sw, int x, int y, Direction dir ) : Actor(sw, IID_WATER_SPURT, x, y, dir, 1, 1)
	{
		GraphObject::setVisible(true);
		movementCount_ = 0;
		moving_ = true;
	}
	~Squirt() {}

	// behaviors
	virtual void doSomething();
	int getMovementCount();
	void incMovementCount();
	bool isMoving();
	void setMoving(bool b);
	Actor* checkForProtestors(int dir, int &tempID);

	// attribute
	int movementCount_;
	bool moving_;
};

/*
	CLASS: Item
	Abstract Base Class for all items that can be picked up.
	Cannot be instantiated.
	They will have a state, temporary or permanent.
	Child Classes: Barrel, Gold, Water
*/
class Item : public Actor
{
public:
	// ctors & dtors
	Item(StudentWorld* sw, int id, int x, int y, Direction dir, int size, int depth, bool tempState)
		: Actor(sw, id, x, y, dir, size, depth)
	{
		tempState_ = tempState;
		tempCount_ = 0;	// default for item class
	}
	virtual ~Item() {}

	// behaviors
	bool isTemp();

	// attributes
	int tempCount_;
private:
	bool tempState_;
};

/*
	CLASS: Barrel (Oil)
	Once all Barrels are picked up from Iceman, the level ends.
*/
class Barrel : public Item
{
public:
	// ctors & dtors
	Barrel(StudentWorld* sw, int x, int y) : Item(sw, IID_BARREL, x, y, right, 1, 2, false)
	{
		GraphObject::setVisible(false);
	}
	~Barrel() {}

	// behaviors
	virtual void doSomething();

};
/*
	CLASS: Gold
*/
class Gold : public Item
{
public:
	// ctors & dtors
	Gold(StudentWorld* sw, int x, int y, bool tempState) : Item(sw, IID_GOLD, x, y, right, 1, 2, tempState)
	{
		if (tempState == false)
			GraphObject::setVisible(false);
		else if (tempState == true)
			GraphObject::setVisible(true);

		tempCount_ = 100;
	}
	~Gold() {}

	// behaviors
	virtual void doSomething();

};
/*
	Water Class
*/
class Water : public Item
{
public:
	// ctors & dtors
	Water(StudentWorld* sw, int x, int y, int level) : Item(sw, IID_WATER_POOL, x, y, right, 1, 2, true)
	{
		tempCount_ = max(100, 300 - 10 * level);
		GraphObject::setVisible(true);
	}
	~Water() {}

	// behaviors
	virtual void doSomething();
};
/*
	Sonar Class
*/
class Sonar : public Item
{
public:
	// ctors & dtors
	Sonar(StudentWorld* sw, int x, int y, int level) : Item(sw, IID_SONAR, x, y, right, 1, 2, true)
	{
		tempCount_ = max(100, 300-10*level);
		GraphObject::setVisible(true);
	}
	~Sonar() {}

	// behaviors
	virtual void doSomething();

};

/*
	CLASS: Character
	Abstract Base Class for all objects that can be annoyed()
	Cannot be instantiated.
	Child Classes: Iceman, Protestor
*/
class Character : public Actor {
public:
	// ctors & dtors
	Character(StudentWorld* sw, int id, int x, int y, Direction dir, int size, int depth, int h)
		: Actor(sw, id, x, y, dir, size, depth)
	{
		health_ = h;
		numOfGold_ = 0;
		annoyed_ = false;
	}
	virtual ~Character() {}

	// behaviors
	virtual void annoyed(int fromID) {}
	bool isAnnoyed();
	void setAnnoyed(bool b);
	int getHealth();
	int getNumOfGold();
	bool hasDied();
	void incGold();
	void decGold();
	void decreaseHealth(unsigned int h);

	// attributes
	int health_;
private:
	int numOfGold_;
	bool annoyed_;
};

/*
	CLASS: Iceman
	User controlled player.
*/
class Iceman : public Character {
public:
	// ctors & dtors
	Iceman(StudentWorld* sw) : Character(sw, IID_PLAYER, 30, 60, right, 1, 0, 10)
	{
		numOfSquirts_ = 5;
		numOfSonars_ = 1;
		GraphObject::setVisible(true);
	}
	~Iceman() {}

	// behaviors
	virtual void doSomething();
	virtual void annoyed(int fromID);
	int getNumOfSquirts();
	void incNumOfSquirts();
	void decNumOfSquirts();
	int getNumOfSonars();
	void incNumOfSonars();
	void decNumOfSonars();
	bool checkSquirtLocation(int direction);

private:
	// attributes
	int numOfSquirts_;
	int numOfSonars_;
	StudentWorld* sw_;
};

class Protestor : public Character {
public:
	// ctors & dtors
	Protestor(StudentWorld* sw, int IID, int health, int level) : Character(sw, IID, 30, 30, left, 1, 0, health)
	{
		setVisible(true);
		level_ = level;
		restingCount_ = max(0, 3 - level / 4);
		yellingCount_ = 15;
		turningCount_ = 200;
		numSquaresToMoveInCurrentDirection_ = updateMobilityCount();

		resting_ = false;
		waitingToYell_ = false;
		leaving_ = false;
		killedByBoulder_ = false;
		killedByIceman_ = false;
	}

	//behaviors
	virtual void doSomething() {}
	int updateMobilityCount(); //updates how many squares the protestor will move in a given direction
	void genNewDirection();
	bool checkForIcemanInRadius();
	bool hasLineOfSight();
	void testLineOfSight();
	bool isFacingIceman();
	void faceIcemanAndMove();
	bool isResting();
	void setResting(bool b);
	void resetRestingCount(int level);
	bool isWaitingToYell();
	void setWaitingToYell(bool b);
	void resetWaitingToYellCount();
	bool isLeaving();
	void setLeaving(bool b);
	void isStunned();

	// attributes
	int numSquaresToMoveInCurrentDirection_;
	int level_;
	bool killedByBoulder_;
	bool killedByIceman_;

	bool resting_;
	int restingCount_;
	bool waitingToYell_;
	int yellingCount_;
	bool leaving_;
	int turningCount_;
};

class RegularProtestor : public Protestor {
public:
	RegularProtestor(StudentWorld* sw, int level) : Protestor(sw, IID_PROTESTER, 5, level) {}

	//behaviors
	virtual void doSomething();

};

class HardcoreProtestor : public Protestor {
public:
	HardcoreProtestor(StudentWorld* sw, int level) : Protestor(sw, IID_HARD_CORE_PROTESTER, 20, level) {}

	//behaviors
	virtual void doSomething();
};

#endif // ACTOR_H_
