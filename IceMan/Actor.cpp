#include "Actor.h"
#include "StudentWorld.h"

/*
	Actor Class
*/
bool Actor::hasDied()
{
	if (getHealth() > 0)
		return false;
	else
		return true;
}

bool Actor::isAlive()
{
	return alive_;
}

bool Actor::hasCompletedLevel()
{
	if (barrelCount == getWorld()->getBarrelsRemaining())
		return true;
	else
		return false;
}

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

void Actor::setAlive(bool n)
{
	alive_ = n;
}

StudentWorld* Actor::getWorld()
{
	return sw_;
}



/*
	IceMan Class
*/
void Iceman::doSomething()
{
	//Detecting if Player is overlaying a visible Ice Object
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
	
	//Player move input 
	int  ch;
	if (Actor::getWorld()->getKey(ch) == true)
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
			//Exit level
			
			break;
		}
	}
}

int Iceman::getHealth()
{
	return Actor::getHealth();
}

/*
	Boulder Class
*/
void Boulder::doSomething()
{
	bool isStable = true;
	int count = 0;
	for (int h = 0; h < 4; h++)
	{
		for (int g = 0; g < 4; g++)
		{
			if (getWorld()->iceField[getX() + g][getY() -1 - h] == nullptr)
			{
				count++;
			}
		}
	}
	if (count == 16)
	{
		isStable = false;
	}
	if (!isStable)
	{
		while (getWorld()->iceField[getX()][getY() - 1] == nullptr && getY() > 1)
		{
			getWorld()->playSound(SOUND_FALLING_ROCK);
			moveTo(getX(), getY() - 1);
		}
		setAlive(false);
	}
}
	
void Barrel::doSomething()
{
	//Calculate distance from Iceman
	int ix = getWorld()->currentActorVector[0]->getX();
	int iy = getWorld()->currentActorVector[0]->getY();
	int bx = getX();
	int by = getY();
	int x = abs(ix - bx);
	int y = abs(iy - by);
	int radius = sqrt(x * x + y * y);
	if (isVisible() == false && radius <= 4)
	{
		setVisible(true);
		return;
	}
}
