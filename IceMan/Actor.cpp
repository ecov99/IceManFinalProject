/*****************************************************
	Actor.cpp will define all functions, except for ctors/dtors.
	Has all logic and implementation.
****************************************************/
#include "Actor.h"
#include "StudentWorld.h"


/****************************************************
	CLASS: Actor
	Abstract Base Class for all other classes.
	Cannot be instantiated.
	Child Classes: Ice, Boulder, Squirt, Item, Character
****************************************************/
bool Actor::isActive() {
	return active_;
}
void Actor::setActive(bool b) {
	active_ = b;
}
StudentWorld* Actor::getWorld() {
	return sw_;
}
double Actor::calcDistance(Actor &other) {
	double Ax = this->getX();
	double Ay = this->getY();
	double Bx = other.getX();
	double By = other.getY();
	return sqrt((pow(Ax - Bx, 2) + pow(Ay - By, 2)));
}
double Actor::calcDistance(Actor &other, int &tempID) {
	tempID = other.getID();

	double Ax = this->getX();
	double Ay = this->getY();
	double Bx = other.getX();
	double By = other.getY();
	return sqrt((pow(Ax - Bx, 2) + pow(Ay - By, 2)));
}
double Actor::calcDistance(int x, int y) {
	double Ax = this->getX();
	double Ay = this->getY();
	double Bx = x;
	double By = y;
	return sqrt((pow(Ax - Bx, 2) + pow(Ay - By, 2)));
}
bool Actor::checkForBoulders(int dir) {
	double temp;

	// loop through current actors vector
	for (int i = 0; i < getWorld()->currentActors.size(); i++) {

		// check only boulders
		if (getWorld()->currentActors[i]->getID() == IID_BOULDER) {

			// check distance Actor is from each boulder
			temp = calcDistance(*getWorld()->currentActors[i]);

			// if boulder is within radius of 3.5
			if (temp <= 3) {
				// check for going up, if boulder is down
				if (dir == KEY_PRESS_UP && getWorld()->currentActors[i]->getY() < this->getY())
					return true;	// it's okay

				// check for going down, if boulder is up
				if (dir == KEY_PRESS_DOWN && getWorld()->currentActors[i]->getY() > this->getY())
					return true;	// it's okay

				// check for going left, if boulder is to the right
				if (dir == KEY_PRESS_LEFT && getWorld()->currentActors[i]->getX() > this->getX())
					return true;	// it's okay

				// check for going right, if boulder is to the left
				if (dir == KEY_PRESS_RIGHT && getWorld()->currentActors[i]->getX() < this->getX())
					return true;	// it's okay

				// otherwise, boulder is in direction of movement
				return false;		// not okay
			}
		}
	}
	return true;	// Actor is not near boulder
}

bool Actor::checkForIce() {
	bool isCovered = false;
	for (int h = 0; h < 4; h++)
	{
		for (int g = 0; g < 4; g++)
		{
			if (getWorld()->iceField_[getX() + g][getY() + h] != nullptr)
			{
				isCovered = true;
			}
		}
	}
	return isCovered;
}

//Coordinate Actor::getLocation()
//{
//	return Location_;
//}


