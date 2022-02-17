#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  bool Overlap(BaseActor* actor1, BaseActor* actor2);
  bool Overlap(double x1, double y1, double x2, double y2);
  bool objectBelowPeach();
  bool BonkAt(BaseActor* actor1);
  bool BonkAt(double x, double y);
  bool objectBlocker(BaseActor* actor1);
  bool objectThere(double x, double y); 

private:
	vector<BaseActor*> m_actors;
	Peach* m_peach; 
};

#endif // STUDENTWORLD_H_

