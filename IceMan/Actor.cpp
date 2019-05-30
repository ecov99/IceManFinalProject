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
		if (checkCollision(tempID) == true) {	// if collided with active Actor
			
			if (tempID == IID_PLAYER)		// if collided with ICEMAN
				getWorld()->IcemanPtr_->annoyed(this->getID());
				
			setCollided(true);
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
bool Boulder::checkCollision(int &victimID) {
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
				return true;
			}
		}
	}
	return false;
}


/******************************************
	CLASS: Squirt
******************************************/
void Squirt::doSomething() {

	// MOVE UP if no boulder
	if (this->getDirection() == up && checkForBoulders(this->getDirection()) && checkForIce() == false) {
		moveTo(getX(), getY() + 1);
		incMovementCount();
	}

	// MOVE DOWN if no boulder
	else if (this->getDirection() == down && checkForBoulders(this->getDirection()) && checkForIce() == false) {
		moveTo(getX(), getY() - 1);
		incMovementCount();
	}

	// MOVE LEFT if no boulder
	else if (this->getDirection() == left && checkForBoulders(this->getDirection()) && checkForIce() == false) {
		moveTo(getX() - 1, getY());
		incMovementCount();
	}

	// MOVE RIGHT if no boulder
	else if (this->getDirection() == right && checkForBoulders(this->getDirection()) && checkForIce() == false) {
		moveTo(getX() + 1, getY());
		incMovementCount();
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
			TESTING: used to increment Sonar
		******************************************/
		case 61:// PLUS
			getWorld()->IcemanPtr_->incNumOfSonars();
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

bool Protestor::hasLineOfSight()
{

	// get x,y coordinates of both iceman and protestors
	int xP = this->getX();
	int yP = this->getY();
	int xI = getWorld()->IcemanPtr_->getX();
	int yI = getWorld()->IcemanPtr_->getY();
	double deltaX = xI - xP;

	if (yP == yI)
	{
		if (xP < xI)
		{
			for (int x = xP; x <= xI; x++)
			{
				for (int y = yP; y < (yP + 4); y++)
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
		else if (xP > xI)
		{
			for (int x = xI; x <= xP; x++)
			{
				for (int y = yI; y < (yI + 4); y++)
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
	}
	else if(xP == xI)
	{
		if (yP > yI)
		{
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
		else if (yP < yI)
		{
			for (int x = xP; x < (xP + 4); x++)
			{
				for (int y = yP; y <= yI; y++)
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
	}
	//// calculate slope of line for each quadrant
	//if (deltaX != 0) {
	//	double m = (yI - yP) / (xI - xP);
	//	if(m != 0)
	//	{ 
	//		// check every point along line for ice
	//		// QUAD I
	//		if (m > 0 && xP < xI) {
	//			for (int x = 0; x <= deltaX; x++) {
	//				int temp = m * x + yP;
	//				if (getWorld()->iceField_[xP+x][temp] != nullptr)
	//					return false;
	//			}
	//			return true;
	//		}
	//		// QUAD II
	//		else if (m < 0 && xP > xI && deltaX < 0) {
	//			for (int x = 0; x <= deltaX; x--) {
	//				int temp = m * x + yP;
	//				if (getWorld()->iceField_[xI + x][temp] != nullptr)
	//					return false;
	//			}
	//			return true;
	//		}
	//		// QUAD III
	//		else if (m > 0 && xP > xI && deltaX < 0) {
	//			for (int x = 0; x <= deltaX; x--) {
	//				int temp = m * x + yI;
	//				if (getWorld()->iceField_[xI + x][temp] != nullptr)
	//					return false;
	//			}
	//			return true;
	//		}
	//		// QUAD IV
	//		else if (m < 0 && xP < xI && deltaX > 0) {
	//			for (int x = 0; x < deltaX; x++) {
	//				int temp = m * x + yI;
	//				if (getWorld()->iceField_[xP + x][temp] != nullptr)
	//					return false;
	//			}
	//			return true;
	//		}
	//	}
	//	else // horizontal lines
	//	{
	//		// iceman is on the right
	//		if (xI > xP) {
	//			for (int x = xP; x <= xI; x++) {
	//				if (getWorld()->iceField_[xP + x][yP] != nullptr)
	//					return false;
	//			}
	//			return false;
	//		}
	//		// iceman is on the left
	//		else if (xI < xP) {
	//			for (int x = xI; x <= xP; x++) {
	//				if (getWorld()->iceField_[xP + x][yP] != nullptr)
	//					return false;
	//			}
	//			return true;
	//		}
	//	}
	//}
	//else // vertical lines
	//{
	//	// iceman is above
	//	if (yP < yI) {
	//		for (int y = 0; y <= (yI - yP); y++) {
	//			if (getWorld()->iceField_[xP][yP + y] != nullptr)
	//				return false;
	//		}
	//		return true;
	//	}	
	//	// iceman is below
	//	else if (yP > yI) {
	//		for (int y = 0; y <= (yP - yI); y++) {
	//			if (getWorld()->iceField_[xP][yI + y] != nullptr)
	//				return false;
	//		}
	//		return true;
	//	}
	//	
	//}
}

void Protestor::genNewDirection()
{
	int newDirection = getWorld()->genRandNumber(1, 4);
	int oldDirection = getDirection();

	switch (newDirection)
	{
		case 1:
		{
			if (oldDirection != newDirection)
			{
				setDirection(left);
			}
			else
			{
				setDirection(right);
			}
			break;
		}
		case 2:
		{
			if (oldDirection != newDirection)
			{
				setDirection(right);
			}
			else
			{
				setDirection(left);
			}
			break;
		}
		case 3:
		{
			if (oldDirection != newDirection)
			{
				setDirection(up);
			}
			else
			{
				setDirection(down);
			}
			break;
		}
		case 4:
		{
			if (oldDirection != newDirection)
			{
				setDirection(down);
			}
			else
			{
				setDirection(up);
			}
			break;
		}
	}

}

void Protestor::testLineOfSight()
{
	if (hasLineOfSight() == true)
		cout << "I HAVE LINE OF SIGHT!" << endl;
}

bool Protestor::isFacing()
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

void RegularProtestor::doSomething()
{
	ticksToWaitBetweenMoves_ = 0; //TESTING PURPOSES REMOVE AFTER TESTING

	if (ticksToWaitBetweenMoves_ > 0)			// Protestor can not move
		ticksToWaitBetweenMoves_--;
	else if (ticksToWaitBetweenMoves_ == 0)		// Protestor can move
	{
		//*********************************************************
		//	TEST BEGIN
		//*********************************************************/
		//int t;
		//if (getWorld()->getKey(t) == true)
		//{
		//	// user pressed a key this tick
		//	switch (t)
		//	{
		//	/******************************************
		//		MOVE LEFT
		//	******************************************/
		//	case 'j':
		//	case 'J':
		//		// change direction
		//		if (getDirection() != left)
		//		{
		//			setDirection(left);
		//		}
		//		// move
		//		else
		//		{
		//			// if space is in bounds and no boulder
		//			if (getX() > 0 && checkForBoulders(t))
		//			{
		//				moveTo(getX() - 1, getY());
		//			}
		//		}
		//		break;
		//	/******************************************
		//		MOVE RIGHT
		//	******************************************/
		//	case 'l':
		//	case 'L':
		//		// change direction
		//		if (getDirection() != right)
		//		{
		//			setDirection(right);
		//		}
		//		// move
		//		else
		//		{
		//			// if space is in bounds and no boulder
		//			if (getX() < 60 && checkForBoulders(t))
		//			{
		//				moveTo(getX() + 1, getY());
		//			}
		//		}
		//		break;
		//	/******************************************
		//		MOVE DOWN
		//	******************************************/
		//	case 'k':
		//	case 'K':
		//		// change direction
		//		if (getDirection() != down)
		//		{
		//			setDirection(down);
		//		}
		//		// move
		//		else
		//		{
		//			// if space is in bounds and no boulder
		//			if (getY() > 0 && checkForBoulders(t))
		//			{
		//				moveTo(getX(), getY() - 1);
		//			}
		//		}
		//		break;
		//	/******************************************
		//		MOVE UP
		//	******************************************/
		//	case 'i':
		//	case 'I':
		//		// change direction
		//		if (getDirection() != up)
		//		{
		//			setDirection(up);
		//		}
		//		// move
		//		else
		//		{
		//			// if space is in bounds and no boulder
		//			if (getY() < 60 && checkForBoulders(t))
		//			{
		//				moveTo(getX(), getY() + 1);
		//			}
		//		}
		//		break;
		//	}
		//}
		//*********************************************************
		//	TEST END
		//*********************************************************/

		//To ensure protestor only speaks once
		if (getHealth() <= 0 && (killedByBoulder == false || killedByIceman == false))
		{
			leaveOilFieldState_ = true;
			getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		}
		if (leaveOilFieldState_ == true)// wants to leave the oilField by beeing annoyed
		{
			if (calcDistance(60, 60) <= 1)//If Protestor reaches exit
			{
				setActive(false);
			}
			//Cannot be quirted or bonked

			//Play sound SOUND_PROTESTOR_GIVE_UP
			//resting tick set to zero so it never rests

			//If bonked by boulder increaseScore(500)
			//If annoyed by squirted increaseScore(100)

			//Find shortest path to exit

			//Moves towards exit

			return;
		}
		//If Protestor is not trying to leave the oilField
		yellingCounter--;
		//If protester is within radius of 4 and facing Iceman 
		//and has not shouted within the last 15 nonresting tick
		if (calcDistance(*getWorld()->IcemanPtr_) <= 4 && isFacing() && yellingCounter <= 0)
		{
			//Shout at Iceman
			getWorld()->playSound(SOUND_PROTESTER_YELL);

			//Annoy Iceman by deducting 2 health points
			//getWorld()->IcemanPtr_->decreaseHealth(2);
		

			//Reset shouting variable
			yellingCounter = 15;
		}
		//else if protestor is within vertical or horizontal line of sight of Iceman
		//and is more than 4 radius away
		else if (hasLineOfSight() && calcDistance(getWorld()->IcemanPtr_->getX(), getWorld()->IcemanPtr_->getY()) >= 4)
		{
			////Change direction facing Iceman and move one step towards Iceman
			//if (getWorld()->IcemanPtr_->getDirection() != getDirection())
			//{
			//	setDirection(getWorld()->IcemanPtr_->getDirection());
			//}
			//set numSquaresToMoveinCurrentDirecton to zero, forcing to pick a new direction during its
			//next non-resting tick, unless Protestor still sees Iceman then will continue to move towards Iceman
			numSquaresToMoveInCurrentDirection_ = 0;
			//then return
			return;
		}

		//If the Protestor can not see Iceman then
		else if (hasLineOfSight() == false)
		{
			numSquaresToMoveInCurrentDirection_--;
			if (numSquaresToMoveInCurrentDirection_ <= 0)
			{
				//Protestor will pick a random direction
			}
		}
		//If Protestor can move in that direction, move numSquaresToMoveInCurrentDirection
		//else choose new direction


		//If Protestor is sitting at an intersection and 
		//made a perpendicular turn in 200 non-resting ticks
		//then pick viable route is multiple viable routes exist pick one direction randomly
		//new numSquaresToMoveInCurrentDirection values





		//At the end of each movement reset ticksToWaitBetweenMoves
		ticksToWaitBetweenMoves_ = updateMobilityCount();
	}
}

void HardcoreProtestor::doSomething()
{
	
}