/******************************************
	CLASS: Boulder
	Starts off stable.
	Falls once ice below is mined by Iceman.
******************************************/
void Boulder::doSomething() {
	// boulder is in the ice
	if (isStable() == true && isFalling() == false && hasCollided() == false) {

		// checks for ice below if boulder is not waiting to fall
		if (isWaitingToFall() == false) {
			int count = 0;
			for (int h = 0; h < 4; h++)
			{
				for (int g = 0; g < 4; g++)
				{
					if (getY() > 4) {
						if (getWorld()->iceField_[getX() + g][getY() - 1 - h] == nullptr)
							count++;
					}
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
				getWorld()->playSound(SOUND_FALLING_ROCK);	// play sound for boulder falling
			}
		}
	}

	// boulder is falling
	if (isStable() == false && isFalling() == true && hasCollided() == false) {
		
		int tempID = -1;	// initialize tempID to dummy value

		// pass tempID by reference in order to update value
		Actor* temp = checkCollision(tempID);
		if (temp != nullptr) {	// if collided with active Actor
			
			// if collided with ICEMAN
			if (tempID == IID_PLAYER)		
				getWorld()->takeDamage(*this, *temp);
	
			// if collided with REGULAR PROTESTOR
			else if (tempID == IID_PROTESTER)
				getWorld()->takeDamage(*this, *temp);

			// if collided with HC PROTESTOR
			else if (tempID == IID_PROTESTER)
				getWorld()->takeDamage(*this, *temp);

			/************************************************
			DELETED: setCollided(true);
				Boulder is allowed to fall through objects
			************************************************/
		}

		// if no ice, then move
		if (checkForIceBelow() == false)		
			moveTo(getX(), getY() - 1);
		// otherwise, hit ice
		else							
			setCollided(true);

		// if bottom of field is reached
		if (getY() == 1)				
			setCollided(true);
	}
	// check collision
	if (hasCollided() == true)
		setActive(false);
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
bool Boulder::checkForIceBelow() {
	// if there is no ice below
	if (getY() > 1 && getWorld()->iceField_[getX()][getY() - 1] == nullptr)
		return false;
	// otherwise there is ice below
	else
		return true;
}
Actor* Boulder::checkCollision(int &victimID) {
	// loop through active Actors
	for (int i = 0; i < getWorld()->currentActors.size(); i++) {
		// if Actor is Iceman or a Protestor
		if (getWorld()->currentActors[i]->getID() == IID_PLAYER ||
			getWorld()->currentActors[i]->getID() == IID_PROTESTER ||
			getWorld()->currentActors[i]->getID() == IID_HARD_CORE_PROTESTER)
		{
			// if Boulder is above Actor
			if (getY() - 4 == getWorld()->currentActors[i]->getY() &&
				getX() + 4 > getWorld()->currentActors[i]->getX() &&
				getX() - 4 < getWorld()->currentActors[i]->getX())
			{
				victimID = getWorld()->currentActors[i]->getID();
				Actor* temp = getWorld()->currentActors[i];
				return temp;
			}
		}
	}
	return nullptr;
}


/******************************************
	CLASS: Squirt
******************************************/
void Squirt::doSomething() {
	
	int tempID = -1;
	Actor* tempActor;

	// MOVE UP
	if (this->getDirection() == up && checkForBoulders(this->getDirection()) && checkForIce() == false) {
		tempActor = checkForProtestors(getDirection(), tempID);
		if (tempActor != nullptr)
		{
			setActive(false);
			getWorld()->takeDamage(*this, *tempActor);
			getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		}
		else
		{
			moveTo(getX(), getY() + 1);
			incMovementCount();
		}
	}

	// MOVE DOWN
	else if (this->getDirection() == down && checkForBoulders(this->getDirection()) && checkForIce() == false) {
		tempActor = checkForProtestors(getDirection(), tempID);
		if (tempActor != nullptr)
		{
			setActive(false);
			getWorld()->takeDamage(*this, *tempActor);
			getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		}
		else
		{
			moveTo(getX(), getY() - 1);
			incMovementCount();
		}
	}

	// MOVE LEFT
	else if (this->getDirection() == left && checkForBoulders(this->getDirection()) && checkForIce() == false) {
		tempActor = checkForProtestors(getDirection(), tempID);
		if (tempActor != nullptr)
		{
			setActive(false);
			getWorld()->takeDamage(*this, *tempActor);
			getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		}
		else
		{
			moveTo(getX() - 1, getY());
			incMovementCount();
		}
	}

	// MOVE RIGHT
	else if (this->getDirection() == right && checkForBoulders(this->getDirection()) && checkForIce() == false) {
		tempActor = checkForProtestors(getDirection(), tempID);
		if (tempActor != nullptr)
		{
			setActive(false);
			getWorld()->takeDamage(*this, *tempActor);
			getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		}
		else
		{
			moveTo(getX() + 1, getY());
			incMovementCount();
		}
	}
	else {
		setMoving(false);
	}

	if (getMovementCount() > 4 || isMoving() == false)
		setActive(false);
}

int Squirt::getMovementCount() {
	return movementCount_;
}
void Squirt::incMovementCount() {
	movementCount_++;
}
bool Squirt::isMoving() {
	return moving_;
}
void Squirt::setMoving(bool b) {
	moving_ = b;
}

Actor* Squirt::checkForProtestors(int dir, int &tempID)
{
	double temp;

	// loop through current actors vector
	for (int i = 0; i < getWorld()->currentActors.size(); i++) {

		// check only boulders
		if (getWorld()->currentActors[i]->getID() == (IID_PROTESTER || IID_HARD_CORE_PROTESTER)) {

			// check distance Actor is from each boulder
			temp = calcDistance(*getWorld()->currentActors[i]);

			// if Protestor is within radius of 3
			if (temp <= 3) {
				// check for going up, if Protestor is up
				if (dir == up && getWorld()->currentActors[i]->getY() > this->getY())
				{
					tempID = getWorld()->currentActors[i]->getID();
					Actor* tempProt = getWorld()->currentActors[i];
					return tempProt;	// it's okay
				}

				// check for going down, if Protestor is down
				if (dir == down && getWorld()->currentActors[i]->getY() < this->getY())
				{
					tempID = getWorld()->currentActors[i]->getID();
					Actor* tempProt = getWorld()->currentActors[i];
					return tempProt;	// it's okay
				}

				// check for going left, if Protestor is to the left
				if (dir == left && getWorld()->currentActors[i]->getX() < this->getX())
				{
					tempID = getWorld()->currentActors[i]->getID();
					Actor* tempProt = getWorld()->currentActors[i];
					return tempProt;	// it's okay
				}

				// check for going right, if Protestor is to the right
				if (dir == right && getWorld()->currentActors[i]->getX() > this->getX())
				{
					tempID = getWorld()->currentActors[i]->getID();
					Actor* tempProt = getWorld()->currentActors[i];
					return tempProt;	// it's okay
				}

			}
		}
	}
	return nullptr;
}


/******************************************
	CLASS: Item
	Abstract Base Class for all items that can be picked up.
	Cannot be instantiated.
	They will have a state, temporary or permanent.
	Child Classes: Barrel, Gold, Water
******************************************/
bool Item::isTemp() {
	return tempState_;
}


/******************************************
	CLASS: Barrel (Oil)
	Once all Barrels are picked up from Iceman, the level ends.
******************************************/
void Barrel::doSomething()
{
	// determine if Iceman is close enough to see barrel
	if (isVisible() == false && calcDistance(*getWorld()->IcemanPtr_) <= 4) {
		setVisible(true);
		return;
	}

	// determine if Iceman is close enough to pick up barrel
	if (calcDistance(*getWorld()->IcemanPtr_) <= 3) {
		setActive(false);
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decBarrels();
	}
}


/******************************************
	CLASS: Gold
******************************************/
void Gold::doSomething()
{
	// determine if Gold is in temporary state
	if (isTemp() == true) {
		tempCount_--;
		if (tempCount_ <= 0)
			setActive(false);
	}

	// determine if Iceman is close enough to see Gold
	if (isVisible() == false && calcDistance(*getWorld()->IcemanPtr_) <= 4) {
		setVisible(true);
		return;
	}

	// determine if Iceman is close enough to pick up Gold (if not temporary)
	if (isTemp() == false && calcDistance(*getWorld()->IcemanPtr_) <= 3) {
		setActive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->IcemanPtr_->incGold();
	}
	// determine if Protestor is close enough to pick up Gold (if temporary)
	else if (isTemp() == true) {
		for (int i = 0; i < getWorld()->currentActors.size(); i++)
		{
			if (getWorld()->currentActors[i]->getID() == IID_PROTESTER
				|| getWorld()->currentActors[i]->getID() == IID_HARD_CORE_PROTESTER)
			{
				if (calcDistance(*getWorld()->currentActors[i]) <= 3)
				{
					setActive(false);
					getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
					
					Protestor* tempChar = static_cast<Protestor*>(getWorld()->currentActors[i]);
					if(tempChar->getID() == IID_PROTESTER)
					{ 
						tempChar->setLeaving(true);
						getWorld()->increaseScore(25);
					}
					else if (tempChar->getID() == IID_HARD_CORE_PROTESTER)
					{
						getWorld()->increaseScore(50);
					}

					break;
				}
			}
		}
	}
}
bool Character::isAnnoyed() {
	return annoyed_;
}
void Character::setAnnoyed(bool b) {
	annoyed_ = b;
}


/******************************************
	CLASS: Water
******************************************/
void Water::doSomething() {

	tempCount_--;
	if (tempCount_ <= 0)
		setActive(false);
	else if (calcDistance(*getWorld()->IcemanPtr_) <= 3) {
		setActive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->IcemanPtr_->incNumOfSquirts();
		getWorld()->increaseScore(100);
	}
}


/******************************************
	CLASS: Sonar
******************************************/
void Sonar::doSomething() {

	tempCount_--;
	if (tempCount_ <= 0)
		setActive(false);
	else if (calcDistance(*getWorld()->IcemanPtr_) <= 3) {
		setActive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->IcemanPtr_->incNumOfSonars();
		getWorld()->increaseScore(75);
	}

}

/******************************************
	CLASS: Character
	Abstract Base Class for all objects that can be annoyed()
	Cannot be instantiated.
	Child Classes: Iceman, Protestor
******************************************/
int Character::getHealth() {
	return health_;
}
int Character::getNumOfGold() {
	return numOfGold_;
}
bool Character::hasDied() {
	if (health_ <= 0)
		return true;
	else
		return false;
}
void Character::incGold() {
	numOfGold_++;
}
void Character::decGold() {
	numOfGold_--;
}

void Character::decreaseHealth(unsigned int h)
{
	health_ -= h;
}


/******************************************
	CLASS: Iceman
	User controlled player.
******************************************/
void Iceman::doSomething()
{
	/******************************************
		REMOVE ICE
	******************************************/
	if (checkForIce())
	{
		getWorld()->playSound(SOUND_DIG);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				delete getWorld()->iceField_[getX() + j][getY() + i];
				getWorld()->iceField_[getX() + j][getY() + i] = nullptr;
			}
		}
	}
	/******************************************
		PLAYER INPUT
	******************************************/
	int k;
	if (getWorld()->getKey(k) == true)
	{
		// user pressed a key this tick
		switch (k)
		{
			/******************************************
				MOVE LEFT
			******************************************/
			case KEY_PRESS_LEFT:
				// change direction
				if (getDirection() != left)	
				{
					setDirection(left);
				}
				// move
				else
				{
					// if space is in bounds and no boulder
					if (getX() > 0 && checkForBoulders(k))
					{
						moveTo(getX() - 1, getY());
					}
				}
				break;

			/******************************************
				MOVE RIGHT
			******************************************/
			case KEY_PRESS_RIGHT:
				// change direction
				if (getDirection() != right)
				{
					setDirection(right);
				}
				// move
				else
				{
					// if space is in bounds and no boulder
					if (getX() < 60 && checkForBoulders(k))
					{
						moveTo(getX() + 1, getY());
					}
				}
				break;
		
			/******************************************
				MOVE DOWN
			******************************************/
			case KEY_PRESS_DOWN:
				// change direction
				if (getDirection() != down)
				{
					setDirection(down);
				}
				// move
				else
				{
					// if space is in bounds and no boulder
					if (getY() > 0 && checkForBoulders(k))
					{
						moveTo(getX(), getY() - 1);
					}
				}
				break;

			/******************************************
				MOVE UP
			******************************************/
			case KEY_PRESS_UP:
				// change direction
				if (getDirection() != up)
				{
					setDirection(up);
				}
				// move
				else
				{
					// if space is in bounds and no boulder
					if (getY() < 60 && checkForBoulders(k))
					{
						moveTo(getX(), getY() + 1);
					}
				}
				break;

			/******************************************
				DIE
			******************************************/
			case KEY_PRESS_ESCAPE:
				getWorld()->IcemanPtr_->setActive(false);
				break;
		
			/******************************************
				DROP GOLD
			******************************************/
			case KEY_PRESS_TAB:	
				if (getWorld()->IcemanPtr_->getNumOfGold() > 0) {
					Gold* temp = new Gold(getWorld(), getX(), getY(), true);
					getWorld()->currentActors.push_back(temp);
					getWorld()->IcemanPtr_->decGold();
				}
				break;

			/******************************************
				USE SONAR
			******************************************/
			case 'z':
			case 'Z':
				if (getWorld()->IcemanPtr_->getNumOfSonars() > 0) {
					// loop through current Actors
					for (int i = 0; i < getWorld()->currentActors.size(); i++) {
						// if actor is hidden and within radius of 12
						if (getWorld()->currentActors[i]->isVisible() == false &&
							calcDistance(*getWorld()->currentActors[i]) <= 12)
						{
							getWorld()->currentActors[i]->GraphObject::setVisible(true);
						}
					}
					getWorld()->IcemanPtr_->decNumOfSonars();
				}
				break;

			/******************************************
				USE SQUIRT
			******************************************/
			case KEY_PRESS_SPACE:

				// if there are squirts in inventory
				if (getWorld()->IcemanPtr_->getNumOfSquirts() > 0) {

					// UP
					// if space is in bounds AND in same direction AND no boulder in direction
					if (getY() < 60 && getWorld()->IcemanPtr_->getDirection() == up && checkSquirtLocation(up))
					{
						Squirt* temp = new Squirt(getWorld(), getWorld()->IcemanPtr_->getX(), getWorld()->IcemanPtr_->getY() + 4, getWorld()->IcemanPtr_->getDirection());
						getWorld()->currentActors.push_back(temp);
						getWorld()->IcemanPtr_->decNumOfSquirts();
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					}

					// DOWN
					// if space is in bounds AND in same direction AND no boulder in direction
					else if (getY() > 0 && getWorld()->IcemanPtr_->getDirection() == down && checkSquirtLocation(down))
					{
						Squirt* temp = new Squirt(getWorld(), getWorld()->IcemanPtr_->getX(), getWorld()->IcemanPtr_->getY() - 4, getWorld()->IcemanPtr_->getDirection());
						getWorld()->currentActors.push_back(temp);
						getWorld()->IcemanPtr_->decNumOfSquirts();
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					}

					// LEFT
					// if space is in bounds AND in same direction AND no boulder in direction
					else if (getX() > 0 && getWorld()->IcemanPtr_->getDirection() == left && checkSquirtLocation(left))
					{
						Squirt* temp = new Squirt(getWorld(), getWorld()->IcemanPtr_->getX() - 4, getWorld()->IcemanPtr_->getY(), getWorld()->IcemanPtr_->getDirection());
						getWorld()->currentActors.push_back(temp);
						getWorld()->IcemanPtr_->decNumOfSquirts();
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					}

					// RIGHT
					// if space is in bounds AND in same direction AND no boulder in direction
					else if (getX() < 60 && getWorld()->IcemanPtr_->getDirection() == right && checkSquirtLocation(right))
					{
						Squirt* temp = new Squirt(getWorld(), getWorld()->IcemanPtr_->getX() + 4, getWorld()->IcemanPtr_->getY(), getWorld()->IcemanPtr_->getDirection());
						getWorld()->currentActors.push_back(temp);
						getWorld()->IcemanPtr_->decNumOfSquirts();
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					}

					// otherwise, there is a boulder in front
					else {
						getWorld()->IcemanPtr_->decNumOfSquirts();
						getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					}
				}
				break;

			/******************************************
				TESTING: used to increment Gold
			******************************************/
			case 61:// PLUS
				getWorld()->IcemanPtr_->incGold();
				break;

			/******************************************
				TESTING: toggles visibility of actors
			******************************************/
			case 8:// BACKSPACE
				// loop through active Actors
				for (int i = 1; i < getWorld()->currentActors.size(); i++) {
					// toggle visibility of Actors
					if (getWorld()->currentActors[i]->getID() != IID_SONAR &&
						getWorld()->currentActors[i]->getID() != IID_WATER_POOL)
					{
						if (getWorld()->currentActors[i]->isVisible() == false) {
							getWorld()->currentActors[i]->GraphObject::setVisible(true);
						}
						else {
							if (getWorld()->currentActors[i]->getID() != IID_BOULDER)
								getWorld()->currentActors[i]->GraphObject::setVisible(false);
						}
					}
				}
				break;

			/******************************************
				TESTING: used to populate Water
			******************************************/
			case '-':
				getWorld()->populateWater(getWorld()->getLevel());
				break;

			/******************************************
				TESTING: stops program
			******************************************/
			case 127:// DEL
				exit(0);
				break;
		}
	}

	if (hasDied())
		setActive(false);
}
void Iceman::annoyed(int fromID) {
	if (fromID == IID_BOULDER)
		health_ -= 100;
}
int Iceman::getNumOfSquirts() {
	return numOfSquirts_;
}
void Iceman::incNumOfSquirts() {
	numOfSquirts_ += 5;
}
void Iceman::decNumOfSquirts() {
	numOfSquirts_--;
}
int Iceman::getNumOfSonars() {
	return numOfSonars_;
}
void Iceman::incNumOfSonars() {
	numOfSonars_++;
}
void Iceman::decNumOfSonars() {
	numOfSonars_--;
}
bool Iceman::checkSquirtLocation(int direction)
{
	double temp;

	// loop through current actors vector
	for (int i = 0; i < getWorld()->currentActors.size(); i++) {

		// check only boulders
		if (getWorld()->currentActors[i]->getID() == IID_BOULDER) {

			// check distance Actor is from each boulder
			temp = calcDistance(*getWorld()->currentActors[i]);

			// if boulder is within radius
			if (temp <= 3.5) {
				// check for going up
				if (direction == up && getWorld()->currentActors[i]->getY() > this->getY())
					return false;	// it's not okay

				// check for going down
				if (direction == down && getWorld()->currentActors[i]->getY() < this->getY())
					return false;	// it's not okay

				// check for going left
				if (direction == left && getWorld()->currentActors[i]->getX() < this->getX())
					return false;	// it's not okay

				// check for going right
				if (direction == right && getWorld()->currentActors[i]->getX() > this->getX())
					return false;	// it's not okay

				// otherwise, boulder is in not direction of movement
				return true;		// it's okay
			}
		}
	}
	return true;	// Actor is not near boulder
}

