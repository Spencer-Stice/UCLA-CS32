#include "Actor.h"
#include "StudentWorld.h"
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Pseudocode for complicated functions can be found in my report

/////////////////////////////////////////////////////////////
//Actor functions
bool Actor::move(double vS, double hS)
{
	int nX = getX() + hS;
	int nY = getY() + vS;
	moveTo(nX, nY);
	if (nX < 0 || nY < 0 || nX > VIEW_WIDTH || nY > VIEW_HEIGHT)
	{
		setDead();
		return false;
	}
	return true;
}
void Actor::beSprayed() {};

double Actor::getRelVSpeed()
{
	return (getVSpeed() - (getWorld()->getGRSpeed()));
}

bool Actor::isAffectedbySpray() { return false; };

bool Actor::isCAW()
{
	return true;
}

StudentWorld* Actor::getWorld()
{
	return m_SW;
};
bool Actor::isAlive() { return m_isAlive; };
double Actor::getVSpeed() { return m_vertSpeed; };
double Actor::getHSpeed() { return m_horSpeed; };
void Actor::setVSpeed(double s) { m_vertSpeed = s; };
void Actor::setHSpeed(double s) { m_horSpeed = s; };
void Actor::setDead() { m_isAlive = false; };

void Actor::playSound(int i)
{
	getWorld()->playSound(i);
}

int Actor::getActorLane()
{
	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
	if (getX() < LEFT_EDGE + ROAD_WIDTH / 3)
		return 1;
	else if (getX() >= LEFT_EDGE + ROAD_WIDTH / 3 && getX() < RIGHT_EDGE - ROAD_WIDTH / 3)
		return 2;
	else
		return 3;
}

Actor::~Actor() {};

///////////////////////////////////////////////////////////
//Borderline functions


void BorderLine::doSomething()
{
	move(getRelVSpeed(), getHSpeed());
	return;
}

bool BorderLine::isCAW() { return false; };


////////////////////////////////////////////////////////////////
//Agent functions

bool Agent::damage(int d, int s)	//Fix sound
{
	m_health -= d;
	if (m_health <= 0)
	{
		setDead();
		if(s != -100)
			getWorld()->playSound(s);
		return true;
	}
	return false;
}

int Agent::getHealth() { return m_health; };

void Agent::beHealed(int a)
{
	if (a + m_health > 100)
		m_health = 100;
	else
		m_health += a;
}

//////////////////////////////////////////////////////////////
//Ped functions

void Ped::setMPD(int a) { m_MPD = a; };
void Ped::changeMPD(int a) { m_MPD += a; };
int Ped::getMPD() { return m_MPD; };
void Ped::useMPD()
{
	m_MPD--;
	if (m_MPD > 0)
		return;
	int newHS;
	int newHSL = randInt(-3, -1);
	int newHSR = randInt(1, 3);
	int pick = randInt(0, 1);
	if (pick == 1)
	{
		setHSpeed(newHSL);
		newHS = newHSL;
	}
	else
	{
		setHSpeed(newHSR);
		newHS = newHSR;
	}
	m_MPD = randInt(4, 32);
	if (newHS < 0)
		setDirection(180);
	else
		setDirection(0);
}

bool Ped::isAffectedbySpray() { return true; };

/////////////////////////////////////////////////////
//Zombie Ped functions



void ZombiePed::damage(int d)
{
	bool isDead = Agent::damage(d, SOUND_PED_DIE);
	if (!isDead)
	{
		int Rand = randInt(1, 5);
		if (Rand == 1)
		{
			Actor* nH = new HealingGoodie(getWorld(), getX(), getY());
			getWorld()->addActor(nH);
		}
		getWorld()->increaseScore(150);
	}
	else
		playSound(SOUND_PED_HURT);
}
void ZombiePed::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->overlaps(this, getWorld()->getGR()))
	{
		getWorld()->getGR()->damage(5);
		damage(2);
		return;
	}
	int xdToGR = abs(getX() - getWorld()->getGR()->getX());
	if (xdToGR <= 30 && (getY() > getWorld()->getGR()->getY()))
	{
		setDirection(270);
		if (getX() < getWorld()->getGR()->getX())
			setHSpeed(1);
		else if (getX() > getWorld()->getGR()->getX())
			setHSpeed(-1);
		else
			setHSpeed(0);
		tickTillGrunt--;
		if (tickTillGrunt <= 0)
		{
			getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
			tickTillGrunt = 20;
		}
	}
	if (!Actor::move(getRelVSpeed(), getHSpeed()))
		return;
	Ped::useMPD();

}

