#include "Actor.h"
#include "StudentWorld.h"

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

void Boulder::doSomething()
{
	if (isAlive())
	{
		bool isStable = true;
		for (int h = 0; h < 4; h++)
		{
			for (int g = 0; g < 4; g++)
			{
				if (getWorld()->iceField[getX() + g][getY() - h] == nullptr)
				{
					isStable = false;
				}
			}
		}
		if (!isStable)
		{
			while (getWorld()->iceField[getX()][getY() - 1] == nullptr)
			{
				if (getY() > 0)
				{
					moveTo(getX(), getY() - 1);
				}
			}
			setAlive(false);
		}
	}
	else
	{
		return;
	}
}
