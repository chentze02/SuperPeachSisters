#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class BaseActor : public GraphObject
{
public:
    BaseActor(int imageID, int startX, int startY, int dir, double size, int depth, StudentWorld* world);
    virtual ~BaseActor();
    virtual void doSomething() = 0;
    virtual bool isBlocker() { return false; }
    StudentWorld* getWorld() const;
    bool Overlap(BaseActor* actor1, BaseActor* actor2);
    bool isAlive() const;
    void setDead();
private:
    bool m_alive;
    StudentWorld* m_world;
};

class Peach : public BaseActor {
    public:
        Peach(int startX, int startY, StudentWorld* world,  int invicible = 0, int specialPower = 0, int startHitpoint = 1);
        virtual ~Peach();
        virtual void doSomething();
        int getHitpoint() const;
        void decreaseHitpoint(int amount);
        void increaseHitpoint(int amount);
        bool canOverlap(BaseActor* actor1, BaseActor* actor2);
        bool objectBelow();
        void peach_Invincible(); 

    private:
        int m_hitpoint; 
        int m_invicible;
        int m_specialPower;
        int m_keyValue;
};

class Block : public BaseActor {
public:
    Block(int startX, int startY, StudentWorld* world);
    virtual ~Block();
    virtual void doSomething();
    virtual bool isBlocker() { return true; }
};




#endif // ACTOR_H_


