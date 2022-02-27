#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream> 
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_levelComplete = false;
    m_win = false;
}

StudentWorld :: ~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(2);
    if (getLevel() < 9) {
        oss << "level0" << getLevel() << ".txt";
    }
    else {
        oss << "level" << getLevel() << ".txt";
    }

    cerr << "The level is suppose to be" << getLevel() << endl;
    string level_file = oss.str();
    cerr << level_file << endl;
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
            for (int lx = 0; lx < GRID_HEIGHT; lx++) {
                for (int ly = 0; ly < GRID_WIDTH; ly++) {
                    Level::GridEntry ge;
                    ge = lev.getContentsOf(lx, ly); // x=5, y=10
                    switch (ge) {
                        case Level::empty:
                            /*cout << " " << endl;*/
                            break;
                        case Level::koopa:
                            m_actors.push_back(new Koopa(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, this));
                            break;
                        case Level::goomba:
                            m_actors.push_back(new Goomba(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, this));
                            break;
                        case Level::peach:
                            m_peach = new Peach(lx* SPRITE_HEIGHT, ly* SPRITE_WIDTH,this);
                            if (m_peach->isAlive()) {
                                cerr << "Peach is alive" << endl;
                            }
                            break;
                        case Level::flag:
                            m_actors.push_back(new Flag(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, this));
                            break;
                        case Level::block:
                            m_actors.push_back(new Block(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, 0, this));
                            break;
                        case Level::star_goodie_block:
                            m_actors.push_back(new Block(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, 1, this));
                            break;

                        case Level::flower_goodie_block:                       
                            m_actors.push_back(new Block(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, 3, this));
                            break;
         
                        case Level::mushroom_goodie_block:                          
                            m_actors.push_back(new Block(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, 2, this));
                            break;

                        case Level::pipe:
                            m_actors.push_back(new Pipe(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, this));
                            break;
                        case Level::piranha: 
                            m_actors.push_back(new Piranha(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, this));
                            break;                       
                        case Level::mario:                            
                            m_actors.push_back(new Mario(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, this));
                            break;                      
                    }
                }
            }

            cout << endl;
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld:: move()
{

    m_peach->doSomething();

    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->isAlive() && m_peach->isAlive()) {
            m_actors[i]->doSomething();
        }

        if (!m_actors[i]->isAlive()) {
            cerr << "Koopa is set to dead " << endl;
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
        }

        if (!m_peach->isAlive()) {
            cerr << "Peach has died " << endl;
            decLives();            
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
    }


    for (int i = 0; i < m_actors.size(); i++) {
        if (!m_actors[i]->isAlive()) {
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
        }
    }

    if (m_levelComplete) {
        cerr << "level completed" << endl;
        m_levelComplete = false;
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }

    if (m_win) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }


    setGameStatus();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
        delete m_peach;
   /* m_actors.erase(m_actors.begin(), m_actors.end());
    m_actors.shrink_to_fit();*/

    for (int i = 0; i < m_actors.size(); i++) {
        delete m_actors[i];
    }

    m_actors.clear();
    cerr << "The size of the vector is " << m_actors.size() << endl;
}

bool StudentWorld::Overlap(BaseActor* actor1, BaseActor* actor2) const {
    double displacement_X = actor1->getX() - actor2->getX();
    double displacement_Y = actor1->getY() - actor2->getY();

    if (-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= (SPRITE_WIDTH - 1) && -(SPRITE_HEIGHT - 1) <= displacement_Y  && displacement_Y  <= (SPRITE_HEIGHT - 1)) {
        return true;
    }
    return false;
}

bool StudentWorld :: Overlap(double x1, double y1, double x2, double y2) const {
    double displacement_X = x2 - x1;
    double displacement_Y = y2 - y1;

    if (-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= (SPRITE_WIDTH - 1) && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= (SPRITE_HEIGHT - 1)) {
        return true;
    }
    return false;
}


bool StudentWorld::objectBelowPeach() {
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->isBlocker()) {
            double displacement_X = m_peach->getX() - m_actors[i]->getX();
            double displacement_Y = m_peach->getY() - m_actors[i]->getY() - 3;
            if (-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= (SPRITE_WIDTH - 1) && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= (SPRITE_HEIGHT - 1)) {
                return true;
            }
        }
    }

    return false;

}

bool StudentWorld::objectBelowOthers(double x, double y) const {
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->isBlocker()) {
            double displacement_X = x - m_actors[i]->getX();
            double displacement_Y = y - m_actors[i]->getY() - 1;
            if (-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= SPRITE_HEIGHT - 1) {
                return true;
            }
        }
    }

    return false;

}

