#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
using namespace std;

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class BaseActor : public GraphObject
{
public:
    BaseActor(int imageID, int startX, int startY, int dir, double size, int depth, StudentWorld* world);
    virtual ~BaseActor();
    virtual void doSomething() = 0;
    virtual bool isBlocker() = 0;
    virtual bool isDamageable() = 0;
    virtual bool attacked() { return false;  }
    virtual bool bonk() {
        return false;
    }
    int direction() const { return m_dir;}
    StudentWorld* getWorld() const;
    //bool Overlap(BaseActor* actor1, BaseActor* actor2);
    bool isAlive() const;
    void setDead();
private:
    bool m_alive;
    StudentWorld* m_world;
    int m_dir;
};

class Peach : public BaseActor {
    public:
        Peach(int startX, int startY, StudentWorld* world,  int invicible = 0, int specialPower = 0, int startHitpoint = 1);
        virtual ~Peach();
        virtual void doSomething();
        virtual bool attacked() { return false; }
        virtual bool isBlocker() { return false; }
        virtual bool isDamageable() { return true; }
        virtual bool bonk();
        int getHitpoint() const;
        void decreaseHitpoint(int amount);
        void increaseHitpoint(int amount);
        //bool canOverlap(BaseActor* actor1, BaseActor* actor2);
        bool peach_starPower() { return m_starPower; }
        bool peach_shootPower() { return m_shootPower; }
        bool peach_jumpPower() { return m_jumpPower; }
        void peach_activateStar() { m_starPower = true; m_invicible = 150; }
        void peach_activateJump() { m_jumpPower = true; }
        void peach_activateShoot() { m_shootPower = true; }
        bool peach_Invincible(); 
        bool peach_tempInvincible();
        bool peach_rechargeMode() { if (m_fireballRechargeTime == 0) { return true; } else if (m_fireballRechargeTime > 0) { m_fireballRechargeTime--; return false; } }

    private:
        int m_hitpoint; 
        int m_invicible;
        int m_tempInvicible; 
        int m_remaining_jump_distance;
        int m_jumpTicks; 
        int m_fireballRechargeTime; 
        bool m_shootPower;
        bool m_starPower;
        bool m_jumpPower;
};

class Block : public BaseActor {
public:
    Block(int startX, int startY, int goodie, StudentWorld* world);
    virtual ~Block();
    virtual void doSomething();
    virtual bool isBlocker() { return true; }
    virtual bool isDamageable() { return false;  }
    virtual bool bonk();
    bool hasBeenBonked() { return m_hasBeenBonk; }

private:
    int m_goodie; 
    bool m_hasBeenBonk;
};


class Pipe : public BaseActor {
public:
    Pipe(int startX, int startY, StudentWorld* world);
    virtual ~Pipe() {}
    virtual void doSomething() {}
    virtual bool bonk();
    virtual bool isBlocker() { return true; }
    virtual bool isDamageable() { return false; }
};

class Flag : public BaseActor {
public: 
    Flag(int startX, int startY, StudentWorld* world);
    virtual ~Flag() {};
    virtual void doSomething();
    virtual bool isBlocker() { return false; }
    virtual bool isDamageable() { return false; }
};

class Projectile : public BaseActor
{
public:
    Projectile(int imageID, int x, int y, int dir, StudentWorld* w);
    virtual ~Projectile() {}
    virtual void doSomething();
    virtual bool isBlocker() { return false; }
    virtual bool isDamageable() { return false; }
};


class Peach_Fireball : public Projectile {
public:
    Peach_Fireball(int startX, int startY, int dir, StudentWorld* world);
    virtual ~Peach_Fireball() {}
    virtual void doSomething();

};

class PiranhaFireball : public Projectile
{
public:
    PiranhaFireball(int x, int y, int dir, StudentWorld* w);
    virtual ~PiranhaFireball() {}
    virtual void doSomething();
};

class Shell : public Projectile
{
public:
    Shell(int x, int y, int dir, StudentWorld* w);
    virtual ~Shell() {}
    virtual void doSomething();
};


//GOODIE IMPLEMENTATION
class Goodie : public BaseActor
{
public:
    Goodie(int imageID, int x, int y, int goodie, int score, StudentWorld* w);
    virtual ~Goodie() {}
    virtual void doSomething();
    virtual bool isBlocker() { return false; }
    virtual bool isDamageable() { return false; }
private:
    int m_goodie; 
    int m_scoreIncrease;
};

////FLOWER IMPLEMENTATION
class Flower : public Goodie
{
public:
    Flower(int x, int y, StudentWorld* w);
    virtual ~Flower() {}
    virtual void doSomething();
};

////MUSHROOM IMPLEMENTATION
class Mushroom : public Goodie
{
public:
    Mushroom(int x, int y, StudentWorld* w);
    virtual ~Mushroom() {}
    virtual void doSomething();
};

////STAR IMPLEMENTATION
class Star : public Goodie
{
public:
    Star(int x, int y, StudentWorld* w);
};

//ENEMY
class Enemy : public BaseActor
{
public:
    Enemy(int imageID, int x, int y, StudentWorld* w);
    virtual bool attacked();
    virtual void doSomething();
    //virtual void getBonked(bool bonkerIsInvinciblePeach);
    virtual bool isBlocker() { return false; }
    virtual bool isDamageable() { return true; }

};


////Goomba 

class Goomba : public Enemy
{
public:
    Goomba(int x, int y, StudentWorld* w);
};

//// Koopa
class Koopa : public Enemy
{
public:
    Koopa(int x, int y, StudentWorld* w);
    virtual bool attacked();
};


//// Piranha

class Piranha : public Enemy
{
public:
    Piranha(int x, int y, StudentWorld* w);
    virtual void doSomething();
private:
    int m_firedelay;
};


//Mario
class Mario : public BaseActor
{
public:
    Mario(int x, int y, StudentWorld* world);
    virtual void doSomething();
    virtual bool isBlocker() { return false; }
    virtual bool isDamageable() { return false; }
};


#endif // ACTOR_H_


