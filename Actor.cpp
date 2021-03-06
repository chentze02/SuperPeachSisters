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
    m_dir = dir;
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
    m_fireballRechargeTime = 0;
    m_shootPower = false;
    m_starPower = false;
    m_jumpPower = false;
}

Peach :: ~Peach() {

}

bool Peach::bonk() {
    if (m_starPower || m_tempInvicible > 0) {
        cerr << "Peach has star power and tempinvicible for " << m_tempInvicible << "inivicible "  << m_invicible <<  endl;
        return true;
    }
    else {
        cerr << "Health decreased" << endl;
        m_shootPower = false;
        m_jumpPower = false;
        decreaseHitpoint(1);
        m_tempInvicible = 10;
        cerr << "Peach hp decrease 1" << endl;
        if (getHitpoint() != 0) {
            getWorld()->playSound(SOUND_PLAYER_HURT);
        }
        else {
            cerr << "Peach hp is " << getHitpoint() << endl;
            setDead();           
        }
    }

    return true;
}

void Peach::doSomething()
{
    double x = getX();
    double y = getY();

    if (!isAlive()) {
        cerr << "died" << endl;
        return;
    }

    if (m_invicible == 0) {
        m_starPower = false;
    }

    if (peach_starPower()) {
        cerr << "Peach is invicible and would decrease" << endl;
        m_invicible--;
    }

    if (peach_tempInvincible()) {
        cerr << "Peach is temp invicible and would decrease" << endl;
        m_tempInvicible--;
    }

    if (m_shootPower && m_fireballRechargeTime > 0) {
        m_fireballRechargeTime--;
    }

    getWorld()->BonkAt(x,y);

    if (m_remaining_jump_distance > 0) {
        y = y + 4;
        if (getWorld()->objectThere(x,y)){
            getWorld()->BonkAt(x, y);
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
           else {
               getWorld()->BonkSound(x, y);
           }
            break;
        case KEY_PRESS_RIGHT:
            cerr << "Moved right" << endl;
            setDirection(0);
            x = x + 4;
            if (!getWorld()->objectThere(x,y))
                moveTo(x, y);
            else {
                getWorld()->BonkSound(x, y);
            }
            break;
        case KEY_PRESS_SPACE:
            cerr << "Suppose to have fire" << endl;
            cerr << "recharge time is " << m_fireballRechargeTime << endl;
            if (m_fireballRechargeTime == 0 && m_shootPower) {
                cerr << "shoot fireball" << endl;
                m_fireballRechargeTime = 8; 
                if (getDirection() == 0) {
                    cerr << "creating fire 0" << endl;
                    getWorld()->peachCreateFireball(x + 4, y, 0);
                }
                else if (getDirection() == 180) {
                    cerr << "creating fire 180" << endl;
                    getWorld()->peachCreateFireball(x - 4, y, 180);
                }
            }
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

Block::Block(int startX, int startY, int goodie, StudentWorld* world)
    : BaseActor(IID_BLOCK, startX, startY, 0, 1, 0, world)
{
    m_goodie = goodie;
    m_hasBeenBonk = false;
}

Block :: ~Block() {

}


void Block::doSomething() {

}

bool Block::bonk() {
    double x = getX();
    double y = getY();


    if (m_goodie == 0) { 
        getWorld()->playSound(SOUND_PLAYER_BONK);
        return true;
    }

    else {
        cerr << "Goodie created" << endl;
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        getWorld()->createGoodieForBlock(m_goodie, x, y);
        m_goodie = 0;
        m_hasBeenBonk = true;
    }

    return true;
}

void Block::bonkForSound() {
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

//PIPE IMPLEMENTATION 

Pipe::Pipe(int startX, int startY, StudentWorld* world) 
    : BaseActor(IID_PIPE, startX, startY, 0, 1, 2, world)
{
}

bool Pipe:: bonk() {
    getWorld()->playSound(SOUND_PLAYER_BONK);
    return true;
}

void Pipe::bonkForSound() {
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

//FLAG IMPLEMENTATION 

Flag::Flag(int startX, int startY, StudentWorld* world)
    : BaseActor(IID_FLAG, startX, startY, 0, 1, 1, world)
{
}

void Flag::doSomething() {
    if (!isAlive()) {
        return;
    }

    double x = getX();
    double y = getY();

    if (getWorld()->overlapsPeach(x, y)) {
        setDead();
        getWorld()->increaseScore(1000);
        getWorld()->reachFlag();
    }
}

//PROJECTILE IMPLEMENTATION
Projectile::Projectile(int imageID, int x, int y, int dir, StudentWorld* w) 
    :BaseActor(imageID, x, y, dir, 1.0, 1, w)
{
}


void Projectile::doSomething() {

    double x = this->getX();
    double y = this->getY();

    if (!getWorld()->objectBelowOthers(x, y)) {
        y -= 2;
        moveTo(x, y);
    }
    cerr << "My direction is " << direction() << endl;
    if (direction() == 180) {
        x = x - 2;
        if (getWorld()->objectThere(x, y)) {
            setDead();
            return;
        }
        else {
            cerr << "projectile shot left" << endl;
            moveTo(x, y);
        }
    }
    else if (direction() == 0) {
        x = x + 2;
        if (getWorld()->objectThere(x, y)) {
            setDead();
            return;
        }
        else {
            cerr << "projectile shot right" << endl;
            moveTo(x, y);
        }
    }
}

//// PEACH FIREBALL IMPLEMENTATION
Peach_Fireball::Peach_Fireball(int startX, int startY, int dir, StudentWorld* world) 
    : Projectile(IID_PEACH_FIRE, startX, startY, dir, world)
{
}

void Peach_Fireball::doSomething() {
    double x = this->getX();
    double y = this->getY();

    if (getWorld()->canDamage(x, y)) {
        setDead();
        return;
    }
    Projectile::doSomething();
}

//// PIRAHNA FIREBALL IMPLEMENTATION
PiranhaFireball::PiranhaFireball(int startX, int startY, int dir, StudentWorld* world)
    : Projectile(IID_PIRANHA_FIRE, startX, startY, dir, world)
{
}

void PiranhaFireball::doSomething() {
    double x = this->getX();
    double y = this->getY();

    if (getWorld()->overlapsPeach(x, y)) {
        setDead();
        getWorld()->bonkPeach();
        return;
    }

    Projectile::doSomething();
}

////SHELL IMPLEMENTATION 
Shell::Shell(int x, int y, int dir, StudentWorld* w) 
    : Projectile(IID_SHELL, x ,y , dir, w)
{
}

void Shell::doSomething() {
    double x = this->getX();
    double y = this->getY();

    if (getWorld()->canDamage(x, y)) {
        cerr << "Can damage" << endl;
        setDead();
        return;
    }
    Projectile::doSomething();
}

//GOODIE IMPLEMENTATION
Goodie::Goodie(int imageID, int x, int y, int goodie, int score, StudentWorld* w)
    :BaseActor(imageID, x, y, 0, 1.0, 1, w)
{
    m_goodie = goodie;
    m_scoreIncrease = score;
}

void Goodie::doSomething() {
    double x = getX();
    double y = getY(); 

    if (!isAlive()) {
        return;
    }

    if (getWorld()->overlapsPeach(x, y)) {
        getWorld()->increaseScore(m_scoreIncrease);
        getWorld()->createPowerForPeach(m_goodie);
        setDead();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }

    if (!getWorld()->objectBelowOthers(x,y)) {
        cerr << "Falling " << endl;
        y = y - 2;
        moveTo(x, y);
    }

    if (getDirection() == 180) {
        x = x - 2;
        if (getWorld()->objectThere(x, y)) {
            setDirection(0);
            return;
        }
        else {
            moveTo(x, y);
        }
    }
    else if (getDirection() == 0) {
        x = x + 2;
        if (getWorld()->objectThere(x, y)) {
            setDirection(180);
            return;
        }
        else {
            moveTo(x, y);
        }
    }
}

////
Flower::Flower(int x, int y, StudentWorld* w) 
    :Goodie(IID_FLOWER, x, y, 3, 50, w)
{
}

void Flower::doSomething() {
    getWorld()->setPeachHP(2);
    Goodie::doSomething();
}

Mushroom:: Mushroom(int x, int y, StudentWorld* w)
    : Goodie(IID_MUSHROOM, x, y, 2, 75, w)
{
}

void Mushroom::doSomething() {
    getWorld()->setPeachHP(2);
    Goodie::doSomething();
}

Star::Star(int x, int y, StudentWorld* w)
    : Goodie(IID_STAR, x, y, 1, 100, w)
{
}


//ENEMY IMPLEMENTATION
Enemy::Enemy(int imageID, int x, int y, StudentWorld* w) 
    :BaseActor(imageID, x, y, 0, 1.0, 1, w)
{
    int r = rand();
    if (r % 2 == 0) {
        setDirection(0);
    }
    else {
        setDirection(180);
    }
}

void Enemy::doSomething() {

    if (!isAlive()) {
        return;
    }

    double x = this->getX();
    double y = this->getY();

    //cerr << "the invicible status is " << getWorld()->peachInvicible() << endl;

    if (getWorld()->overlapsPeach(x, y) && !getWorld()->peachInvicible()) {
        cerr << "Peach has no invicible" << endl;
        getWorld()->bonkPeach();
        //setDead();      
        return;
    }

    else if (getWorld()->overlapsPeach(x, y) && getWorld()->peachInvicible()) {
        cerr << "Peach has invicible" << endl;
        getWorld()->playSound(SOUND_PLAYER_KICK);
        attacked();
    }

    if (getDirection() == 0) {
        x = x + 1;
        int x1 = x + SPRITE_WIDTH - 1;
        if (getWorld()->moveIfPossible(x1, y)) {
            moveTo(x, y);
        }
        else {
            x = x - 1;
            setDirection(180);
            moveTo(x, y);
        }
    }
    else if (getDirection() == 180) {
        x = x - 1;
        int x1 = x - SPRITE_WIDTH + 1;
        if (getWorld()->moveIfPossible(x1, y)) {
            moveTo(x, y);
        }
        else {
            x = x + 1;
            setDirection(0);
            moveTo(x, y);
        }
    }
}

bool Enemy::attacked() {
    if (isDamageable()) {
        cerr << "object is set dead" << endl;
        setDead(); 
        getWorld()->increaseScore(100); 
        return true;
    }
    return false;
}

////GOOMBA IMPLEMENTATION
Goomba::Goomba(int x, int y, StudentWorld* w) 
    :Enemy(IID_GOOMBA,x,y,w)
{
}

void Goomba::doSomething() {

    if (!isAlive()) {
        return;
    }

    double x = this->getX();
    double y = this->getY();

    //cerr << "the invicible status is " << getWorld()->peachInvicible() << endl;

    if (getWorld()->overlapsPeach(x, y) && !getWorld()->peachInvicible()) {
        cerr << "Peach has no invicible" << endl;
        getWorld()->bonkPeach();
        //setDead();      
        return;
    }

    else if (getWorld()->overlapsPeach(x, y) && getWorld()->peachInvicible()) {
        cerr << "Peach has invicible" << endl;
        getWorld()->playSound(SOUND_PLAYER_KICK);
        attacked();
    }

    if (getDirection() == 0) {
        x = x + 1;
        if (getWorld()->moveIfPossible(x, y)) {
            moveTo(x, y);
        }
        else {
            x = x - 1;
            setDirection(180);
            moveTo(x, y);
        }
    }
    else if (getDirection() == 180) {
        x = x - 1;
        if (getWorld()->moveIfPossible(x, y)) {
            moveTo(x, y);
        }
        else {
            x = x + 1;
            setDirection(0);
            moveTo(x, y);
        }
    }
}

////KOOPA IMPLEMNTATION
Koopa::Koopa(int x, int y, StudentWorld* w)
    : Enemy(IID_KOOPA, x, y, w)
{
}

bool Koopa::attacked() {
    double x = getX();
    double y = getY();

    cerr << "Koopa is attacked" << endl;
    Enemy::attacked();
    getWorld()->newShell(x, y, getDirection());
    return true;
   
}

////PIRANHA IMPLEMENTATION
Piranha::Piranha(int x, int y, StudentWorld* w) 
    : Enemy(IID_PIRANHA, x, y, w)
{
    m_firedelay = 0;
}

void Piranha::doSomething() {

    double peach_x;
    double peach_y; 
    int peach_dir;
    double piranha_x = getX();
    double piranha_y = getY();


    if (!isAlive()) {
        return;
    }

    //cerr << "the invicible status is " << getWorld()->peachInvicible() << endl;

    if (getWorld()->overlapsPeach(piranha_x, piranha_y) && !getWorld()->peachInvicible()) {
        cerr << "Peach has no invicible" << endl;
        getWorld()->bonkPeach();
        //setDead();      
        return;
    }

    else if (getWorld()->overlapsPeach(piranha_x, piranha_y) && getWorld()->peachInvicible()) {
        cerr << "Peach has invicible" << endl;
        getWorld()->playSound(SOUND_PLAYER_KICK);
        attacked();
    }

    //Enemy::doSomething();
    increaseAnimationNumber();
    getWorld()->peachLocation(peach_x, peach_y, peach_dir);
    double displacement_y = piranha_y - peach_y;
    cerr << "pir y " << piranha_y << "peach y " << peach_y << endl;
    cerr << displacement_y << endl;
    if (displacement_y <= 1.5 * SPRITE_HEIGHT && displacement_y >= -(1.5 * SPRITE_HEIGHT)) {
        cerr << "Peach within 1.5 range" << endl;
        double displacement_x = piranha_x - peach_x;
        if (displacement_x > 0 && displacement_x != 0) {
            cerr << "Peach is on left" << endl;
            setDirection(180);
        }
        else if (displacement_x < 0 && displacement_x != 0) {
            cerr << "Peach is on right" << endl;
            setDirection(0);
        }

        if (m_firedelay > 0) {
            cerr << "Need to wait before fire" << endl;
            m_firedelay--;
            return;
        }

        else {
            if(displacement_x < 8 * SPRITE_WIDTH && displacement_x > -(8 * SPRITE_WIDTH)){
                cerr << "within 8 range" << endl;
                getWorld()->piranhaFireball(piranha_x, piranha_y, getDirection());
                cerr << "Fireball shot" << endl;
                getWorld()->playSound(SOUND_PIRANHA_FIRE);
                m_firedelay = 40; 
            }
        }
    }
    else {
        return;
    }

}

//MARIO IMPLEMENTATION

Mario::Mario(int x, int y, StudentWorld* world) 
    :BaseActor(IID_MARIO, x, y, 0, 1.0, 1, world)
{
}

void Mario::doSomething() {
    if (!isAlive()) {
        return;
    }
    double x = getX();
    double y = getY();

    if (this->getWorld()->overlapsPeach(x, y)) {
        this->setDead();
        this->getWorld()->increaseScore(1000);
        this->getWorld()->saveMario();
    }
}