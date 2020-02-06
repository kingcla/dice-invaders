#ifndef IGAMEOBJ_H
#define IGAMEOBJ_H

#include "DiceInvaders.h"

#define SPEED 3.0f       // Rocket speed
#define PLY_SPEED 160.0f // Player movment speed
#define LIFES 3          // Number of initial player lifes
#define BOMB_SPEED 1.5f  // Bomb falling speed

// GameObject interface
struct IGameObj
{
	// The object has a position
	struct Position
	{
		int x;
		int y;
	};

	// function to update the object
	virtual void update() = 0;

	// The object has a sprite texture
	ISprite* sprite;
	Position pos;
	int sizex;
	int sizey;
};

// Class for the rocket object
class Rocket : public IGameObj
{
public:
	Rocket();
	~Rocket();
	void update();
	float getSpeed();
private:
	float speed;
};

// Class for player object
class Player : public IGameObj
{
public:
	Player();
	~Player();
	void update();
	void isHit();
	int getLifes();
	float Player::getSpeed();
private:
	int nLife;
	float speed;
};

// Class for the alien object
class Alien : public IGameObj
{
public:

	enum AlienType
	{
		RED,
		GREEN
	};

	Alien(AlienType type);
	~Alien();
	void update();
};

// Class for the bomb object
class Bomb : public IGameObj
{
public:
	Bomb();
	~Bomb();
	void update();
	float getSpeed();
private:
	float speed;
};

#endif