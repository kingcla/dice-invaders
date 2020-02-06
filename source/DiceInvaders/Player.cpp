#include "DiceInvadersLib.h"
#include "IGameObj.h"

Player::Player()
{
	// Check if the window is set before or throw an exception
	if (DiceInvadersLib::getInstance().isInititialized())
		sprite = DiceInvadersLib::getInstance().get()->createSprite("data/player.bmp");
	else
		throw;

	// Set the size of the sprite and the defoult position
	sizex = SPRITE_SIZE;
	sizey = SPRITE_SIZE;
	nLife  = LIFES;
	speed  = PLY_SPEED;
	pos.x = 0;
	pos.y = 0;
}

// Destroy the sprite
Player::~Player(void)
{
	sprite->destroy();
}

// If it is hit decrease the lifes by one
void Player::isHit()
{
	if (nLife > 0)
		nLife--;
}

// Return the lifes
int Player::getLifes()
{
	return nLife;
}

// Return the speed
float Player::getSpeed()
{
	return speed;
}
// Draw the sprite
void Player::update()
{
	sprite->draw(pos.x,pos.y);
}