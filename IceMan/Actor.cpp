/*
	Actor.cpp will define all functions, except for ctors/dtors.
	Has all logic and implementation.
*/
#include "Actor.h"
#include "StudentWorld.h"

/*
	CLASS: Actor
	Abstract Base Class for all other classes.
	Cannot be instantiated.
	Child Classes: Ice, Boulder, Squirt, Item, Character
*/
bool Actor::isActive() {
	return active_;
}
void Actor::setActive(bool b) {
	active_ = b;
}
StudentWorld* Actor::getWorld() {
	return sw_;
}
double Actor::calcDistance(Actor &other)
{
	double Ax = this->getX();
	double Ay = this->getY();
	double Bx = other.getX();
	double By = other.getY();
	return sqrt((pow(Ax - Bx, 2) + pow(Ay - By, 2)));
}

double Actor::calcDistance(int x, int y)
{
	double Ax = this->getX();
	double Ay = this->getY();
	double Bx = x;
	double By = y;
	return sqrt((pow(Ax - Bx, 2) + pow(Ay - By, 2)));
}

//Coordinate Actor::getLocation()
//{
//	return Location_;
//}

/*
	CLASS: Boulder
	Starts off stable. Falls once ice below is mined by Iceman.
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
				// play sound for boulder falling
				getWorld()->playSound(SOUND_FALLING_ROCK);
			}
		}

	}

	// boulder is falling
	if (isStable() == false && isFalling() == true && hasCollided() == false) {

		if (getWorld()->iceField_[getX()][getY() - 1] == nullptr && getY() > 1)
		{
			moveTo(getX(), getY() - 1);
		}

		//Change collision if neccessary
		if (getWorld()->iceField_[getX()][getY() - 1] != nullptr || getY() == 1)
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
	CLASS: Item
	Abstract Base Class for all items that can be picked up.
	Cannot be instantiated.
	They will have a state, temporary or permanent.
	Child Classes: Barrel, Gold, Water
*/
bool Item::IcemanCanPickUp() {
	return IcemanCanPickUp_;
}
bool Item::ProtestorCanPickUp() {
	return ProtestorCanPickUp_;
}

/*
	CLASS: Barrel (Oil)
	Once all Barrels are picked up from Iceman, the level ends.
*/
void Barrel::doSomething()
{
	//Calculate distance from Iceman
	int ix = getWorld()->IcemanPtr_->getX();
	int iy = getWorld()->IcemanPtr_->getY();
	int bx = getX();
	int by = getY();
	int x = abs(ix - bx);
	int y = abs(iy - by);
	double radius = sqrt(x * x + y * y);
	if (isVisible() == false && radius <= 4)
	{
		setVisible(true);
		return;
	}
	//Player picks up Oil and reduces barrelsRemaining_
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (getWorld()->IcemanPtr_->getX() == getX() + i)
			{
				if (getWorld()->IcemanPtr_->getY() == getY() + j)
				{
					getWorld()->increaseScore(1000);
					getWorld()->playSound(SOUND_FOUND_OIL);
					getWorld()->decBarrels();
					setActive(false);
				}
			}
		}
	}

}

/*
	CLASS: Gold
*/
void Gold::doSomething()
{
	//Player picks up gold and reduces goldRemaining_
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (getWorld()->IcemanPtr_->getX() == getX() + i)
			{
				if (getWorld()->IcemanPtr_->getY() == getY() + j)
				{
					getWorld()->increaseScore(10);
					getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
					getWorld()->IcemanPtr_->incGold();
					setActive(false);
				}
			}
		}
	}
}
bool Gold::isTemp() {
	return tempState_;
}
int Gold::getTempCount() {
	return tempCount_;
}

