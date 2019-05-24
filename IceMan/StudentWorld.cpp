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
	//Step 1) Allocate and insert a valid Iceman object into the game world at the proper location
	// Player WILL ALWAYS BE IcemanPtr_
	IcemanPtr_ = new Iceman(this);
	

	//Step 2) Construct new oil field that meets new level requirements 
	//		  ie: filled with Ice, Barrels, Boulders, GoldNuggets, etc

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

	//iceField.resize(64, vector<shared_ptr<Ice>>(64));
	//for (int i = 0; i < 60; i++)		// rows
	//{
	//	for (int j = 0; j < 30; j++)	// left side
	//	{
	//		iceField[j][i] = make_shared<Ice>(this, j, i);
	//	}
	//	for (int p = 31; p < 34; p++)	// middle of field
	//	{
	//		iceField[p][i] = nullptr;
	//	}
	//	for (int k = 34; k < 64; k++)	// right side
	//	{
	//		iceField[k][i] = make_shared<Ice>(this, k, i);
	//	}
	//	for (int l = 0; l < 3; l++)		// bottom of shaft
	//	{
	//		iceField[i][l] = make_shared<Ice>(this, i, l);
	//	}
	//}

	genNumOfItems();
	populateBoulder(bouldersRemaining_);
	//populateGold(goldRemaining_);
	//populateBarrel(barrelsRemaining_);

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
	for (int i = 0; i < currentActorVector.size(); i++)
	{
		if (currentActorVector[i]->isActive())		// if currentActor is active
		{
			currentActorVector[i]->doSomething();	// actor will do something

		}
	}
	for (int i = 0; i < currentBoulders.size(); i++)
	{
		if (currentBoulders[i]->isActive())		// if currentActor is active
		{
			currentBoulders[i]->doSomething();	// actor will do something

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
	
	// Step 2) clear currentActor vector
	for (int i = 0; i < currentActorVector.size(); i++)
	{
		currentActorVector[i].reset();
	}
	currentActorVector.clear();

	// Step 3) clear currentBoulders
	for (int i = 0; i < currentBoulders.size(); i++)
	{
		currentBoulders[i].reset();
	}
	currentBoulders.clear();

	// Step 4) clear iceField; free smart ptrs AND vectors
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			delete iceField_[i][j];
			iceField_[i][j] = nullptr;
		}
	}
	//for (int i = 0; i < 64; i++) {		// rows
	//	for (int j = 0; j < 64; j++) {	// cols
	//		// cout << "J: " << j << " I: " << i << endl;
	//		iceField[i][j].reset();
	//	}
	//	iceField[i].clear();
	//}
	//iceField.clear();
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
	for (int k = 0; k < currentActorVector.size(); k++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (currentActorVector[k]->getX() == x + i ||
					currentActorVector[k]->getY() == y + j ||
					currentActorVector[k]->getX() + i == x ||
					currentActorVector[k]->getY() + j == y)
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
				if (iceField_[x + i][j + i] != nullptr) {
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
			currentBoulders.push_back(make_unique<Boulder>(this, x, y)); // pushes new boulder
		}
		else	// reset counter and find new random numbers
			h--;
	}
}

//void StudentWorld::populateGold(int num)
//{
//	for (int i = 0; i < num; i++)
//	{
//		int x = genRandNumber();
//		int y = genRandNumber();
//		bool isCovered = false;
//		for (int h = 0; h < 4; h++)
//		{
//			for (int g = 0; g < 4; g++)
//			{
//				if (iceField[x + g][y + h] != nullptr)
//				{
//					isCovered = true;
//				}
//				else
//				{
//					isCovered = false;
//					break;
//				}
//			}
//		}
//		if (isCovered && noNeighbors(x, y))
//		{
//			currentActorVector.push_back(make_unique<Gold>(this, x, y, true));
//		}
//		else
//		{
//			i--; //used to reset counter to retry genRandNumber
//		}
//	}
//}
//
//void StudentWorld::populateBarrel(int num)
//{
//	for (int i = 0; i < num; i++)
//	{
//		int x = genRandNumber();
//		int y = genRandNumber();
//		bool isCovered = false;
//		for (int h = 0; h < 4; h++)
//		{
//			for (int g = 0; g < 4; g++)
//			{
//				if (iceField[x + g][y + h] != nullptr)
//				{
//					isCovered = true;
//				}
//				else
//				{
//					isCovered = false;
//					break;
//				}
//			}
//		}
//		if (isCovered && noNeighbors(x, y))
//		{
//			currentActorVector.push_back(make_unique<Barrel>(this, x, y));
//		}
//		else
//		{
//			i--; // used to reset counter to retry genRandNumber
//		}
//	}
//}

void StudentWorld::removeDeadGameObject()
{
	// clear current Actors
	for (int i = 0; i < currentActorVector.size(); i++)
	{
		if (currentActorVector[i]->isActive() == false)
		{
			currentActorVector.erase(currentActorVector.begin() + i);
			i--;
		}
	}

	// clear current Boulders
	for (int i = 0; i < currentBoulders.size(); i++)
	{
		if (currentBoulders[i]->isActive() == false)
		{
			currentBoulders.erase(currentBoulders.begin() + i);
			i--;
		}
	}
}

double StudentWorld::calcDistanceToBoulder(Iceman* A, unique_ptr<Boulder>& B)
{
	double Ax = A->getX();
	double Ay = A->getY();
	double Bx = B->getX();
	double By = B->getY();

	return sqrt((pow(Ax - Bx, 2) + pow(Ay - By, 2)));
}

int StudentWorld::genRandNumber()
{
	int num;
	const int MIN_VALUE = 1;
	const int MAX_VALUE = 59;
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