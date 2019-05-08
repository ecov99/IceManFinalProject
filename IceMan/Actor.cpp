#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


int Actor::getHealth()
{
	return hitPoints_;
}

int Actor::getSquirts()
{
	return waterUnits_;
}

int Actor::getGold()
{
	return goldNuggetWallet_;
}

int Actor::getBarrelsLeft()
{
	return barrelCount;
}

int Actor::getSonar()
{
	return sonarCharge_;
}

int Iceman::getHealth()
{
	return Actor::getHealth();
}