/******************************************
	CLASS: Protestors

******************************************/
int Protestor::updateMobilityCount()
{
	//calculate number of movements from 8 <= x <= 60
	return getWorld()->genRandNumber(8, 60);
}

bool Protestor::checkForIcemanInRadius()
{
	if (calcDistance(*getWorld()->IcemanPtr_) <= 4)
		return true;
	else
		return false;
}

bool Protestor::hasLineOfSight()
{
	// get x,y coordinates of both iceman and protestors
	int xP = this->getX();
	int yP = this->getY();
	int xI = getWorld()->IcemanPtr_->getX();
	int yI = getWorld()->IcemanPtr_->getY();
	double deltaX = xI - xP;

	// Iceman is on same horizontal line as Protestor
	if (yP == yI)
	{
		// Iceman is to the right of Protestor
		if (xP < xI)
		{
			// Checking distance between
			for (int x = xP; x <= xI; x++)
			{
				for (int y = yP; y < (yP + 4); y++)
				{
					// If has Ice anywhere inbetween
					if (getWorld()->iceField_[x][y] != nullptr)
					{
						// Does not have line of sight, return false
						return false;
					}

				}
			}
			// otherwise, no ice, return true
			return true;
		}
		// Iceman is to the left of Protestor
		else if (xP > xI)
		{
			// Checking distance between
			for (int x = xI; x <= xP; x++)
			{
				for (int y = yI; y < (yI + 4); y++)
				{
					// If has Ice anywhere inbetween
					if (getWorld()->iceField_[x][y] != nullptr)
					{
						//Has ice return false
						//Does not have line of sight
						return false;
					}
				}
			}
			// otherwise, has line of sight, return true
			return true;
		}
	}
	// Iceman is on the same vertical line as Protestor
	else if(xP == xI)
	{
		// If Protestor is above Iceman
		if (yP > yI)
		{
			// Check distance inbetween
			for (int x = xI; x < (xI + 4); x++)
			{
				for (int y = yI; y <= yP; y++)
				{
					//If has Ice
					if (getWorld()->iceField_[x][y] != nullptr)
					{
						//Has ice return false
						//Does not have line of sight
						return false;
					}
				}
			}
			return true;
		}
		// If Protestor is below Iceman
		else if (yP < yI)
		{
			// Check distance inbetween
			for (int x = xP; x < (xP + 4); x++)
			{
				for (int y = yP; y <= yI; y++)
				{
					// If has Ice
					if (getWorld()->iceField_[x][y] != nullptr)
					{
						//Has ice return false
						//Does not have line of sight
						return false;
					}
				}
			}
			return true;
		}
	}
}

