#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <memory>
#include <math.h>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	virtual int init()
	{
		//Populates current level "oilField" with ice, boulders, oilBarrels, goldNuggets
		//Constructs a virtual representation of of current level with vector<vector<object>>
		//Function is called when game starts, player completes level, player loses life(not not all lives)

		//Step 1) Initialize data structure that is used to keep track of virtual world
		oilField.resize(64, std::vector<std::shared_ptr<Actor>>(64, actorP_));
		for (int y = 0; y < 64; y++)
		{
			for (int x = 0; x < 64; x++)
			{
				oilField[x][y] = std::make_shared<Actor>(this, x, y); //Creates a 64 x 64 2D vector of Actor smart_pointer objects
			}
		}

		//Step 2) Construct new oil field that meets new level requirements 
		//		  ie: filled with Ice, Barrels, Boulders, GoldNuggets, etc

		bouldersRemaining = fmin(GameWorld::getLevel() / 2 + 2, 9); // Determines number of Boulders in current level

		goldRemaining = fmax(5 - GameWorld::getLevel() / 2, 3); // Determines number of Gold Nuggets in current level

		barrelsRemaining = fmin(2 + GameWorld::getLevel(), 21); // Determine number of Oil Barrels in current level

		iceField.resize(64, std::vector<std::shared_ptr<Ice>>(64));
		for (int i = 0; i < 60; i++) //rows
		{
			for (int j = 0; j < 30; j++) //left side
			{
				iceField[j][i] = std::make_shared<Ice>(this, j, i);
			}
			for (int k = 34; k < 64; k++) //right side
			{
				iceField[k][i] = std::make_shared<Ice>(this, k, i);
			}
			for (int l = 0; l < 3; l++) //Bottom of Shaft
			{
				iceField[i][l] = std::make_shared<Ice>(this, i, l);
			}
		}

		
		//Step 3) Allocate and insert a valid Iceman object into the game world at the proper location
		//player WILL ALWAYS BE currentActorVector[0]
		if (currentActorVector.empty())
		{
			currentActorVector.resize(1);
			currentActorVector[0] = std::make_unique<Iceman>(this);
		}
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		//This function is responsible for calling each of current Actor objects: Icemam, RegProtestor, HardProtestor,
		//Boulders, etc, doSomething() function
		//Responsible for deleting objects that need to disappear
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		
		//Update the game status line
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

		//Give each Actor a chance to do something
		//for(each actor in game world)
		for(int i = 0; i < currentActorVector.size(); i++)
		{
			if(currentActorVector[i]->getHealth() > 0)//If currentActor is alive
			{
				currentActorVector[i]->doSomething();//tellThisActorToDoSomething(actor[i]);

				if(currentActorVector[0]->hasDied())//if(thePlayerDiedDuringThisTick() == true)
				{
					decLives();
					return GWSTATUS_PLAYER_DIED;
				}
				if(currentActorVector[0]->hasCompletedLevel())//if(thePlayerCompletedCurrentLevel == true)
				{
					//playFinishedLevelSound()
					return GWSTATUS_FINISHED_LEVEL;
				}
			}
		}
		//Remove newly dead actors after each tick
		//removeDeadGameObject();// delete dead game objects
		if (currentActorVector[0]->hasDied())//if(thePlayerDiedDuringThisTick() == true)
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		if (currentActorVector[0]->hasCompletedLevel())//if(thePlayerCompletedCurrentLevel == true)
		{
			//playFinishedLevelSound()
			return GWSTATUS_FINISHED_LEVEL;
		}
		return GWSTATUS_CONTINUE_GAME;
	}
	
	virtual void cleanUp()
	{
		//Function is called when Player loses a life or completes the current level
		//Deletes all Actors that are currently active
	}

	int getBarrelsRemaining() //Helper function used to detect if player has completed level
	{
		return barrelsRemaining;
	}

private:
	
	std::shared_ptr<Actor> actorP_; //Actor smartPointer
	std::vector<std::vector<std::shared_ptr<Actor>>> oilField; //Creates a 2D vector array of Actor smart pointers
	std::vector<std::vector<std::shared_ptr<Ice>>> iceField; //Creates a 2D vector array of Ice smart pointers
	std::vector<std::unique_ptr<Actor>> currentActorVector; //Vector that stores Actors currently alive

	int goldRemaining;
	int barrelsRemaining;
	int bouldersRemaining;

};

#endif // STUDENTWORLD_H_
