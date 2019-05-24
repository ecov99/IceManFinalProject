#ifndef ACTOR_H_
#define ACTOR_H_

/*
	Actor.h will define ALL class ctors/dtors.
	Will also declare functions and data members.
*/
#include "GraphObject.h"
#include <algorithm>
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
	double calcDistance(int x, int y);

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
	Item(StudentWorld* sw, int id, int x, int y, Direction dir, int size, int depth)
		: Actor(sw, id, x, y, dir, size, depth)
	{}
	virtual ~Item() {}

	// behaviors
	bool IcemanCanPickUp();
	bool ProtestorCanPickUp();

private:
	// attributes
	bool IcemanCanPickUp_;
	bool ProtestorCanPickUp_;
};

/*
	CLASS: Barrel (Oil)
	Once all Barrels are picked up from Iceman, the level ends.
*/
class Barrel : public Item
{
public:
	// ctors & dtors
	Barrel(StudentWorld* sw, int x, int y) : Item(sw, IID_BARREL, x, y, right, 1, 2)
	{
		GraphObject::setVisible(true);
	}
	~Barrel() {}

	// behaviors
	virtual void doSomething();

private:
	// attributes
};
/*
	CLASS: Gold
*/
class Gold : public Item
{
public:
	// ctors & dtors
	Gold(StudentWorld* sw, int x, int y, bool tempState) : Item(sw, IID_GOLD, x, y, right, 1, 2)
	{
		tempCount_ = 100;
		tempState_ = tempState;
		if (tempState_ == false)
			GraphObject::setVisible(false);
		else
			GraphObject::setVisible(true);
	}
	~Gold() {}

	// behaviors
	virtual void doSomething();
	bool isTemp();
	int getTempCount();

private:
	// attributes
	bool tempState_;
	int tempCount_;
};
/*
	Water Class
*/
class Water : public Item
{
public:
};
/*
	Sonar Class
*/
class Sonar : public Item
{
public:
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
	}
	virtual ~Character() {}

	// behaviors
	virtual void annoyed() {}
	int getHealth();
	int getNumOfGold();
	bool hasDied();
	void incGold();

private:
	// attributes
	int health_;
	int numOfGold_;
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
		numOfOil_ = 0;
		GraphObject::setVisible(true);
	}
	~Iceman() {}

	// behaviors
	virtual void doSomething();
	bool checkForBoulders(int k);
	int getNumOfSquirts();
	int getNumOfSonars();
	void increaseNumOfOil();

private:
	// attributes
	int numOfSquirts_;
	int numOfSonars_;
	int numOfOil_;
	StudentWorld* sw_;
};

class Protestor : public Character {
public:
	// ctors & dtors
	Protestor(StudentWorld* sw, int IID, int health, int level) : Character(sw, IID, 60, 60, left, 1, 0, health)
	{
		setVisible(true);
		leaveOilFieldState_ = false;
		//numSquaresToMoveInCurrentDirection_ = updateMobilityCount();
		level_ = level;
		ticksToWaitBetweenMoves_ = max(0, 3 - level / 4);
	}

	//behaviors
	//int updateMobilityCount(); //updates how many squares the protestor will move in a given direction
	virtual void doSomething() {}
	// attributes
	bool leaveOilFieldState_;
	int numSquaresToMoveInCurrentDirection_;
	int ticksToWaitBetweenMoves_;
	int level_;

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
