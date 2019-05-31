/*
	StudentWorld.cpp will define all functions except ctor/dtor.
	Has all logic and implementation.
*/
#include "StudentWorld.h"


/*
Given Functions
	- will never be called in our code/manually
*/


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

/*
Initializes world at the start of each level.
	- creates ice field
	- populates ice field with actors
 */
int StudentWorld::init()
{
	//Step 1) Create Iceman
	IcemanPtr_ = new Iceman(this);
	currentActors.push_back(IcemanPtr_);
	
	//Step 2) Construct iceField

	// BOTTOM of field
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 4; j++) {
			iceField_[i][j] = new Ice(this, i, j);
		}
	}
	// LEFT side of field
	for (int k = 0; k < 30; k++) {
		for (int l = 4; l < 60; l++) {
			iceField_[k][l] = new Ice(this, k, l);
		}
	}
	// MIDDLE of field
	for (int m = 30; m < 34; m++) {
		for (int n = 4; n < 60; n++) {
			iceField_[m][n] = nullptr;
		}
	}
	// RIGHT side of field
	for (int o = 34; o < 64; o++) {
		for (int p = 4; p < 60; p++) {
			iceField_[o][p] = new Ice(this, o, p);
		}
	}
	// TOP of field
	for (int q = 0; q < 64; q++) {
		for (int r = 60; r < 64; r++) {
			iceField_[q][r] = nullptr;
		}
	}

	int level = getLevel();
	genNumOfRandomActors(level);
	timeSinceLastProtestorAdd_ = 0;
	numberOfProtestors_ = 0;
	populateBoulder(bouldersRemaining_);
	populateGold(goldRemaining_);
	populateBarrel(barrelsRemaining_);

	return GWSTATUS_CONTINUE_GAME;
}

/*
	Responsible for gameplay. Called once per tick.
		- allows each actor to do something
*/
int StudentWorld::move()
{
	// Step 1) update display text
	setDisplayText();

	// Step 2) give each Actor a chance to do something
	IcemanPtr_->doSomething();
	
	if (IcemanPtr_->isActive() == false)	// if iceman/player died
	{
		decLives();
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}
	if (getBarrelsRemaining() <= 0)	// if iceman/player completed level
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	for (int i = 1; i < currentActors.size(); i++)
	{
		if (currentActors[i]->isActive())		// if currentActor is active
		{
			currentActors[i]->doSomething();	// actor will do something
		}
	}

	int level = getLevel();

	// Step 3) populate random Actors
	populateGoodies(level);

	// Step 4) populate Protestors
	populateProtestor(level);

	// Step 4) remove newly dead actors after each tick
	removeDeadGameObject();

	return GWSTATUS_CONTINUE_GAME;
}

/*
	Called at the end of each level.
		- frees all memory associated with data structures
			responsible for actors, field, etc.
*/
void StudentWorld::cleanUp()
{
	// Step 1) delete Iceman
	delete IcemanPtr_;
	IcemanPtr_ = nullptr;

	// Step 2) clear currentActors vector, except for Iceman at i=0
	for (int i = 1; i < currentActors.size(); i++) {
		delete currentActors[i];
		currentActors[i] = nullptr;
	}
	currentActors.clear();

	// Step 3) clear iceField; free smart ptrs AND vectors
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			delete iceField_[i][j];
			iceField_[i][j] = nullptr;
		}
	}

}


/*
Personal Functions
	- used to achieve logic/gameplay goals
*/


int StudentWorld::getBouldersRemaining()
{
	return bouldersRemaining_;
}

int StudentWorld::getBarrelsRemaining()
{
	return barrelsRemaining_;
}

void StudentWorld::decBarrels()
{
	barrelsRemaining_--;
}

int StudentWorld::getGoldRemaining()
{
	return goldRemaining_;
}

void StudentWorld::decGold()
{
	goldRemaining_--;
}

