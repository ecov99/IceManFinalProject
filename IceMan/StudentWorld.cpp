#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::getBarrelsRemaining()
{
	return barrelsRemaining;
}

int StudentWorld::calcDistance(int x, int y)
{
	int radius = sqrt(x * x + y * y);
	return radius;
}

int StudentWorld::calcDistance(std::unique_ptr<Actor> act1, std::unique_ptr<Actor> act2)
{
	int ix = act1->getX();
	int iy = act1->getY();
	int bx = act2->getX();
	int by = act2->getY();
	int x = abs(ix - bx);
	int y = abs(iy - by);
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
//
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
			for (int h = 0; h < 4; h++)
			{
				for (int g = 0; g < 4; g++)
				{
					iceField[x + g][y + h].reset();
					iceField[x + g][y + h] = nullptr;
				}
			}
			currentActorVector.push_back(std::make_unique<Boulder>(this, x, y));
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
			currentActorVector.push_back(std::make_unique<GoldNugget>(this, x, y, true));
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
			i--; //used to reset counter to retry genRandNumber
		}
	}
}

void StudentWorld::removeDeadGameObject()
{
	for (int i = 0; i < currentActorVector.size(); i++)
	{
		if (currentActorVector[i]->isAlive() == false)
		{
			currentActorVector.erase(currentActorVector.begin() + i);
			i = 0;
		}
	}
}

int StudentWorld::genRandNumber()
{
	int num;
	const int MIN_VALUE = 0;
	const int MAX_VALUE = 60;
	unsigned seed = time(0);
	srand(seed);
	num = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
	return num;
}

void StudentWorld::setDisplayText()
{
	int level = GameWorld::getLevel();
	int lives = GameWorld::getLives();
	int health = currentActorVector[0]->getHealth();
	int squirts = currentActorVector[0]->getSquirts();
	int gold = currentActorVector[0]->getGold();
	int barrelsLeft = barrelsRemaining;
	int sonar = currentActorVector[0]->getSonar();
	int score = GameWorld::getScore();
	std::string s = "Lvl: " + std::to_string(level) + " Lives: " + std::to_string(lives) +
		" Hlth: " + std::to_string(health * 10) + "% Wtr: " + std::to_string(squirts) + " Gld: " +
		std::to_string(gold) + " Oil Left: " + std::to_string(barrelsLeft) + " Sonar: " +
		std::to_string(sonar) + " Scr: " + std::to_string(score);
	setGameStatText(s);
}