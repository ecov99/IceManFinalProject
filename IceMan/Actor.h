#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <memory>

class StudentWorld;

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

class Protestor : public Actor
{

};

class RegularProtestor : public Protestor
{

};

class HardcoreProtestor : public Protestor
{

};

class Squirt : public Actor
{

};

class Barrel : public Actor
{

};

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

class GoldNugget : public Actor
{

};

class SonarKit : public Actor
{

};

class Water : public Actor
{

};


#endif // ACTOR_H_
