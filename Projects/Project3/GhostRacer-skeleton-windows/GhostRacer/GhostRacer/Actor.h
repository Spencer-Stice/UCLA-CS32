#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
class StudentWorld;
class GhostRacer;



// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, int startDirection, double size, int depth, StudentWorld* SW, int vspeed, int hspeed)
		:GraphObject(imageID, startX, startY, startDirection, size, depth)
	{
		m_direction = startDirection;
		m_depth = depth;
		m_SW = SW;
		m_vertSpeed = vspeed;
		m_horSpeed = hspeed;
	};
	virtual void doSomething() = 0;
	bool isAlive();
	double getVSpeed();
	double getHSpeed();
	void setDead();
	virtual bool isAffectedbySpray();
	int getActorLane();
	virtual void beSprayed();
	double getRelVSpeed();
	void playSound(int i);
	void setVSpeed(double s);
	void setHSpeed(double s);
	virtual bool move(double vS, double hS);
	virtual bool isCAW();
	StudentWorld* getWorld();
	virtual ~Actor();
private:
	double m_direction;
	int m_depth;
	StudentWorld* m_SW = nullptr;
	bool m_isAlive = true;
	double m_vertSpeed;
	double m_horSpeed;
};

class BorderLine : public Actor
{
public:
	BorderLine(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* SW, int vspeed, int hspeed)
		:Actor(imageID, startX, startY, startDirection, size, depth, SW, vspeed, hspeed) {
	};
	virtual void doSomething();
	virtual bool isCAW();
	virtual ~BorderLine() {};
private:
};

class Agent : public Actor
{
public:
	Agent(int imageID, double startX, double startY, int dir, double size, int vSpeed, int hSpeed, StudentWorld* sw, int depth, int health)
		:Actor(imageID, startX, startY, dir, size, depth, sw, vSpeed, hSpeed)
	{
		m_health = health;
	}
	virtual bool damage(int d, int s);
	void beHealed(int a);
	int getHealth();
private:
	int m_health;
	int m_deathNoise;
};

class GhostRacer : public Agent
{
public:
	GhostRacer(int ImageID, int StartX, int StartY, int StartDirection, double Size, int vSpeed,int hspeed, StudentWorld* SW, int Depth, int Health, int HW)
		:Agent(ImageID, StartX, StartY, StartDirection, Size, vSpeed, hspeed, SW, Depth, Health ) 
	{
		m_holyWaterLeft = HW;
	}
	virtual void doSomething();
	virtual void move();
	void hitOilSlick();
	virtual void damage(int a);
	int getSprays();
	void incHW(int a);
	virtual ~GhostRacer() {};
private:
	int m_holyWaterLeft;
};

class Ped : public Agent
{
public:
	Ped(int imageID, double startX, double startY, int dir, double size, int vSpeed, int hSpeed, StudentWorld* sw, int depth, int health, int MPD)
		:Agent(imageID, startX, startY, dir, size, vSpeed, hSpeed, sw, depth, health) {
		m_MPD = MPD;
	};
	virtual bool isAffectedbySpray();
	void useMPD();
	void changeMPD(int a);
private:
	int getMPD();
	void setMPD(int a);
	int m_MPD;
};

class HumanPed : public Ped
{
public:
	HumanPed(int imageID, int startX, int startY, int sDirection, double size, int depth, int vSpeed, int hSpeed, int HP, StudentWorld* sw, int MPD)
		:Ped(imageID, startX, startY, sDirection, size, vSpeed, hSpeed, sw, depth, HP, MPD ) {};
	virtual void doSomething();
	virtual void beSprayed();
private:
};

class ZombiePed : public Ped
{
public:
	ZombiePed(int imageID, int startX, int startY, int sDirection, double size, int depth, int MPD, int vSpeed, int hSpeed, StudentWorld* sw)
		:Ped(imageID, startX, startY, sDirection, size, vSpeed, hSpeed, sw, depth, 2, MPD) {};
	virtual void doSomething();
	virtual void damage(int d);
	virtual void beSprayed();
private:
	int tickTillGrunt = 0;
};

class HolyWaterProjectile : public Actor
{
public:
	HolyWaterProjectile(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw)
		:Actor(imageID, startX, startY, dir, size, depth, sw, 0, 0) {};
	virtual bool isCAW();
	virtual void doSomething();
private:
	int maxTravelDistance = 160;
	int totalDistanceMoved = 0;
};

class GRActivatedObject : public Actor
{
public:
	GRActivatedObject(int imageID, double x, double y, double size, int dir, StudentWorld* sw)
		:Actor(imageID, x, y, dir, size, 2, sw, -4, 0) {};
	virtual bool isCAW();
	virtual void doSomething(int so, int sc);
	virtual void doActivity() = 0;
private:

};

class SoulGoodie : public GRActivatedObject
{
public:
	SoulGoodie(StudentWorld* sw, double x, double y)
		:GRActivatedObject(IID_SOUL_GOODIE, x, y, 4.0, 0, sw) {};
	virtual void doSomething();
	void doActivity();
private:

};

class OilSlick : public GRActivatedObject
{
public:
	OilSlick(StudentWorld* sw, double x, double y)
		:GRActivatedObject(IID_OIL_SLICK, x, y, randInt(2, 5), 0, sw) {};
	virtual void doSomething();
	virtual void doActivity();
private:

};

class HolyWaterGoodie : public GRActivatedObject
{
public:
	HolyWaterGoodie(StudentWorld* sw, double x, double y)
		:GRActivatedObject(IID_HOLY_WATER_GOODIE, x, y, 2.0, 90, sw) {};
	virtual void doSomething();
	virtual bool isAffectedbySpray();
	virtual void beSprayed();
	void doActivity();
private:

};

class HealingGoodie : public GRActivatedObject
{
public:
	HealingGoodie(StudentWorld* sw, double x, double y)
		:GRActivatedObject(IID_HEAL_GOODIE, x, y, 1.0, 0, sw) {};
	void doSomething();
	virtual bool isAffectedbySpray();
	virtual void beSprayed();
	void doActivity();
private:

};

class ZombieCab : public Agent
{
public:
	ZombieCab(StudentWorld* sw, double x, double y, int vSpeed, int hSpeed)
		:Agent(IID_ZOMBIE_CAB, x, y, 90, 4.0, vSpeed, hSpeed, sw, 0, 3) {};
	virtual void doSomething();
	virtual bool isAffectedbySpray();
	virtual void beSprayed();
private:
	int planLength = 0;
	bool hasDamagedGR = false;
};


#endif // ACTOR_H_
