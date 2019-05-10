#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
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