void Protestor::testLineOfSight()
{
	if (hasLineOfSight() == true)
		cout << "I HAVE LINE OF SIGHT!" << endl;
}

void Protestor::genNewDirection()
{
	int tempUP = 1;
	int tempDOWN = 2;
	int tempLEFT = 3;
	int tempRIGHT = 4;

	int oldDirection;
	Direction tempDirection = getDirection();
	switch (tempDirection)
	{
		case up:
		{
			oldDirection = tempUP;
			break;
		}
		case down:
		{
			oldDirection = tempDOWN;
			break;
		}
		case left:
		{
			oldDirection = tempLEFT;
			break;
		}
		case right:
		{
			oldDirection = tempRIGHT;
			break;
		}

	}
	
	int newDirection = getWorld()->genRandNumber(1, 4);
	while (newDirection == oldDirection) {
		newDirection = getWorld()->genRandNumber(1, 4);
	}
	

	switch (newDirection)
	{
		case 1:
		{
			setDirection(up);
			break;
		}
		case 2:
		{
			setDirection(down);
			break;
		}
		case 3:
		{
			setDirection(left);
			break;
		}
		case 4:
		{
			setDirection(right);
			break;
		}
	}

}

bool Protestor::isResting()
{
	return resting_;
}

void Protestor::setResting(bool b)
{
	resting_ = b;
}

