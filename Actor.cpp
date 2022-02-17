#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//BASE IMPLEMENTATION
BaseActor :: BaseActor(int imageID, int startX, int startY, int dir, double size, int depth, StudentWorld* world)
    :GraphObject(imageID, startX, startY, dir, size, depth)
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
    m_specialPower = specialPower;
}

Peach :: ~Peach() {

}

bool BaseActor::Overlap(BaseActor* actor1, BaseActor* actor2) {
    double displacement_X = actor1->getX() - actor2->getX();
    double displacement_Y = actor1->getY() - actor2->getY();

    if (SPRITE_WIDTH - 1 <= displacement_X <= SPRITE_WIDTH -1 && SPRITE_HEIGHT - 1 <= displacement_Y <= SPRITE_HEIGHT - 1) {
        return true;
    }
    return false; 
}

bool Peach::canOverlap(BaseActor* actor1, BaseActor* actor2) {
    
}

void Peach::doSomething()
{
    if (!isAlive()) {
        return;
    }

    if (getWorld()->objectBelowPeach()) {
        moveTo(getX(), getY()-4);
    }

    if (getWorld()->getKey(m_keyValue)) {
        switch (m_keyValue) {
        case KEY_PRESS_UP:
            if (getY() + 4 < VIEW_HEIGHT) {
                moveTo(getX(), getY() + 4);
            }
            break;
        case KEY_PRESS_DOWN:
            if (getY() - 4 >= 0) {
                moveTo(getX(), getY() - 4);
            }
            break;
        case KEY_PRESS_LEFT:
            if (getX() - 4 >= 0) {
                moveTo(getX() - 4, getY());
            }
            break;
        case KEY_PRESS_RIGHT:
            if (getX() + 4 < VIEW_WIDTH) {
                moveTo(getX() + 4, getY());
            }
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

//BLOCK IMPLEMENTATION 

Block::Block(int startX, int startY, StudentWorld* world)
    : BaseActor(IID_BLOCK, startX, startY, 0, 1, 0, world)
{

}

Block :: ~Block() {

}


void Block::doSomething() {

}