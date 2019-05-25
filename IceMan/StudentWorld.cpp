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

	genNumOfItems();
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
	for (int i = 0; i < currentActors.size(); i++)
	{
		if (currentActors[i]->isActive())		// if currentActor is active
		{
			currentActors[i]->doSomething();	// actor will do something
		}
	}

	// Step 3) remove newly dead actors after each tick
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
	
	// Step 2) clear currentActors vector
	for (int i = 0; i < currentActors.size(); i++) {
		delete currentActors[i];
		currentActors[i] = nullptr;
	}
	currentActors.clear();

	// Step 4) clear iceField; free smart ptrs AND vectors
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

void StudentWorld::genNumOfItems()
{
	int l = GameWorld::getLevel();
	bouldersRemaining_ = min((l / 2) + 2, 9);
	goldRemaining_ = max((5 - l) / 2, 2);
	barrelsRemaining_ = min(2 + l, 21);
}

void StudentWorld::populateBoulder(int num)
{
	for (int h = 0; h < num; h++)
	{
		int x = genRandNumber();
		int y = genRandNumber();
		bool isCovered;

		// looks for 4x4 space that boulder image will appear
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (iceField_[x + i][y + i] != nullptr) {
					isCovered = true;
				}
				else {
					isCovered = false;
					break;
				}
			}
		}

		// location found for boulder, remove ice
		if (isCovered && noNeighbors(x, y))
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
	for (int i = 0; i < num; i++)
	{
		int x = genRandNumber();
		int y = genRandNumber();
		bool isCovered = false;
		for (int h = 0; h < 4; h++)
		{
			for (int g = 0; g < 4; g++)
			{
				if (iceField_[x + g][y + h] != nullptr)
				{
					isCovered = true;
				}
				else
				{
					isCovered = false;
					break;
				}
			}
		}
		if (isCovered && noNeighbors(x, y))
		{
			Gold* temp = new Gold(this, x, y, false);
			currentActors.push_back(temp);
		}
		else
		{
			i--; //used to reset counter to retry genRandNumber
		}
	}
}

void StudentWorld::populateBarrel(int num)
{
	for (int i = 0; i < num; i++)
	{
		int x = genRandNumber();
		int y = genRandNumber();
		bool isCovered = false;
		for (int h = 0; h < 4; h++)
		{
			for (int g = 0; g < 4; g++)
			{
				if (iceField_[x + g][y + h] != nullptr)
				{
					isCovered = true;
				}
				else
				{
					isCovered = false;
					break;
				}
			}
		}
		if (isCovered && noNeighbors(x, y))
		{
			Barrel* temp = new Barrel(this, x, y);
			currentActors.push_back(temp);
		}
		else
		{
			i--; // used to reset counter to retry genRandNumber
		}
	}
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
	const int MAX_VALUE = 56;
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
	int level = GameWorld::getLevel();
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