void StudentWorld::takeDamage(Actor &culprit, Actor &victim)
{
	// look for victim
	for (int i = 0; i < currentActors.size(); i++)
	{
		// if victim is Regular Protestor and within range of the culprit
		if (victim.getID() == IID_PROTESTER && culprit.calcDistance(victim) <= 7)
		{
			// if culprit is Boulder
			if (culprit.getID() == IID_BOULDER)
			{
				Character* tempChar = (Character*)(&victim);
				tempChar->decreaseHealth(10);
				increaseScore(500);
				break;
			}
				
			// if culprit is Squirt
			else if (culprit.getID() == IID_WATER_SPURT)
			{
				Protestor* tempPro = (Protestor*)(&victim);
				tempPro->decreaseHealth(2);
				if (tempPro->hasDied() == true)
					increaseScore(100);
				else
					tempPro->isStunned();
				break;
			}
		}
		// if victim is HC Protestor and within range of the culprit
		else if (victim.getID() == IID_HARD_CORE_PROTESTER && culprit.calcDistance(victim) <= 7)
		{
			// if culprit is Boulder
			if (culprit.getID() == IID_BOULDER)
			{
				Character* tempChar = (Character*)(&victim);
				tempChar->decreaseHealth(10);
				increaseScore(500);
				break;
			}

			// if culprit is Squirt
			else if (culprit.getID() == IID_WATER_SPURT)
			{
				HardcoreProtestor* tempPro = (HardcoreProtestor*)(&victim);
				tempPro->decreaseHealth(2);
				if (tempPro->hasDied() == true)
					increaseScore(250);
				else
					tempPro->isStunned();
				break;
			}

		}
		// if victim is Iceman and within range of the culprit
		else if (victim.getID() == IID_PLAYER && culprit.calcDistance(victim) <= 4)
		{
			Character* tempChar = (Character*)(&victim);

			// if culprit is Boulder
			if (culprit.getID() == IID_BOULDER)
			{
				tempChar->decreaseHealth(10);
				break;
			}
			// if culprit is Regular Protestor
			else if (culprit.getID() == IID_PROTESTER)
			{
				tempChar->decreaseHealth(2);
				break;
			}
			// if culprit is HC Protestor
			else if (culprit.getID() == IID_HARD_CORE_PROTESTER)
			{
				tempChar->decreaseHealth(2);
				break;
			}
		
		}

	}

}

bool StudentWorld::noNeighbors(int x, int y)
{
	for (int h = 0; h < currentActors.size(); h++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (currentActors[h]->getX() == x + i ||
					currentActors[h]->getY() == y + j ||
					currentActors[h]->getX() + i == x ||
					currentActors[h]->getY() + j == y)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool StudentWorld::hasIce(int x, int y)
{

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (iceField_[x + i][y + j] != nullptr)
				return true;	// has ice
	
		}
	}
	return false;
}

void StudentWorld::genNumOfRandomActors(int l)
{
	bouldersRemaining_ = min((l / 2) + 2, 9);
	goldRemaining_ = max((5 - l) / 2, 2);
	barrelsRemaining_ = min(2 + l, 21);

	totalNumberOfProtestorsOnField_ = min(15, 2 + l * 1);
	numberOfProtestors_ = 0;
}

void StudentWorld::populateBoulder(int num)
{
	for (int h = 0; h < num; h++)
	{
		int x = genRandNumber();
		int y = genRandNumber();

		// location found for boulder, remove ice
		if (hasIce(x, y) == true && noNeighbors(x, y))
		{
			for (int m = 0; m < 4; m++) {
				for (int n = 0; n < 4; n++) {
					delete iceField_[x + m][y + n];
					iceField_[x + m][y + n] = nullptr;
				}
			}
			// pushes new boulder
			Boulder* temp = new Boulder(this, x, y);
			currentActors.push_back(temp);
		}
		else	// reset counter and find new random numbers
			h--;
	}
}

void StudentWorld::populateGold(int num)
{
	for (int h = 0; h < num; h++)
	{
		int x = genRandNumber();
		int y = genRandNumber();

		if (hasIce(x, y) == true && noNeighbors(x, y))
		{
			// pushes new gold
			Gold* temp = new Gold(this, x, y, false);
			currentActors.push_back(temp);
		}
		else	// reset counter and find new random numbers
			h--;
	}
}

