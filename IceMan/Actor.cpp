/*
	Actor.cpp will define all functions, except for ctors/dtors.
	Has all logic and implementation.
*/
#include "Actor.h"
#include "StudentWorld.h"

/*
	CLASS: Actor
	Abstract Base Class for all other classes.
*/
//void Actor::doSomething() {}	// doesn't do anything
bool Actor::isActive() {
	return active_;
}
void Actor::setActive(bool b) {
	active_ = b;
}
StudentWorld* Actor::getWorld() {
	return sw_;
}

/*
	CLASS: Character
	Abstract Base Class for all objects that can be annoyed()
		(IceMan and both Protestors)
*/
void Character::annoyed() {}	// doesn't do anything

/*
	CLASS: Player
	Abstract Base Class for all objects that can be annoyed()
		(IceMan and both Protestors)
*/
int Character::getHealth() {
	return health_;
}
bool Character::hasDied() {
	if (health_ <= 0)
		return true;
	else
		return false;
}

/*
	CLASS: Iceman
	User controlled player.
*/
void Iceman::doSomething()
{
	// Detecting if player is overlaying a visible Ice object
	bool isCovered = false;
	for (int h = 0; h < 4; h++)
	{
		for (int g = 0; g < 4; g++)
		{
			if (getWorld()->iceField[getX() + g][getY() + h] != nullptr)
			{
				isCovered = true;
			}
		}
	}
	if (isCovered)
	{
		
		getWorld()->playSound(SOUND_DIG);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				getWorld()->increaseScore(10);
				getWorld()->iceField[getX() + j][getY() + i].reset();
				getWorld()->iceField[getX() + j][getY() + i] = nullptr;
			}
		}
	}
	
	// player input 
	int  ch;
	if (getWorld()->getKey(ch) == true)
	{
		//User hit a key this tick
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			//Move player to the left
			if (getX() > 0)
			{
				moveTo(getX() - 1, getY());
				setDirection(left);
			}
			break;
		case KEY_PRESS_RIGHT:
			//Move player to the right
			if (getX() < 60)
			{
				moveTo(getX() + 1, getY());
				setDirection(right);
			}
			break;
		case KEY_PRESS_DOWN:
			//Move player down
			if (getY() > 0)
			{
				moveTo(getX(), getY() - 1);
				setDirection(down);
			}
			break;
		case KEY_PRESS_UP:
			//Move player up
			if (getY() < 60)
			{
				moveTo(getX(), getY() + 1);
				setDirection(up);
			}
			break;
		case KEY_PRESS_SPACE:
			//Add a squirt in front of the player
			break;
		case KEY_PRESS_TAB:
			//
			break;
		case KEY_PRESS_ESCAPE:
			// stop program
			exit(0);
			break;
		}
	}
}
int Iceman::getNumOfSquirts() {
	return numOfSquirts_;
}
int Iceman::getNumOfSonars() {
	return numOfSonars_;
}
int Iceman::getNumOfGold() {
	return numOfGold_;
}

/*
	CLASS: Boulder
*/
void Boulder::doSomething()
{
	// boulder is in the ice
	if (isStable() == true && isFalling() == false && hasCollided() == false) {

		// checks for ice below if boulder is not waiting to fall
		if (isWaitingToFall() == false) {
			int count = 0;
			for (int h = 0; h < 4; h++)
			{
				for (int g = 0; g < 4; g++)
				{
					if (getWorld()->iceField[getX() + g][getY() - 1 - h] == nullptr)
						count++;

				}
			}
			// boulder is now waiting to fall
			if (count == 16)
				setWaitingToFall(true);
		}
		else {	// gives boulder a pause before falling
			fallWaitCount_--;
			if (fallWaitCount_ <= 0)
			{
				setStable(false);
				setFalling(true);
				// play sound for boulder falling
				getWorld()->playSound(SOUND_FALLING_ROCK);
			}
		}

	}

	// boulder is falling
	if (isStable() == false && isFalling() == true && hasCollided() == false){
		
		if (getWorld()->iceField[getX()][getY() - 1] == nullptr && getY() > 1)
		{
			moveTo(getX(), getY() - 1);
		}

		//Change collision if neccessary
		if (getWorld()->iceField[getX()][getY() - 1] != nullptr || getY() == 1)
		{
			setCollided(true);
		}
	}
	//Collision check
	if (hasCollided() == true)
	{
		setActive(false);
	}
}
bool Boulder::isStable() {
	return stable_;
}
void Boulder::setStable(bool b) {
	stable_ = b;
}
bool Boulder::isWaitingToFall() {
	return waitingToFall_;
}
void Boulder::setWaitingToFall(bool b) {
	waitingToFall_ = b;
}
bool Boulder::isFalling() {
	return falling_;
}
void Boulder::setFalling(bool b) {
	falling_ = b;
}
bool Boulder::hasCollided() {
	return collided_;
}
void Boulder::setCollided(bool b) {
	collided_ = b;
}

/*
	CLASS: Barrel
*/
void Barrel::doSomething()
{
	//Calculate distance from Iceman
	int ix = getWorld()->currentActorVector[0]->getX();
	int iy = getWorld()->currentActorVector[0]->getY();
	int bx = getX();
	int by = getY();
	int x = std::abs(ix - bx);
	int y = std::abs(iy - by);
	double radius = sqrt(x * x + y * y);
	if (isVisible() == false && radius <= 4)
	{
		setVisible(true);
		return;
	}
	
}
