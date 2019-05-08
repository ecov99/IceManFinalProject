#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <memory>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
		: GraphObject(imageID, startX, startY, dir, size, depth), alive_(true), sw_(sw), annoyed_(false), hitPoints_(10),
		waterUnits_(5), solarCharge_(1), goldNuggetWallet_(0) {}
	Actor(StudentWorld* sw, int x, int y) : GraphObject(NULL, x, y, Actor::right, 1, 0) {}
	
	
	virtual ~Actor() {}
	
private:
	bool alive_;
	int hitPoints_;
	bool annoyed_;
	int waterUnits_;
	int solarCharge_;
	int goldNuggetWallet_;
	

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
	virtual void doSomething()
	{
	}
private:

};
class Ice : public Actor
{
public:
	//10 and 60 are dummy variables
	Ice(StudentWorld* sw, int x, int y) :Actor(sw, IID_ICE, x, y, Actor::right, .25, 3) {
		GraphObject::setVisible(true);
	}
	~Ice() {}
	virtual void doSomething()
	{

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

class Squrit : public Actor
{

};
class Barrel : public Actor
{

};

class Boulder : public Actor
{

};

class GoldNugget : public Actor
{

};

class SolarKit : public Actor
{

};

class Water : public Actor
{

};


#endif // ACTOR_H_
