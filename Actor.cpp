#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//BASE IMPLEMENTATION
BaseActor :: BaseActor(int imageID, int startX, int startY, int dir, double size, int depth, StudentWorld* world)
    :GraphObject(imageID, startX, startY, dir, depth, size)
{
    m_world = world;
    m_alive = true;
}

BaseActor::~BaseActor()
{}

StudentWorld* BaseActor:: getWorld() const
{
    return m_world;
}

bool BaseActor::isAlive() const
{
    return m_alive;
}


void BaseActor::setDead()
{
    m_alive = false;
}

/////////////////////////////////////////////////////////////////////////////////
//PEACH IMPLEMENTATION

Peach::Peach(int startX, int startY, StudentWorld* world, int invicible, int specialPower, int startHitpoint)
    : BaseActor(IID_PEACH, startX, startY,0, 1,0, world)
{
    m_hitpoint = startHitpoint;
    m_invicible = invicible;
    m_tempInvicible = 0; 
}

Peach :: ~Peach() {

}
//
//bool BaseActor::Overlap(BaseActor* actor1, BaseActor* actor2) {
//    double displacement_X = actor1->getX() - actor2->getX();
//    double displacement_Y = actor1->getY() - actor2->getY();
//
//    if (SPRITE_WIDTH - 1 <= displacement_X <= SPRITE_WIDTH -1 && SPRITE_HEIGHT - 1 <= displacement_Y <= SPRITE_HEIGHT - 1) {
//        return true;
//    }
//    return false; 
//}

//bool Peach::canOverlap(BaseActor* actor1, BaseActor* actor2) {
//    
//}

void Peach::doSomething()
{
    double x = getX();
    double y = getY();

    if (!isAlive()) {
        cerr << "died" << endl;
        return;
    }

    if (peach_Invincible()) {
        m_invicible--;
    }

    if (peach_tempInvincible()) {
        m_tempInvicible--;
    }

    getWorld()->BonkAt(this);

    if (m_remaining_jump_distance > 0) {
        y = y + 4;
        if (getWorld()->BonkAt(x,y)){
            m_remaining_jump_distance = 0;
        }
        else {
            cerr << "Officially moved up" << endl;
            moveTo(x,y);
            m_remaining_jump_distance--;
        }
    }

    else if (!getWorld()->objectBelowPeach()) {
        y -= 4;
        moveTo(x, y);
    }


    int keyMove; 
    if (getWorld()->getKey(keyMove)) {
        switch (keyMove) {
        case KEY_PRESS_UP:
            cerr << "Moved up" << endl;
            if (getWorld()->objectThere(x, y - 1)) {
                if (peach_jumpPower()) {
                    m_remaining_jump_distance = 12;
                }
                else {
                    m_remaining_jump_distance = 8;
                }
                getWorld()->playSound(SOUND_PLAYER_JUMP);
            }
            break;
        case KEY_PRESS_LEFT:
            cerr << "Moved left" << endl;
            setDirection(180);
            x = x - 4;
           if(!getWorld()->objectThere(x,y))
                moveTo(x,y);      
            break;
        case KEY_PRESS_RIGHT:
            cerr << "Moved right" << endl;
            setDirection(0);
            x = x + 4;
            if (!getWorld()->objectThere(x,y))
                moveTo(x, y);
            break;
        case KEY_PRESS_SPACE:
            break;

        default:
            break;
        }
    }
}

int Peach::getHitpoint() const
{
    return m_hitpoint;
}

void Peach::decreaseHitpoint(int amount)
{
    m_hitpoint -= amount;
}

void Peach::increaseHitpoint(int amount)
{
    m_hitpoint += amount;
}

bool  Peach::peach_Invincible() {
    if (m_invicible > 0) {
        return true;
    }

    return false; 
}

bool  Peach::peach_tempInvincible() {
    if (m_tempInvicible > 0) {
        return true;
    }

    return false;
}

//BLOCK IMPLEMENTATION 

Block::Block(int startX, int startY, StudentWorld* world)
    : BaseActor(IID_BLOCK, startX, startY, 0, 1, 0, world)
{

}

Block :: ~Block() {

}


void Block::doSomething() {

}