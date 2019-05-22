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
#include <memory>
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
	{
		int l = GameWorld::getLevel();
		bouldersRemaining_ = min((l / 2) + 2, 9);
		goldRemaining_ = max((5 - l) / 2, 2);
		barrelsRemaining_ = min(2 + l, 21);
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
	void decreaseBarrelsRemaining();
	int getGoldRemaining();
	void decreaseGoldRemaining();

	int genRandNumber();
	void setDisplayText();
	double calcDistance(int x, int y);
	double calcDistance(unique_ptr<Actor> act1, unique_ptr<Actor> act2);
	bool noNeighbors(int x,  int y);

	void populateBoulder(int num);
	void populateGold(int num);
	void populateBarrel(int num);

	void removeDeadGameObject();

	/*
	Personal Data (variables, structures, etc.)
	*/
	vector<vector<shared_ptr<Ice>>> iceField; //Creates a 2D vector array of Ice smart pointers
	vector<unique_ptr<Actor>> currentActorVector; //Vector that stores Actors currently alive
	int goldRemaining_;
	int barrelsRemaining_;
	int bouldersRemaining_;

private:
	// shared_ptr<Iceman> IcemanPtr_; // points to currentActorVector[0]
};

#endif // STUDENTWORLD_H_
