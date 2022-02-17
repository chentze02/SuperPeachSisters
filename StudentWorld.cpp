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
    oss << "level0" << getLevel()  << ".txt";

    string level_file = oss.str();
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
                            //cout << " " << endl;
                            break;
                        case Level::goomba:
                            //cout << " " << endl;
                            break;
                        case Level::peach:
                            m_peach = new Peach(lx* SPRITE_HEIGHT, ly* SPRITE_WIDTH,this);
                            if (m_peach->isAlive()) {
                                cerr << "Peach is alive" << endl;
                            }
                            break;
                        case Level::flag:
                            //cout << "Location 5,10 is where a flag is" << endl;
                            break;
                        case Level::block:
                            m_actors.push_back(new Block(lx * SPRITE_HEIGHT, ly * SPRITE_WIDTH, this));
                            break;
                        case Level::star_goodie_block:
                            //cout << "Location 5,10 has a star goodie block" << endl;
                            break;
                    }
                }
            }

            cout << endl;
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_peach->doSomething();
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->isAlive() && m_peach->isAlive()) {
            m_actors[i]->doSomething();

            if (!m_peach->isAlive()) {
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }

            return GWSTATUS_CONTINUE_GAME;
        }
    }
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    for (int i = 0; i < m_actors.size(); i++) {
        delete m_actors[i];
    }
}

bool StudentWorld::Overlap(BaseActor* actor1, BaseActor* actor2) {
    double displacement_X = actor1->getX() - actor2->getX();
    double displacement_Y = actor1->getY() - actor2->getY();

    if (-(SPRITE_WIDTH + 1) <= displacement_X && displacement_X <= (SPRITE_WIDTH - 1) && -(SPRITE_HEIGHT + 1) <= displacement_Y  && displacement_Y  <= (SPRITE_HEIGHT - 1)) {
        return true;
    }
    return false;
}

bool StudentWorld :: Overlap(double x1, double y1, double x2, double y2) {
    double displacement_X = x2 - x1;
    double displacement_Y = y2 - y1;

    if (-(SPRITE_WIDTH - 1) <= displacement_X && displacement_X <= SPRITE_WIDTH - 1 && -(SPRITE_HEIGHT - 1) <= displacement_Y && displacement_Y <= SPRITE_HEIGHT - 1) {
        return true;
    }
    return false;
}


bool StudentWorld::objectBelowPeach() {
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->isBlocker()) {
            double displacement_X = m_peach->getX() - m_actors[i]->getX();
            double displacement_Y = m_peach->getY() - m_actors[i]->getY() - 3;
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

bool StudentWorld::objectThere(double x, double y) {
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


bool StudentWorld::BonkAt(double x, double y) {
    if (!objectThere(x,y)) {
        return false;
    }
    else {
        for (int i = 0; i < m_actors.size(); i++) {
            double x2 = m_actors[i]->getX();
            double y2 = m_actors[i]->getY();
            if (Overlap(x,y,x2,y2)) {
                m_actors[i]->bonk();
                return true;
            }
        }
    }

    return false;
}
