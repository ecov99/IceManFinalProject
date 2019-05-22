#ifndef ACTOR_H_
#define ACTOR_H_

/*
	Actor.h will define ALL class ctors/dtors.
	Will also declare functions and data members.
*/
#include "GraphObject.h"
#include <memory>
using namespace std;

class StudentWorld;

/*
	CLASS: Actor
Base Class for all other classes.
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
	virtual void doSomething() = 0;
	bool isActive();
	void setActive(bool b);
	StudentWorld* getWorld();

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
	CLASS: Character
	Abstract Base Class for all objects that can be annoyed()
		(IceMan and both Protestors)
*/
class Character : public Actor {
public:
	// ctors & dtors
	Character(StudentWorld* sw, int id, int x, int y, Direction dir, int size, int depth, int h)
		: Actor(sw, id, x, y, dir, size, depth)
	{
		health_ = h;
	}
	virtual ~Character() {}

	// behaviors
	virtual void annoyed();
	virtual bool collisionCheck(int id);
	int getHealth();
	bool hasDied();

private:
	// attributes
	int health_;
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
		numOfGold_ = 0;
		GraphObject::setVisible(true);
	}
	~Iceman() {}

	// behaviors
	virtual void doSomething();
	int getNumOfSquirts();
	int getNumOfSonars();
	int getNumOfGold();

private:
	// attributes
	int numOfSquirts_;
	int numOfSonars_;
	int numOfGold_;
	StudentWorld* sw_;
};

/*
	CLASS: Item
	Base Class for all items.
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
private:
	// attributes
};
/*
	CLASS: Boulder
*/
class Boulder : public Item
{
public:
	// ctors & dtors
	Boulder(StudentWorld* sw, int x, int y) : Item(sw, IID_BOULDER, x, y, down, 1, 1)
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
	CLASS: Barrel
*/
class Barrel : public Item
{
public:
	// ctors & dtors
	Barrel(StudentWorld* sw, int x, int y) : Item(sw, IID_BARREL, x, y, right, 1, 2)
	{
		GraphObject::setVisible(false);
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
	Gold(StudentWorld* sw, int x, int y, bool isVis) : Item(sw, IID_GOLD, x, y, right, 1, 2)
	{
		GraphObject::setVisible(isVis);
	}
	~Gold() {}

	// behaviors
	virtual void doSomething() {}

private:
	// attributes
};
/*
	Water Class
*/
class Water : public Item
{
public:
};

/*
	Squirt Class
*/
class Squirt : public Item
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

#endif // ACTOR_H_