void StudentWorld::populateBarrel(int num)
{
	for (int h = 0; h < num; h++)
	{
		int x = genRandNumber();
		int y = genRandNumber();

		if (hasIce(x, y) == true && noNeighbors(x, y))
		{
			// pushes new barrel
			Barrel* temp = new Barrel(this, x, y);
			currentActors.push_back(temp);
		}
		else	// reset counter and find new random numbers
			h--;
	}
}

void StudentWorld::populateGoodies(int l) {
	
	int G = l * 25 + 100;

	if (rand() % G == 0) {		// 1/G chance to gen either Sonar or Water
		
		if (rand() % 100 < 20) {	// 1/5 chance to gen Sonar
			Sonar* temp = new Sonar(this, 0, 60, l);
			currentActors.push_back(temp);
		}
		else {					// 4/5 chance to gen Water
			populateWater(l);
		}
	}
}

void StudentWorld::populateWater(int l)
{
	for (int h = 0; h < 1; h++)
	{
		int x = genRandNumber(0,60);
		int y = genRandNumber(0, 60);

		if (hasIce(x, y) == false)
		{
			// pushes new barrel
			Water* temp = new Water(this, x, y, l);
			currentActors.push_back(temp);
		}
		else	// reset counter and find new random numbers
			h--;
	}
}
void StudentWorld::populateProtestor(int l)
{
	int probabilityOfHC = min(90, l*10+30);

	timeSinceLastProtestorAdd_--;
	if (timeSinceLastProtestorAdd_ <= 0 && numberOfProtestors_ <= totalNumberOfProtestorsOnField_)
	{
		// generate HC Protestor
		if (rand() % 100 < probabilityOfHC)
		{
			HardcoreProtestor* tempHCProt = new HardcoreProtestor(this, l);
			currentActors.push_back(tempHCProt);
			resetTimeSinceLastProtestorAdd(l);
			numberOfProtestors_++;
		}
		// generate Reg Protestor
		else
		{
			RegularProtestor* tempProt_ = new RegularProtestor(this, l);
			currentActors.push_back(tempProt_);
			resetTimeSinceLastProtestorAdd(l);
			numberOfProtestors_++;
		}
	}

}
void StudentWorld::resetTimeSinceLastProtestorAdd(int l)
{
	timeSinceLastProtestorAdd_ = max(25, 200 - l);
}
void StudentWorld::removeDeadGameObject()
{
	// clears inactive actors
	for (int i = 0; i < currentActors.size(); i++) {
		if (currentActors[i]->isActive() == false) {
			delete currentActors[i];
			currentActors[i] = nullptr;
			currentActors.erase(currentActors.begin() + i);
			i--;
		}
	}
}

int StudentWorld::genRandNumber()
{
	int num;
	const int MIN_VALUE = 1;
	const int MAX_VALUE = 60;
	num = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
	return num;
}

int StudentWorld::genRandNumber(int min, int max)
{
	int num;
	const int MIN_VALUE = min;
	const int MAX_VALUE = max;
	num = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
	return num;
}


void StudentWorld::setDisplayText()
{
	string temp;
	int level = GameWorld::getLevel() + 1;
	int lives = GameWorld::getLives();
	int score = GameWorld::getScore();
	
	int health = IcemanPtr_->getHealth();
	int squirts = IcemanPtr_->getNumOfSquirts();
	int sonar = IcemanPtr_->getNumOfSonars();
	int gold = IcemanPtr_->getNumOfGold();
	
	temp = "Lvl: " + to_string(level) + " Lives: " + to_string(lives) +
		" Hlth: " + to_string(health * 10) + "% Wtr: " + to_string(squirts) +
		" Gld: " + to_string(gold) + " Oil Left: " + to_string(barrelsRemaining_) +
		" Sonar: " + to_string(sonar) + " Scr: " + to_string(score);
	setGameStatText(temp);
}