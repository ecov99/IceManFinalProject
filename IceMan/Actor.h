#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class foo
{
public:
	int chooseACourseOfAction() { return 0; } //Dummy version
};

class Actor: public GraphObject
{
public:
	Actor();
	Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
	~Actor();


};

class Iceman : public Actor
{
public:
	Iceman(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
	{
		setVisible(true);
	}
	~Iceman() {}
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

class Ice : public Actor
{

};
#endif // ACTOR_H_
