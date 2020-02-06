#include "DiceInvadersLib.h"
#include "IGameObj.h"

Bomb::Bomb()
{
	// Check if the window is set before or throw an exception
	if (DiceInvadersLib::getInstance().isInititialized())
	{
		sprite = DiceInvadersLib::getInstance().get()->createSprite("data/bomb.bmp");
	}
	else
		throw;

	// Set the size of the sprite and the defoult position
	sizex = SPRITE_SIZE;
	sizey = SPRITE_SIZE;
	speed = BOMB_SPEED;
	pos.x = 0;
	pos.y = 0;
}

// Destroy the sprite
Bomb::~Bomb()
{
	sprite->destroy();
}

// Draw the sprite
void Bomb::update()
{
	sprite->draw(pos.x,pos.y);
}

// Return the speed
float Bomb::getSpeed()
{
	return speed;
}