#include "DiceInvadersLib.h"
#include "IGameObj.h"

Rocket::Rocket()
{
	// Check if the window is set before or throw an exception
	if (DiceInvadersLib::getInstance().isInititialized())
		sprite = DiceInvadersLib::getInstance().get()->createSprite("data/rocket.bmp");
	else
		throw;

	// Set the size of the sprite and the defoult position
	sizex = SPRITE_SIZE;
	sizey = SPRITE_SIZE;
	speed = SPEED;
	pos.x = 0;
	pos.y = 0;	
}

// Destroy the sprite
Rocket::~Rocket()
{
	sprite->destroy();
}

// Return the speed
float Rocket::getSpeed()
{
	return speed;
}

// Draw the sprite
void Rocket::update()
{
	sprite->draw(pos.x,pos.y);
}
