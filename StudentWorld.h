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
  bool Overlap(BaseActor* actor1, BaseActor* actor2) const;
  bool Overlap(double x1, double y1, double x2, double y2) const;
  bool objectBelowPeach();
  bool objectBelowOthers(double x, double y) const;
  bool BonkAt(BaseActor* actor1);
  bool BonkAt(double x, double y);
  bool objectBlocker(BaseActor* actor1);
  bool objectThere(double x, double y) const;
  void setGameStatus();
  void createGoodieForBlock(int goodie, double x, double y); 
  void peachCreateFireball(int x, int y, int dir);
  void createPowerForPeach(int goodie);
  bool moveIfPossible(double x, double y) const;
  bool overlapsPeach(double x, double y) const;
  void bonkPeach();
  bool canDamage(double x, double y);
  //bool canDamageShell(double x, double y);
  bool peachInvicible() { return m_peach->peach_Invincible(); }
  void reachFlag() { m_levelComplete = true; }
  void saveMario() { m_win = true; }
  void newShell(int x, int y, int dir); 
  void setPeachHP(int hp) const;
  bool peachHasStarPower();
  void piranhaFireball(int x, int y, int dir);


private:
	vector<BaseActor*> m_actors;
	Peach* m_peach; 
	//string m_GameStatus; 
	bool m_levelComplete; 
	bool m_win; 
};

#endif // STUDENTWORLD_H_