void ZombiePed::beSprayed()
{
	damage(1);
	if (isAlive())
		playSound(SOUND_PED_HURT);

}

///////////////////////////////////////////////////////////////
//Human ped funtions

void HumanPed::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->overlaps(this, getWorld()->getGR()))
	{
		getWorld()->decLives();
		getWorld()->endLevel();
		return;

	}
	move(getRelVSpeed(), getHSpeed());
	Ped::changeMPD(-1);
	Ped::useMPD();
}

void HumanPed::beSprayed()
{
	setHSpeed(getHSpeed() * -1);
	int cDir = getDirection();
	if (cDir == 0)
		setDirection(180);
	else
		setDirection(0);
	playSound(SOUND_PED_HURT);
}
//////////////////////////////////////////////////////////
//Ghostracer functions


void GhostRacer::doSomething()
{
	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
	int ch;
	if (getX() <= LEFT_EDGE)
	{
		damage(10);
		setDirection(82);
		getWorld()->playSound(SOUND_VEHICLE_CRASH);
	}
	if (getX() >= RIGHT_EDGE)
	{
		damage(10);
		setDirection(98);
		getWorld()->playSound(SOUND_VEHICLE_CRASH);
	}
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case(KEY_PRESS_LEFT):
		{
			if (getDirection() < 114)
			{
				setDirection(getDirection() + 8);
			}
			break;
		}
		case(KEY_PRESS_RIGHT):
		{
			if (getDirection() > 66)
			{
				setDirection(getDirection() - 8);
			}
			break;
		}
		case(KEY_PRESS_UP):
		{
			if (getVSpeed() < 5)
				setVSpeed(1);
			break;
		}
		case(KEY_PRESS_DOWN):
		{
			if (getVSpeed() > -1)
				setVSpeed(-1);
			break;
		}
		case(KEY_PRESS_SPACE):
		{
			if (m_holyWaterLeft >= 1)
			{
				Actor* a = new HolyWaterProjectile(IID_HOLY_WATER_PROJECTILE, (getX() + (SPRITE_HEIGHT * cos(getDirection() * (3.141592653 / 180)))), (getY() + (SPRITE_HEIGHT * sin(getDirection() * (3.141592653 / 180)))), getDirection(), 1.0, 1, getWorld());
				getWorld()->addActor(a);
				playSound(SOUND_PLAYER_SPRAY);
				m_holyWaterLeft--;
			}
		}
		}
	}
	move();
}

void GhostRacer::move()
{
	double max_shift_per_tick = 4.0;
	double delta_x = (cos(getDirection() * (3.141592653/180)) * max_shift_per_tick);
	moveTo((getX() + delta_x), getY());
}

void GhostRacer::damage(int a)
{
	Agent::damage(a, SOUND_PLAYER_DIE);
	if (!isAlive())
		getWorld()->decLives();
}

void GhostRacer::hitOilSlick()
{
	int cRand = randInt(5, 20);
	int rDir = randInt(0, 1);
	if (rDir == 1)
	{
		if (getDirection() - cRand > 60)
		{
			setDirection(getDirection() - cRand);
			return;
		}
	}
	if(getDirection() + cRand < 120)
	{ 
	setDirection(getDirection() + cRand);
	return;
	}
}

void GhostRacer::incHW(int a) { m_holyWaterLeft += a; };
int GhostRacer::getSprays() { return m_holyWaterLeft; };