void Protestor::resetRestingCount(int level)
{
	restingCount_ = max(0, 3 - level / 4);
}

bool Protestor::isWaitingToYell()
{
	return waitingToYell_;
}

void Protestor::setWaitingToYell(bool b)
{
	waitingToYell_ = b;
}

void Protestor::resetWaitingToYellCount()
{
	yellingCount_ = 15;
}

bool Protestor::isLeaving()
{
	return leaving_;
}

void Protestor::setLeaving(bool b)
{
	leaving_ = b;
}

void Protestor::isStunned()
{
	int l = getWorld()->getLevel();
	restingCount_ = max(50, 100-l*10);
}

bool Protestor::isFacingIceman()
{
	int proDir = this->getDirection();
	int iceX = getWorld()->IcemanPtr_->getX();
	int iceY = getWorld()->IcemanPtr_->getY();
	int proX = this->getX();
	int proY = this->getY();

	if (iceY > proY && proDir == up) //If Iceman is above
	{
		return true;
	}
	else if (iceY < proY && proDir == down)//If Iceman is below
	{
		return true;
	}
	else if (iceX < proX && proDir == left)//If ICeman is to the left
	{
		return true;
	}
	else if (iceX > proX && proDir == right)//If Iceman is to the right
	{
		return true;
	}

	//If none is true return false
	return false;
}

