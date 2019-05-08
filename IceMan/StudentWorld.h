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
		iceField.resize(60, std::vector<std::shared_ptr<Ice>>(60));
		for (int i = 0; i < 60; i++) //rows
		{
			for (int j = 0; j < 30; j++) //left side
			{
				iceField[j][i] = std::make_shared<Ice>(this, j, i);
			}
			for (int k = 34; k < 60; k++) //right side
			{
				iceField[k][i] = std::make_shared<Ice>(this, k, i);
			}
			for (int l = 0; l < 3; l++) //Bottom of Shaft
			{
				iceField[i][l] = std::make_shared<Ice>(this, i, l);
			}
		}

		int B = fmin(GameWorld::getLevel() / 2 + 2, 9); // Determines number of Boulders in current level

		int G = fmax(5 - GameWorld::getLevel() / 2, 3); // Determines number of Gold Nuggets in current level

		int L = fmin(2 + GameWorld::getLevel(), 21); // Determine number of Oil Barrels in current level

		//Step 3) Allocate and insert a valid Iceman object into the game world at the proper location
		player_  = std::make_unique<Iceman>(this);
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		//This function is responsible for calling each of current Actor objects: Icemam, RegProtestor, HardProtestor,
		//Boulders, etc, doSomething() function
		//Responsible for deleting objects that need to disappear
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		return GWSTATUS_CONTINUE_GAME;
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	virtual void cleanUp()
	{
		//Function is called when Player loses a life or completes the current level
		//Deletes all Actors that are currently active
	}


private:
	std::unique_ptr<Iceman> player_; //Iceman object
	std::shared_ptr<Actor> actorP_; //Actor smartPointer
	std::vector<std::vector<std::shared_ptr<Actor>>> oilField; //Creates a 2D vector array of Actor smart pointers
	std::vector<std::vector<std::shared_ptr<Ice>>> iceField; //Creates a 2D vector array of Ice smart pointers
};

#endif // STUDENTWORLD_H_
