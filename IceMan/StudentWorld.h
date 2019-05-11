#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <memory>
#include <math.h>
#include <ctime>
#include <cstdlib>


class StudentWorld : public GameWorld
{
public:
	std::vector<std::vector<std::shared_ptr<Ice>>> iceField; //Creates a 2D vector array of Ice smart pointers
	std::vector<std::unique_ptr<Actor>> currentActorVector; //Vector that stores Actors currently alive


	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	// Personal Functions
	int genRandNumber();
	void setDisplayText();
	int getBarrelsRemaining();
	int calcDistance(int x, int y);
	int calcDistance(std::unique_ptr<Actor> act1, std::unique_ptr<Actor> act2);
	bool noNeighbors(int x,  int y);
	//void populateActor(int num, Actor A);
	//void populateActor(int num, Actor A, bool isVis);
	void populateBoulder(int num);
	void populateGold(int num);
	void populateBarrel(int num);
	void removeDeadGameObject();

	virtual int init()
	{
		//Populates current level "oilField" with ice, boulders, oilBarrels, goldNuggets
		//Constructs a virtual representation of of current level with vector<vector<object>>
		//Function is called when game starts, player completes level, player loses life(not not all lives)

		//Step 1) Allocate and insert a valid Iceman object into the game world at the proper location
		// Player WILL ALWAYS BE currentActorVector[0]
		if (currentActorVector.empty())
		{
			currentActorVector.resize(1);
			currentActorVector[0] = std::make_unique<Iceman>(this);
		}
		else
		{
			currentActorVector[0].release();
			currentActorVector[0] = std::make_unique<Iceman>(this);
		}

		//Step 2) Construct new oil field that meets new level requirements 
		//		  ie: filled with Ice, Barrels, Boulders, GoldNuggets, etc
		iceField.resize(64, std::vector<std::shared_ptr<Ice>>(64));
		for (int i = 0; i < 60; i++) //rows
		{
			for (int j = 0; j < 30; j++) //left side
			{
				iceField[j][i] = std::make_shared<Ice>(this, j, i);
			}
			for (int p = 31; p < 34; p++)
			{
				iceField[p][i] = nullptr;
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


		bouldersRemaining = fmin(GameWorld::getLevel() / 2 + 2, 9); // Determines number of Boulders in current level
		populateBoulder(bouldersRemaining);

		goldRemaining = fmax(5 - GameWorld::getLevel() / 2, 3); // Determines number of Gold Nuggets in current level
		populateGold(goldRemaining);

		barrelsRemaining = fmin(2 + GameWorld::getLevel(), 21); // Determine number of Oil Barrels in current level
		populateBarrel(barrelsRemaining);

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
		setDisplayText();

		//Give each Actor a chance to do something
		//for(each actor in game world)
		for(int i = 0; i < currentActorVector.size(); i++)
		{
				if (currentActorVector[i]->isAlive())//If currentActor is alive
				{
					currentActorVector[i]->doSomething();//tellThisActorToDoSomething(actor[i]);

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
		iceField.clear();
		currentActorVector.clear();
	}

private:
	
	std::shared_ptr<Actor> actorP_; //Actor smartPointer
	
	int goldRemaining;
	int barrelsRemaining;
	int bouldersRemaining;

};

#endif // STUDENTWORLD_H_