bool StudentWorld::objectBlocker(BaseActor* actor1) {
    for (int i = 0; i < m_actors.size(); i++) {
        if (Overlap(actor1, m_actors[i]) && m_actors[i]->isBlocker()) {
            return true;
        }
    }

    return false; 
}

bool StudentWorld::objectThere(double x, double y) const{
    for (int i = 0; i < m_actors.size(); i++) {
        if (Overlap(x,y,m_actors[i]->getX(), m_actors[i]->getY()) && m_actors[i]->isBlocker()) {
            return true;
        }
    }

    return false;
}


bool StudentWorld::BonkAt(BaseActor* actor1) {
    if (!objectBlocker(actor1)){
        return false; 
    }
    else {
        for (int i = 0; i < m_actors.size(); i++) {
            if (objectBlocker(actor1)) {
                m_actors[i]->bonk();
                return true;
            }
        }
    }

    return false; 
}

bool StudentWorld::BonkSound(double x, double y) {
    for (int i = 0; i < m_actors.size(); i++) {
        double x2 = m_actors[i]->getX();
        double y2 = m_actors[i]->getY();
        if (Overlap(x, y, x2, y2)) {
            m_actors[i]->bonkForSound();
            return true;
        }
    }

    return false;
}

bool StudentWorld::BonkAt(double x, double y) {
    for (int i = 0; i < m_actors.size(); i++) {
        double x2 = m_actors[i]->getX();
        double y2 = m_actors[i]->getY();
        if (Overlap(x, y, x2, y2)) {
            m_actors[i]->bonk();
            return true;
        }
    }
   
    return false;
}


void StudentWorld::setGameStatus() {
    string powers = "";
    ostringstream text;
    text.setf(ios::fixed);
    text.precision(2);

    if (m_peach->peach_shootPower()) {
        powers += "ShootPower! ";
    }
    if (m_peach->peach_jumpPower()) {
        powers += "JumpPower! ";
    }
    if (m_peach->peach_starPower()) {
        powers += "StarPower! ";
    }

    text << "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << getScore() << "  " << powers;
    string GameStatus = text.str();

    setGameStatText(GameStatus); 
}

void StudentWorld::createGoodieForBlock(int goodie, double x, double y) {
    if (goodie == 0) {
        return;
    }

    if (goodie == 1) {      
        m_actors.push_back(new Star(x, y + 8, this));
    }
    else if (goodie == 2) {        
        m_actors.push_back(new Mushroom(x, y + 8, this));
    }
    else if (goodie == 3) {        
        m_actors.push_back(new Flower(x, y + 8, this));
    }

    /*m_actors.push_back(goodieType,x,y);*/ 
}

void StudentWorld::peachCreateFireball(int x, int y, int dir) {
    m_actors.push_back(new Peach_Fireball(x, y, dir, this));
}

void StudentWorld::createPowerForPeach(int goodie) {
    if (goodie == 1) {
        m_peach->peach_activateStar();
    }
    else if (goodie == 2) {
        m_peach->peach_activateJump();
    }
    else if (goodie == 3) {
        m_peach->peach_activateShoot(); 
    }
}

bool StudentWorld::moveIfPossible(double x, double y) const {
    bool cond1 = objectBelowOthers(x, y);
    bool cond2 = objectThere(x, y);
    if (cond1 && !cond2) {
        return true;
    }
    else {
        return false;
    }
}

bool StudentWorld::overlapsPeach(double x, double y) const {
    if (Overlap(x, y, m_peach->getX(), m_peach->getY())) {
        return true;
    }

    return false;
}

void StudentWorld::bonkPeach() {
    m_peach->bonk();
}

bool StudentWorld::canDamage(double x, double y) {
    for (int i = 0; i < m_actors.size(); i++) {
        if (Overlap(x, y, m_actors[i]->getX(), m_actors[i]->getY()) && m_actors[i]->isDamageable()) {
            cerr << "Can be attacked" << endl;
            if (m_actors[i]->attacked()) {
                return true;
            }
        }
    }
    return false;
}

void StudentWorld::newShell(int x, int y, int dir) {
    m_actors.push_back(new Shell(x, y, dir, this));
    cerr << "Shell is created" << endl;
}

void StudentWorld::setPeachHP(int hp) const {
    int currHP = m_peach->getHitpoint();
    int toIncrease = hp - currHP;

    m_peach->increaseHitpoint(toIncrease);
}

bool StudentWorld::peachHasStarPower() {
    return m_peach->peach_starPower();
}

void StudentWorld::piranhaFireball(int x, int y, int dir) {
    m_actors.push_back(new PiranhaFireball(x, y, dir, this));
}

void StudentWorld::peachLocation(double& x, double& y, int& dir) {
    x = m_peach->getX();
    y = m_peach->getY();
    dir = m_peach->getDirection();
}