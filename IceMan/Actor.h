#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <memory>

class StudentWorld;

/*
	Actor Class
*/
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
		: GraphObject(imageID, startX, startY, dir, size, depth), alive_(true), sw_(sw), annoyed_(false), hitPoints_(10),
		waterUnits_(5), sonarCharge_(1), goldNuggetWallet_(0), barrelCount(0) {}
	Actor(StudentWorld* sw, int x, int y) : GraphObject(NULL, x, y, Actor::right, 1, 0) {}
	
	
	virtual ~Actor() {}

	virtual void doSomething() {}
	virtual bool hasDied();
	virtual bool isAlive();
	virtual bool hasCompletedLevel();
	virtual int getHealth();
	virtual int getSquirts();
	virtual int getGold();
	virtual int getBarrelsLeft();
	virtual int getSonar();
	virtual void setAlive(bool n);
	virtual StudentWorld* getWorld();
	virtual void increaseGoldWallet();
	virtual void decreaseGoldNugget();
private:
	bool alive_;
	int hitPoints_;
	bool annoyed_;
	int waterUnits_;
	int sonarCharge_;
	int goldNuggetWallet_;
	int barrelCount;
	

	StudentWorld* sw_;
	
};

/*
	Ice Class
*/
class Ice : public Actor
{
public:
	Ice(StudentWorld* sw, int x, int y) :Actor(sw, IID_ICE, x, y, Actor::right, .25, 3)
	{
		GraphObject::setVisible(true);
	}
	~Ice() {}
	virtual void doSomething()
	{
		//doSomething Function serves no purpose
	}
};

/*
	Player Class
*/
class Player : public Actor
{
public:

};

/*
	IceMan Class
*/
class Iceman : public Actor
{
public:
	Iceman(StudentWorld* sw):Actor(sw, IID_PLAYER, 30, 60, Actor::right, 1, 0)
	{
		GraphObject::setVisible(true);
	}
	
	~Iceman() {}
	virtual void doSomething();
	

	int getHealth();
private:
	
};


/*
	Protestor
*/
class Protestor : public Actor
{
public:
};

/*
	RegularProtestor Class
*/
class RegularProtestor : public Protestor
{
public:
};

/*
	HardcoreProtestor Class
*/
class HardcoreProtestor : public Protestor
{
public:
};

/*
	Item Class
*/
class Item : public Actor
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
	Barrel Class
*/
class Barrel : public Actor
{
public:
	Barrel(StudentWorld* sw, int x, int y) :Actor(sw, IID_BARREL, x, y, Actor::right, 1, 2)
	{
		GraphObject::setVisible(false);
	}
	~Barrel() {}
	virtual void doSomething();
};

/*
	Boulder Class
*/
class Boulder : public Actor
{
public:
	Boulder(StudentWorld* sw, int x, int y) : Actor(sw, IID_BOULDER, x, y, Actor::down, 1, 1)
	{
		GraphObject::setVisible(true);
	}
	~Boulder() {}
	virtual void doSomething();

};

/*
	GoldNugget Class
*/
class GoldNugget : public Actor
{
public:
	GoldNugget(StudentWorld* sw, int x, int y, bool isVis) :Actor(sw, IID_GOLD, x, y, Actor::right, 1, 2)
	{
		GraphObject::setVisible(isVis);
	}
	~GoldNugget() {}
	virtual void doSomething() {}
};

/*
	SonarKit Class
*/
class SonarKit : public Actor
{
public:
};

/*
	Water Class
*/
class Water : public Actor
{
public:
};


#endif // ACTOR_H_
