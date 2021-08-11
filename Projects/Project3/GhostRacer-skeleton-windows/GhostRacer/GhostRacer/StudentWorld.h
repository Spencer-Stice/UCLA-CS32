#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include <string>
class GhostRacer;
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void endLevel();
    void soulSaved();
    bool checkForZombieCabF(Actor* a);
    bool checkForZombieCabB(Actor* a);
    void addActor(Actor* a);
    bool sprayFirstActor(Actor* a);
    bool overlaps(Actor* first, Actor* second);
    double getGRSpeed();
    GhostRacer* getGR();
    ~StudentWorld();
private:
    void possiblyAddActor(int rand, char type);
    int getSoulsToSave();
    void updateStatusText();
    bool completedLevel();
    void tryToAddZC();
    GhostRacer* gr;
    std::vector<Actor*> actorList;
    double ycLastWBorderline;
    int currentBonus = 5000;
    bool continueLevel = true;
    int lostSoulsSaved = 0;
};

#endif // STUDENTWORLD_H_
