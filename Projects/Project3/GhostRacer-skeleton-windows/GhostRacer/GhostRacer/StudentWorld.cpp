#include "StudentWorld.h"
#include "GameConstants.h"
#include <sstream>
#include <string>
#include "Actor.h"
using namespace std;

//Pseudocode for complicated functions can be found in my report

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    actorList.size();
    gr = nullptr;
}
//Get GR Speed
double StudentWorld::getGRSpeed() { return gr->getVSpeed(); };
//Get GR pointer
GhostRacer* StudentWorld::getGR() { return gr; };
//End level if necessary
void StudentWorld::endLevel() { continueLevel = false; };
//Add a new actor
void StudentWorld::addActor(Actor* a)
{
    actorList.push_back(a);
}
void StudentWorld::soulSaved() { lostSoulsSaved++; };
//Returns true if two actors overlap
bool StudentWorld::overlaps(Actor* first, Actor* second)
{
    double delta_x = abs(first->getX() - second->getX());
    double delta_y = abs(first->getY() - second->getY());
    double rSum = first->getRadius() + second->getRadius();
    if (delta_x < rSum * .25 && delta_y < rSum * .6)
        return true;
    return false;
}
//Add an actor based on random chance
void StudentWorld::possiblyAddActor(int randMax, char type)
{
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    int rand = randInt(0, randMax - 1);
    int rand1 = randInt(0, 255);
    int rand2 = randInt(LEFT_EDGE, RIGHT_EDGE);
    if (rand != 0)
        return;

    switch (type)
    {
    case('h'):
    {
        Actor* newHP = new HumanPed(IID_HUMAN_PED, rand1, VIEW_HEIGHT, 0, 2.0, 0, -4, 0, 2, this, 0);
        actorList.push_back(newHP);
        break;
    }
    case('z'):
    {
        Actor* newZ = new ZombiePed(IID_ZOMBIE_PED, rand1, VIEW_HEIGHT, 0, 3.0, 0, 0, -4, 0, this);
        actorList.push_back(newZ);
        break;
    }
    case('s'):
    {

        Actor* newZ = new SoulGoodie(this, rand2, VIEW_HEIGHT);
        addActor(newZ);
        break;
    }
    case('o'):
    {
        Actor* newOS = new OilSlick(this, rand2, VIEW_HEIGHT);
        addActor(newOS);
        break;
    }
    case('w'):
    {
        Actor* newHWG = new HolyWaterGoodie(this, rand2, VIEW_HEIGHT);
        addActor(newHWG);
        break;
    }
    }

}
//Find any available actors to spray
bool StudentWorld::sprayFirstActor(Actor* a)
{
    for (int i = 0; i < actorList.size(); i++)
    {
        if (overlaps(a, actorList[i]) && actorList[i]->isAlive() && actorList[i]->isAffectedbySpray())
        {
            actorList[i]->beSprayed();
            return true;
        }
    }
    return false;
}
//Check for CAW actors fo the zombie cabs
bool StudentWorld::checkForZombieCabF(Actor* a)
{
    int ZCL = a->getActorLane();
    for (int i = 0; i < actorList.size(); i++)
    {
        if (actorList[i]->getActorLane() == ZCL && actorList[i]->isCAW() && (actorList[i]->getY() - a->getY()) < 96 && actorList[i]->getY() > a->getY())
            return true;
    }
    return false;
}
//Return true if the level is complete
bool StudentWorld::completedLevel()
{
    if (lostSoulsSaved >= (getLevel() * 2 + 5))
        return true;
    return false;
}
//Return the number of saved souls
int StudentWorld::getSoulsToSave()
{
    return ((getLevel() * 2 + 5) - lostSoulsSaved);
}
//Update the game status text
void StudentWorld::updateStatusText()
{
    ostringstream updatedText;
    updatedText << "Score: " << getScore() << "  Lvl: " << getLevel() << " Souls2Save: " << getSoulsToSave() << "  Lives: " << getLives() << "  Health: " << gr->getHealth() << "  Sprays: " << gr->getSprays() << "  Bonus: " << currentBonus;
    string uT = updatedText.str();
    setGameStatText(uT);
}
//Check for CAW actors for the zombie cab
bool StudentWorld::checkForZombieCabB(Actor* a)
{
    int ZCL = a->getActorLane();
    for (int i = 0; i < actorList.size(); i++)
    {
        if (actorList[i]->getActorLane() == ZCL && actorList[i]->isCAW() && abs((actorList[i]->getY() - a->getY())) < 96 && actorList[i]->getY() < a->getY())
            return true;
    }
    return false;
}
//Try to add a zombie cab
void StudentWorld::tryToAddZC()
{
    int chosenSpeed;
    int chosenLane;
    int checkedLanes = 0;
    int chosenY;
    Actor* bottom = nullptr;
    Actor* top = nullptr;
    bool alreadyChosen = false;
    int cur_lane = randInt(1, 3);
    while (!alreadyChosen)
    {
        bottom = nullptr;
        top = nullptr;
        double curXmin = 256;
        double curXmax = 0;
        if (checkedLanes == 3)
            break;
        for (int i = 0; i < actorList.size(); i++)
        {
            if (actorList[i]->getActorLane() == cur_lane && actorList[i]->getY() <= curXmin && actorList[i]->isCAW() == true)
                bottom = actorList[i];
        }
        if (gr->getActorLane() == cur_lane && gr->getY() <= curXmin)
            bottom = gr;
        if (bottom == nullptr || bottom->getY() > (VIEW_HEIGHT / 3))
        {
            chosenLane = cur_lane;
            chosenY = (SPRITE_HEIGHT / 2);
            chosenSpeed = (getGRSpeed() + randInt(2, 4));
            alreadyChosen = true;
        }
        if (!alreadyChosen)
        {
            for (int i = 0; i < actorList.size(); i++)
            {
                if (actorList[i]->getActorLane() == cur_lane && actorList[i]->getY() >= curXmax && actorList[i]->isCAW() == true)
                    top = actorList[i];
            }
            if (gr->getActorLane() == cur_lane && gr->getY() >= curXmax)
                top = gr;
            if (top == nullptr || top->getY() < (VIEW_HEIGHT * (2 / 3)))
            {
                chosenLane = cur_lane;
                chosenY = VIEW_HEIGHT - (SPRITE_HEIGHT / 2);
                chosenSpeed = (getGRSpeed() - randInt(2, 4));
                alreadyChosen = true;
            }
        }
        if (cur_lane == 3)
            cur_lane = 1;
        else
            cur_lane++;
        checkedLanes++;
    }
    if (!alreadyChosen)
    {
        return;
    }
    switch (chosenLane)
    {
    case(1):
    {
        Actor* zc1 = new ZombieCab(this, ROAD_CENTER - ROAD_WIDTH / 3, chosenY, chosenSpeed, 0);
        addActor(zc1);
        break;
    }
    case(2):
    {
        Actor* zc2 = new ZombieCab(this, ROAD_CENTER, chosenY, chosenSpeed, 0);
        addActor(zc2);
        break;
    }
    case(3):
    {
        Actor* zc3 = new ZombieCab(this, ROAD_CENTER + ROAD_WIDTH / 3, chosenY, chosenSpeed, 0);
        addActor(zc3);
        break;
    }
    }
}
//Create a new playing field
int StudentWorld::init()
{
    lostSoulsSaved = 0;
    continueLevel = true;
    currentBonus = 5000;
    gr = new GhostRacer(IID_GHOST_RACER, 128, 32, 90, 4.0, 0, 0, this, 0, 100, 10);
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    for (int i = 0; i < N; i++)
    {
        Actor* a = new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, i * SPRITE_HEIGHT, 0, 2.0, 1, this, -4, 0);
        actorList.push_back(a);
    }
    for (int i = 0; i < N; i++)
    {
        Actor* a = new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, i * SPRITE_HEIGHT, 0, 2.0, 1,this, -4, 0);
        actorList.push_back(a);
    }
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    for (int i = 0; i < M; i++)
    {
        Actor* a = new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3, i * (4*SPRITE_HEIGHT), 0, 2.0,1, this, -4, 0);
        actorList.push_back(a);
        if (i == (M - 1))
        {
            ycLastWBorderline = (i * static_cast<double>((4 * SPRITE_HEIGHT)));
        }
    }
    for (int i = 0; i < M; i++)
    {
        Actor* a = new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT), 0, 2.0, 1,this, -4, 0);
        actorList.push_back(a);
    }
    return GWSTATUS_CONTINUE_GAME;
}
//Handle most things related to actors
int StudentWorld::move()
{
    int cRand;
    int cLevel = getLevel();
    gr->doSomething();
    for (int i = 0; i < actorList.size(); i++)
    {
        if(actorList[i]->isAlive())
            actorList[i]->doSomething();
        if (gr->isAlive() == false)
            return GWSTATUS_PLAYER_DIED;
        if (completedLevel())
        {
            increaseScore(currentBonus);
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
        if (!continueLevel)
        {
            return GWSTATUS_PLAYER_DIED;
        }
    }
    vector<Actor*>::iterator it = actorList.begin();
    //Delete all dead actors
    for (; it != actorList.end();)
    {
        bool maybeAlive = (*it)->isAlive();
        if (!maybeAlive)
        {
            delete (*it);
            it = actorList.erase(it);
        }
        else
            it++;
    }
    //Add new borderlines
    double new_border_y = static_cast<double>(VIEW_HEIGHT - SPRITE_HEIGHT);
    ycLastWBorderline = (ycLastWBorderline - 4 - (gr->getVSpeed()));
    double delta_y = new_border_y - (ycLastWBorderline);
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    if (delta_y >= SPRITE_HEIGHT)
    {
        Actor* a = new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE,new_border_y, 0, 2.0, 2, this, -4, 0);
        actorList.push_back(a);
        Actor* b = new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, new_border_y, 0, 2.0, 2, this, -4, 0);
        actorList.push_back(b);
    }
    if (delta_y >= static_cast<double>((4 * SPRITE_HEIGHT)))
    {
        Actor* c = new BorderLine(IID_WHITE_BORDER_LINE, (ROAD_CENTER - (ROAD_WIDTH / 2) + (ROAD_WIDTH / 3)), new_border_y, 0, 2.0, 2, this, -4, 0);
        actorList.push_back(c);
        Actor* d = new BorderLine(IID_WHITE_BORDER_LINE, (ROAD_CENTER + (ROAD_WIDTH / 2) - (ROAD_WIDTH / 3)), new_border_y, 0, 2.0, 2, this, -4, 0);
        actorList.push_back(d);
        ycLastWBorderline = new_border_y;
    }
    //Add most other actors, except zombie cabs by calling the generic function
    possiblyAddActor(max((200 - (cLevel * 10)), 30), 'h');
    possiblyAddActor(max(100 - cLevel * 10, 20), 'z');
    possiblyAddActor(100 + 10 * cLevel, 's');
    possiblyAddActor(max(150 - cLevel * 10, 40), 'o');
    possiblyAddActor(100 + 10 * cLevel, 'w');
    //Add zombie cabs
    int cV = max(100 - cLevel * 10, 20);
    cRand = randInt(0, cV - 1);
    if (cRand == 0)
        tryToAddZC();
    //Update game status text
    updateStatusText();
    currentBonus--;
    return GWSTATUS_CONTINUE_GAME;
}
//Clean up all of the actors
void StudentWorld::cleanUp()
{
    delete gr;
    while (actorList.size() != 0)
    {
        delete actorList[0];
        actorList.erase(actorList.begin());
    }
}
//Get rid of this world
StudentWorld::~StudentWorld()
{
    cleanUp();
}
