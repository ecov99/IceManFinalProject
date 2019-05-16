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
	// Player WILL ALWAYS BE currentActorVector[0]
	if (currentActorVector.empty())
	{
		currentActorVector.push_back(make_unique<Iceman>(this));
	}

	//Step 2) Construct new oil field that meets new level requirements 
	//		  ie: filled with Ice, Barrels, Boulders, GoldNuggets, etc
	iceField.resize(64, std::vector<shared_ptr<Ice>>(64));
	for (int i = 0; i < 60; i++)		// rows
	{
		for (int j = 0; j < 30; j++)	// left side
		{
			iceField[j][i] = std::make_shared<Ice>(this, j, i);
		}
		for (int p = 31; p < 34; p++)
		{
			iceField[p][i] = nullptr;
		}
		for (int k = 34; k < 64; k++)	// right side
		{
			iceField[k][i] = std::make_shared<Ice>(this, k, i);
		}
		for (int l = 0; l < 3; l++)		// bottom of shaft
		{
			iceField[i][l] = std::make_shared<Ice>(this, i, l);
		}
	}

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
	for (int i = 0; i < currentActorVector.size(); i++)
	{
		if (currentActorVector[i]->isActive())		// if currentActor is active
		{
			currentActorVector[i]->doSomething();	// actor will do something

			if (currentActorVector[0]->isActive() == false)	// if iceman/player died
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (getBarrelsRemaining() <= 0)	// if iceman/player completed level
			{
				return GWSTATUS_FINISHED_LEVEL;
			}
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
	// Step 1) clear currentActor vector
	currentActorVector.clear();

	// Step 2) clear iceField; free smart ptrs AND vectors
	for (int i = 0; i < 64; ++i) {		// rows
		for (int j = 0; j < 64; ++j) {	// cols
			iceField[j][i].reset();
		}
		iceField[i].clear();
	}
	iceField.clear();
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

int StudentWorld::getGoldRemaining()
{
	return goldRemaining_;
}

int StudentWorld::calcDistance(int x, int y)
{
	double radius = sqrt(x * x + y * y);
	return radius;
}
 
int StudentWorld::calcDistance(std::unique_ptr<Actor> act1, std::unique_ptr<Actor> act2)
{
	int ix = act1->getX();
	int iy = act1->getY();
	int bx = act2->getX();
	int by = act2->getY();
	int x = std::abs(ix - bx);
	int y = std::abs(iy - by);
	int radius = sqrt(x * x + y * y);
	return radius;
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

//void StudentWorld::populateActor(int num, Actor A)
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
//			for (int h = 0; h < 4; h++)
//			{
//				for (int g = 0; g < 4; g++)
//				{
//					iceField[x + g][y + h].reset();
//					iceField[x + g][y + h] = nullptr;
//				}
//			}
//			currentActorVector.push_back(std::make_unique<A>(this, x, y));
//		}
//		else
//		{
//			i--; //used to reset counter to retry genRandNumber
//		}
//	}
//}
//
//void StudentWorld::populateActor(int num, Actor A, bool isVis)
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
//			currentActorVector.push_back(std::make_unique<A>(this, x, y, isVis));
//		}
//		else
//		{
//			i--; //used to reset counter to retry genRandNumber
//		}
//	}
//}

void StudentWorld::populateBoulder(int num)
{
	for (int i = 0; i < num; i++)
	{
		int x = genRandNumber();
		int y = genRandNumber();
		bool isCovered = false;
		for (int h = 0; h < 4; h++)	// looks for 4x4 space that boulder image will appear
		{
			for (int g = 0; g < 4; g++)
			{
				if (iceField[x + g][y + h] != nullptr)
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
			for (int h = 0; h < 4; h++)	// removes ice
			{
				for (int g = 0; g < 4; g++)
				{
					iceField[x + g][y + h].reset();
					iceField[x + g][y + h] = nullptr;
				}
			}
			currentActorVector.push_back(std::make_unique<Boulder>(this, x, y)); // pushes new boulder
		}
		else
		{
			i--; //used to reset counter to retry genRandNumber
		}
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
				if (iceField[x + g][y + h] != nullptr)
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
			currentActorVector.push_back(std::make_unique<Gold>(this, x, y, true));
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
				if (iceField[x + g][y + h] != nullptr)
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
			currentActorVector.push_back(std::make_unique<Barrel>(this, x, y));
		}
		else
		{
			i--; // used to reset counter to retry genRandNumber
		}
	}
}

void StudentWorld::removeDeadGameObject()
{
	for (int i = 0; i < currentActorVector.size(); i++)
	{
		if (currentActorVector[i]->isActive() == false)
		{
			currentActorVector.erase(currentActorVector.begin() + i);
			i--;
		}
	}
}

int StudentWorld::genRandNumber()
{
	int num;
	const int MIN_VALUE = 0;
	const int MAX_VALUE = 59;
	unsigned seed = time(0);
	srand(seed);
	num = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
	return num;
}

void StudentWorld::setDisplayText()
{
	string temp;
	int level = GameWorld::getLevel();
	int lives = GameWorld::getLives();
	int score = GameWorld::getScore();
	
	//int health = currentActorVector[0]->getHealth();
	//int squirts = currentActor[0]->get_numOfSquirts();
	//int sonar = currentActor[0]->get_numOfSonars();
	//int gold = currentActor[0]->get_numOfGold();

	int health = 0;
	int squirts = 0;
	int sonar = 0;
	int gold = 0;
	
	temp = "Lvl: " + to_string(level) + " Lives: " + to_string(lives) +
		" Hlth: " + to_string(health * 10) + "% Wtr: " + to_string(squirts) +
		" Gld: " + to_string(gold) + " Oil Left: " + to_string(barrelsRemaining_) +
		" Sonar: " + to_string(sonar) + " Scr: " + to_string(score);
	setGameStatText(temp);
}