void Protestor::faceIcemanAndMove()
{
	// Iceman is above and in line of sight
	if (getWorld()->IcemanPtr_->getY() > this->getY()
		&& getWorld()->IcemanPtr_->getX() == this->getX())
	{
		this->setDirection(up);
		moveTo(getX(), getY() + 1);
	}
	// Iceman is below and in line of sight
	else if (getWorld()->IcemanPtr_->getY() < this->getY()  
		&& getWorld()->IcemanPtr_->getX() == this->getX())
	{
		this->setDirection(down);
		moveTo(getX(), getY() - 1);
	}
	// Iceman is to the left and in line of sight
	else if (getWorld()->IcemanPtr_->getX() < this->getX()
		&& getWorld()->IcemanPtr_->getY() == this->getY())
	{
		this->setDirection(left);
		moveTo(getX() - 1, getY());
	}
	// Iceman is to the right and in line of sight
	else if (getWorld()->IcemanPtr_->getX() > this->getX()
		&& getWorld()->IcemanPtr_->getY() == this->getY())
	{
		this->setDirection(right);
		moveTo(getX() + 1, getY());
	}
}

void RegularProtestor::doSomething()
{
	//// TEST
	//cout << "Health: " << health_ << endl;

	// Check protestors health
	if (health_ <= 0 && isLeaving() == false)
	{
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setLeaving(true);
		setResting(false);
		resetRestingCount(level_);
	}
	// Protestor is waiting to doSomething
	if (isResting())
	{
		restingCount_--;
		if (restingCount_ <= 0)
		{
			setResting(false);
			resetRestingCount(level_);
		}
		return;
		//cout << "RESTING" << endl;
	}
	// Protestor is leaving oil field
	if (isLeaving() == true)
	{
		// TEST: until shortest path is determined
		setActive(false);
		getWorld()->numberOfProtestors_--;
		return;

		if (calcDistance(60, 60) <= 1) // Protestor is at the exit
			setActive(false);
		else
		{
			// find shortest path
			// move towards it
		}

		//cout << "LEAVING" << endl;
	}
	// Protestor is trying to shout
	else if (checkForIcemanInRadius() == true && isFacingIceman())
	{
		if (isWaitingToYell())	// if I recently shouted
		{
			yellingCount_--;	// wait
			if (yellingCount_ <= 0)
				setWaitingToYell(false);	// dont wait anymore
		}
		else	// shout at Iceman
		{
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			getWorld()->takeDamage(*this, *getWorld()->IcemanPtr_);
			setWaitingToYell(true);
			resetWaitingToYellCount();
		}

		//cout << "YELLING" << endl;
	}
	// Protestor walks towards Iceman he has line of sight
	else if (checkForIcemanInRadius() == false && hasLineOfSight() == true)	// > 4 away && hasLineOfSight
	{
		// face and move towards Iceman one space
		faceIcemanAndMove();
		resetRestingCount(level_);
		setResting(true);

		/************************************************
			set numSquaresToMoveInCurrentDirection = 0
				unless Protestor still hasLineOfSight()
				he will still move towards Iceman
		************************************************/
		numSquaresToMoveInCurrentDirection_ = 0;
		//cout << "FACING ICEMAN" << endl;
	}
	// Protestor needs new direction
	else
	{	
		numSquaresToMoveInCurrentDirection_--;
		if (numSquaresToMoveInCurrentDirection_ <= 0)
		{
			genNewDirection();
			numSquaresToMoveInCurrentDirection_ = updateMobilityCount();
			return;
		}

		// check to move if protestor is facing up
		if (getDirection() == up && getWorld()->hasIce(getX(), getY() + 1) == false
			&& getY() < 60)
		{
			moveTo(getX(), getY() + 1);
			resetRestingCount(level_);
			setResting(true);
		}
		// check to move if protestor is facing down
		else if (getDirection() == down && getWorld()->hasIce(getX(), getY() - 1) == false
			&& getY() > 0)
		{
			moveTo(getX(), getY() - 1);
			resetRestingCount(level_);
			setResting(true);
		}
		// check to move if protestor is facing left
		else if (getDirection() == left && getWorld()->hasIce(getX() - 1, getY()) == false
			&& getX() > 0)
		{
			moveTo(getX() - 1, getY());
			resetRestingCount(level_);
			setResting(true);
		}
		// check to move if protestor is facing right
		else if (getDirection() == left && getWorld()->hasIce(getX() + 1, getY()) == false
			&& getX() < 60)
		{
			moveTo(getX() + 1, getY());
			resetRestingCount(level_);
			setResting(true);
		}
		else
		{
			genNewDirection();
		}
		//cout << "MOVE RANDOMLY" << endl;
	}
	/**********************************
		PLACE AFTER MOVE FUNCTION
		resetRestingCount(level_);
	**********************************/
	

	//If Protestor is sitting at an intersection and 
	//made a perpendicular turn in 200 non-resting ticks
	//then pick viable route is multiple viable routes exist pick one direction randomly
	//new numSquaresToMoveInCurrentDirection values
}

