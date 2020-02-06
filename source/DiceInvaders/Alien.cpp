#include "DiceInvadersLib.h"
#include "IGameObj.h"

Alien::Alien(AlienType type)
{
	// Check if the window is set before or throw an exception
	if (DiceInvadersLib::getInstance().isInititialized())
	{
		if (type == RED)
			sprite = DiceInvadersLib::getInstance().get()->createSprite("data/enemy1.bmp");
		else if (type == GREEN)
			sprite = DiceInvadersLib::getInstance().get()->createSprite("data/enemy2.bmp");
	}
	else
		throw;

	// Set the size of the sprite and the defoult position
	sizex = SPRITE_SIZE;
	sizey = SPRITE_SIZE;
	pos.x = 0;
	pos.y = 0;
}

// Destroy the sprite
Alien::~Alien()
{
	sprite->destroy();
}

// Draw the sprite
void Alien::update()
{
	sprite->draw(pos.x,pos.y);
}