/*
	CLASS: Character
	Abstract Base Class for all objects that can be annoyed()
	Cannot be instantiated.
	Child Classes: Iceman, Protestor
*/
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
			if (getWorld()->iceField_[getX() + g][getY() + h] != nullptr)
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
				delete getWorld()->iceField_[getX() + j][getY() + i];
				getWorld()->iceField_[getX() + j][getY() + i] = nullptr;
			}
		}
	}
	// player input 
	int k;
	if (getWorld()->getKey(k) == true)
	{
		//User hit a key this tick
		switch (k)
		{
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

		case KEY_PRESS_ESCAPE:
			// stop program
			exit(0);
			break;
		}
	}
}
bool Iceman::checkForBoulders(int k) {
	double temp;

	// loop through current actors vector
	for (int i = 0; i < getWorld()->currentActors.size(); i++) {

		// check only boulders
		if (getWorld()->currentActors[i]->getID() == IID_BOULDER) {

			// check distance Iceman is from each boulder
			temp = calcDistance(*getWorld()->currentActors[i]);

			// if boulder is within radius of 3.5
			if (temp <= 3.5) {
				// check for going up, if boulder is down
				if (k == KEY_PRESS_UP && getWorld()->currentActors[i]->getY() < getWorld()->IcemanPtr_->getY())
					return true;	// it's okay

				// check for going down, if boulder is up
				if (k == KEY_PRESS_DOWN && getWorld()->currentActors[i]->getY() > getWorld()->IcemanPtr_->getY())
					return true;	// it's okay

				// check for going left, if boulder is to the right
				if (k == KEY_PRESS_LEFT && getWorld()->currentActors[i]->getX() > getWorld()->IcemanPtr_->getX())
					return true;	// it's okay

				// check for going right, if boulder is to the left
				if (k == KEY_PRESS_RIGHT && getWorld()->currentActors[i]->getX() < getWorld()->IcemanPtr_->getX())
					return true;	// it's okay

				// otherwise, boulder is in direction of movement
				return false;		// not okay
			}
		}
	}
	return true;	// Iceman is not near boulder
}
int Iceman::getNumOfSquirts() {
	return numOfSquirts_;
}
int Iceman::getNumOfSonars() {
	return numOfSonars_;
}
void Iceman::increaseNumOfOil() {
	numOfOil_++;
}

int Protestor::updateMobilityCount()
{
	//calculate number of movements from 8 <= x <= 60
	return getWorld()->genRandNumber(8, 60);
}

void RegularProtestor::doSomething()
{
	if (isActive() == false) //Checks if alive
		return;
	if (ticksToWaitBetweenMoves_ > 0) //Protestor can not move
		ticksToWaitBetweenMoves_--;
	else if(ticksToWaitBetweenMoves_ == 0) //Protestor can move
	{
		if (leaveOilFieldState_ == true)// wants to leave the oilField by beeing annoyed
		{
			//if (getLocation().xCoordinate == 60 && getLocation().yCoordinate == 60)//If Protestor reaches exit
			//{
			//	setActive(false);
			//}
			//Cannot be quirted or bonked

			//Play sound SOUND_PROTESTOR_GIVE_UP
			//resting tick set to zero so it never rests

			//If bonked by boulder increaseScore(500)
			//If annoyed by squirted increaseScore(100)

			//Find shortest path to exit

			//Moves towards exit

			//returns
		}
		
		//If Protestor is not trying to leave the oilField

		//If protester is within radius of 4 and facing Iceman 
		
		//and has not shouted within the last 15 nonresting tick
		//Shout at Iceman
		//Annoy Iceman by deducting 2 health points
		//Reset shouting variable

		//else if protestor is within vertical or horizontal line of sight of Iceman
		//and is more than 4 radius away
		//Change direction facing Iceman and move one step towards Iceman
		//set numSquaresToMoveinCurrentDirecton tp zero, forcing to pick a new direction during its
		//next non-resting tick, unless Protestor still sees Iceman then will continue to move towards Iceman
		//then return


		//If the Protestor can not see Iceman then
		//numSquaresToMoveInCurrentDirection--;
		//if numSquaresToMoveInCurrentDirection <=0 then
		//Protestor will pick a random direction
		//If Protestor can move in that direction, move numSquaresToMoveInCurrentDirection
		//else choose new direction


		//If Protestor is sitting at an intersection and 
		//made a perpendicular turn in 200 non-resting ticks
		//then pick viable route is multiple viable routes exist pick one direction randomly
		//new numSquaresToMoveInCurrentDirection values





		//At the end of each movement reset ticksToWaitBetweenMoves
		//ticksToWaitBetweenMoves_ = updateMobilityCount();
	}
}

void HardcoreProtestor::doSomething()
{
}