void HardcoreProtestor::doSomething()
{
	//// TEST
//cout << "Health: " << health_ << endl;

// Check protestors health
	if (health_ <= 0 && isLeaving() == false)
	{
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setLeaving(true);
		setResting(false);
		resetRestingCount(level_);
	}
	// Protestor is waiting to doSomething
	if (isResting() && isStaringAtGold() == false)
	{
		restingCount_--;
		if (restingCount_ <= 0)
		{
			setResting(false);
			resetRestingCount(level_);
		}
		return;
		//cout << "RESTING" << endl;
	}
	// Protestor is staring at gold
	if (isStaringAtGold() && isLeaving() == false)
	{
		ticksToStare_--;
		if (ticksToStare_ <= 0)
		{
			setStaringAtGold(false);
			resetTicksToStare(level_);
		}
	}
	// Protestor is leaving oil field
	if (isLeaving() == true)
	{
		// TEST: until shortest path is determined
		setActive(false);
		getWorld()->numberOfProtestors_--;
		return;

		if (calcDistance(60, 60) <= 1) // Protestor is at the exit
			setActive(false);
		else
		{
			// find shortest path
			// move towards it
		}

		//cout << "LEAVING" << endl;
	}
	// Protestor is trying to shout
	else if (checkForIcemanInRadius() == true && isFacingIceman())
	{
		if (isWaitingToYell())	// if I recently shouted
		{
			yellingCount_--;	// wait
			if (yellingCount_ <= 0)
				setWaitingToYell(false);	// dont wait anymore
		}
		else	// shout at Iceman
		{
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			getWorld()->takeDamage(*this, *getWorld()->IcemanPtr_);
			setWaitingToYell(true);
			resetWaitingToYellCount();
		}

		//cout << "YELLING" << endl;
	}
	// Protestor walks towards Iceman he has line of sight
	else if (checkForIcemanInRadius() == false && hasLineOfSight() == true)	// > 4 away && hasLineOfSight
	{
		// face and move towards Iceman one space
		faceIcemanAndMove();
		resetRestingCount(level_);
		setResting(true);

		/************************************************
			set numSquaresToMoveInCurrentDirection = 0
				unless Protestor still hasLineOfSight()
				he will still move towards Iceman
		************************************************/
		numSquaresToMoveInCurrentDirection_ = 0;
		//cout << "FACING ICEMAN" << endl;
	}
	// Protestor needs new direction
	else
	{
		numSquaresToMoveInCurrentDirection_--;
		if (numSquaresToMoveInCurrentDirection_ <= 0)
		{
			genNewDirection();
			numSquaresToMoveInCurrentDirection_ = updateMobilityCount();
			return;
		}

		// check to move if protestor is facing up
		if (getDirection() == up && getWorld()->hasIce(getX(), getY() + 1) == false
			&& getY() < 60)
		{
			moveTo(getX(), getY() + 1);
			resetRestingCount(level_);
			setResting(true);
		}
		// check to move if protestor is facing down
		else if (getDirection() == down && getWorld()->hasIce(getX(), getY() - 1) == false
			&& getY() > 0)
		{
			moveTo(getX(), getY() - 1);
			resetRestingCount(level_);
			setResting(true);
		}
		// check to move if protestor is facing left
		else if (getDirection() == left && getWorld()->hasIce(getX() - 1, getY()) == false
			&& getX() > 0)
		{
			moveTo(getX() - 1, getY());
			resetRestingCount(level_);
			setResting(true);
		}
		// check to move if protestor is facing right
		else if (getDirection() == left && getWorld()->hasIce(getX() + 1, getY()) == false
			&& getX() < 60)
		{
			moveTo(getX() + 1, getY());
			resetRestingCount(level_);
			setResting(true);
		}
		else
		{
			genNewDirection();
		}
		//cout << "MOVE RANDOMLY" << endl;
	}
	/**********************************
		PLACE AFTER MOVE FUNCTION
		resetRestingCount(level_);
	**********************************/


	//If Protestor is sitting at an intersection and 
	//made a perpendicular turn in 200 non-resting ticks
	//then pick viable route is multiple viable routes exist pick one direction randomly
	//new numSquaresToMoveInCurrentDirection values
}

bool HardcoreProtestor::isStaringAtGold()
{
	return staringAtGold_;
}

void HardcoreProtestor::setStaringAtGold(bool b)
{
	staringAtGold_ = b;
}

void HardcoreProtestor::resetTicksToStare(int level)
{
	ticksToStare_ = max(50, 100 - level * 10);
}