///////////////////////////////////////////////////////////
//Spray functions

bool HolyWaterProjectile::isCAW() { return false; };
void HolyWaterProjectile::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->sprayFirstActor(this))
	{
		setDead();
		return;
	}
	moveForward(SPRITE_HEIGHT);
	totalDistanceMoved += SPRITE_HEIGHT;
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT)
	{
		setDead();
		return;
	}
	if (totalDistanceMoved >= maxTravelDistance)
		setDead();
	return;
}


/////////////////////////////////////////////////////
//GRActivatedObject funtions

bool GRActivatedObject::isCAW() { return false; };

void GRActivatedObject::doSomething(int so, int sc)
{
	move(getRelVSpeed(), 0);
	if (getWorld()->overlaps(this, getWorld()->getGR()))
	{
		doActivity();
		setDead();
		Actor::playSound(so);
		getWorld()->increaseScore(sc);
	}
}

//////////////////////////////////////////////////
//Soul Goodie functions

void SoulGoodie::doSomething()
{
	GRActivatedObject::doSomething(SOUND_GOT_SOUL, 100);
	setDirection(getDirection() - 10);
}
void SoulGoodie::doActivity()
{
	getWorld()->soulSaved();
}

///////////////////////////////////////////////
//Oil Slick functions

void OilSlick::doSomething()
{
	move(getRelVSpeed(), getHSpeed());
	if (getWorld()->overlaps(this, getWorld()->getGR()))
	{
		playSound(SOUND_OIL_SLICK);
		getWorld()->getGR()->hitOilSlick();
	}
}

void OilSlick::doActivity() { return; };

///////////////////////////////////////
//Holy Water goodie functions

void HolyWaterGoodie::doSomething()
{
	GRActivatedObject::doSomething(SOUND_GOT_GOODIE, 50);
}
void HolyWaterGoodie::doActivity()
{
	getWorld()->getGR()->incHW(10);
}

bool HolyWaterGoodie::isAffectedbySpray() { return true; };
void HolyWaterGoodie::beSprayed()
{
	setDead();
}
/////////////////////////////////
//Healing Goodie functions

void HealingGoodie::doSomething()
{
	GRActivatedObject::doSomething(SOUND_GOT_GOODIE, 250);
}

void HealingGoodie::doActivity()
{
	getWorld()->getGR()->beHealed(10);
}

bool HealingGoodie::isAffectedbySpray() { return true; };
void HealingGoodie::beSprayed()
{
	setDead();
}

///////////////////////////////////
//Zombie cab functions

void ZombieCab::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->overlaps(this, getWorld()->getGR()))
	{
		if (!hasDamagedGR)
		{
			playSound(SOUND_VEHICLE_CRASH);
			getWorld()->getGR()->damage(20);
			if (getX() <= getWorld()->getGR()->getX())
			{
				setHSpeed(-5);
				setDirection(120 + randInt(0, 19));
			}
			else
			{
				setHSpeed(5);
				setDirection(60 - randInt(0, 19));
			}
			hasDamagedGR = true;
		}
	}
	move(getRelVSpeed(), getHSpeed());
	if (getRelVSpeed() > 0)
	{
		if (getWorld()->checkForZombieCabF(this))
		{
			setVSpeed(-.5);
			return;
		}
	}
	else
	{
		if (getWorld()->checkForZombieCabB(this))
		{
			setVSpeed(.5);
			return;
		}
	}
	planLength--;
	if (planLength > 0)
		return;
	planLength = randInt(4, 32);
	setVSpeed(getVSpeed() + randInt(-2, 2));
}

bool ZombieCab::isAffectedbySpray() { return true; };

void ZombieCab::beSprayed()
{
	if(!isAlive())
	{
		int c = randInt(1, 5);
		if (c == 1)
		{
			Actor* a = new OilSlick(getWorld(), getX(), getY());
			getWorld()->addActor(a);
		}
		getWorld()->increaseScore(200);
		return;
	}
	playSound(SOUND_VEHICLE_HURT);

}