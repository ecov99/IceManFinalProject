#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
/*
	StudentWorld.h will define ctor/dtor.
	Will also declare functions, data members.
*/
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;

class StudentWorld : public GameWorld
{
public:

	/*
	Given Functions
		- will never be called in our code/manually
	*/
	StudentWorld(string assetDir)
		: GameWorld(assetDir)
	{}
	~StudentWorld()
	{
		// DELETE TEST OBJECTS
		delete testBoulder_;
		testBoulder_ = nullptr;
	}


	virtual int init();
	virtual int move();
	virtual void cleanUp();

	/*
	Personal Functions
		- used to achieve logic/gameplay goals
	*/
	int getBouldersRemaining();
	int getBarrelsRemaining();
	void decBarrels();
	int getGoldRemaining();
	void decGold();
	void takeDamage(Actor &culprit, Actor &victim);

	int genRandNumber();
	int genRandNumber(int min, int max);
	void setDisplayText();
	bool noNeighbors(int x,  int y);
	bool hasIce(int x, int y);
	void genNumOfRandomActors();
	void populateBoulder(int num);
	void populateGold(int num);
	void populateBarrel(int num);
	void populateWater(int num);
	void populateGoodies();
	void populateProtestor();
	void resetTimeSinceLastProtestorAdd(int l);
	void removeDeadGameObject();
	
	/*
	Personal Data (variables, structures, etc.)
	*/
	Ice* iceField_[64][64];			// creates a 2D array of Ice pointers
	vector<Actor*> currentActors;	// holds all active actors
	int goldRemaining_;
	int barrelsRemaining_;
	int bouldersRemaining_;

	int timeSinceLastProtestorAdd_;
	int numberOfProtestors_;
	int totalNumberOfProtestorsOnField_;
	
	Iceman* IcemanPtr_; // points to IcemanPtr_
	Boulder* testBoulder_;
};

#endif // STUDENTWORLD_H_
