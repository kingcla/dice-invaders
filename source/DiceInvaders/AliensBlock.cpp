#include <vector>

#include "IGameObj.h"
#include "AliensBlock.h"
#include "GameMath.h"
#include "DiceInvadersLib.h"

AliensBlock::AliensBlock(int width, int height, int difficulty)
{
	// Init the rows and columns of the matrix
	row = ROW;
	col = COL;

	// Init the starting position of the block
	left = SPACE_LEFT;
	up   = SPACE_UP;

	// Calculate the number of aliens in the block
	nAliens = row * col;

	// Init the widht and height of the screen
	AliensBlock::width  = width;
	AliensBlock::height = height;

	// Init the direction 
	// 1  from left to right
	// -1 from right to left
	direction = 1;

	// Init the time used to update the position of the block
	time = 0;

	// Get the elapsed time
	lastTime = DiceInvadersLib::getInstance().get()->getElapsedTime();

	// Set the difficulty level
	AliensBlock::difficulty = difficulty;

	// Set the starting speed of moving for the block
	speed = CYCLE_SPEED / difficulty;

	// Init the aliens matrix
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			if ((j+1)%2 == 0) // Alternate row with different alien sprites
				matrix[j][i] = new Alien(Alien::RED);
			else
				matrix[j][i] = new Alien(Alien::GREEN);
		}
	}

	// Init the size of the sprite
	sizex = matrix[0][0]->sizex;
	sizey = matrix[0][0]->sizey;
}

// Destroy all the alien object in the matrix
AliensBlock::~AliensBlock()
{
	// Destroy all the aliens in the matrix
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			if (matrix[j][i] != 0)
			{
				delete(matrix[j][i]);
			}
		}
	}
}

// Release one bomb come from a random alien ship that is still active
Bomb* AliensBlock::releaseBomb()
{
	if (nAliens > 0)
	{
		Bomb* b = new Bomb();
		std::vector<IGameObj::Position> aList;

		// Search all the alien ships still intact in the block
		// and insert its position into the list
		for (int i = 0; i < col; i++)
		{
			for (int j = 0; j < row; j++)
			{
				if (matrix[j][i] != 0)
				{
					aList.push_back(matrix[j][i]->pos);
				}
			}
		}
		
		// Randomly take one position inside the list of active aliens
		int random_index;
		int lowest=0;
		int highest=nAliens-1; 
		int range=(highest-lowest)+1; 
    
        random_index = lowest+int(range*rand()/(RAND_MAX + 1.0)); 

		b->pos = aList[random_index];

		return b;
	}

	return 0;
}

// Update the block
// This function takes the player and rocket references to check if
// an alien ship hits one of them
// moreover return if an alien ship hits the ground
void AliensBlock::update(const Player* p, const Rocket* r, bool &out_isShipHit, bool &out_isPlayerHit, bool &out_isGroundHit)
{
	int x = left;
	int y = up;
	bool bOnRWall = false;
	bool bOnLWall = false;

	// If there are still aliens inside the matrix...
	if (nAliens > 0)
	{
		// ...for each column
		for (int i = 0; i < col; i++)
		{
			// ...for each row
			for (int j = 0; j < row; j++)
			{				
				// If the alien is still intact...
				if (matrix[j][i] != 0)
				{
					// ...check if it hits the rocket
					if (r != 0 && checkRocketHit(r, matrix[j][i]))
					{				
						out_isShipHit = true;
					}
				
					// ...check if it hits the player ship
					if (checkPlayerHit(p, matrix[j][i]))
						out_isPlayerHit = true;

					// ...check if it hits the ground
					if (matrix[j][i]->pos.y + matrix[j][i]->sizey > height)
						out_isGroundHit = true;

					// eventually destroy the alien
					if ((r != 0 && checkRocketHit(r, matrix[j][i])) || checkPlayerHit(p, matrix[j][i]))
					{
						delete(matrix[j][i]);
						matrix[j][i] = 0;  
 						nAliens--;
					}
					else // or if it didn't hit anything update its position
					{ 
						// check if the alien ship is touching the right wall
						if (bOnRWall==false && matrix[j][i]->pos.x + matrix[j][i]->sizex > width)
							bOnRWall=true;					
						// check if the alien ship is touching the left wall
						else if(bOnLWall == false && matrix[j][i]->pos.x < 0)
							bOnLWall=true;

						matrix[j][i]->pos.x = x;
						matrix[j][i]->pos.y = y;
						matrix[j][i]->update();
					}
				}
			
				// Pass to the next row position (size of the sprite + 5px of space between)
				y += sizey + 5;			
			}
			
			// Pass to the next colomn position (size of the sprite + 5px of space between, except the last column)
			x += (i != col - 1)? sizex + 5 : sizex;

			// Reset the space from the top
			y = up;
		}

		// Increase the horizontal and in case the vertical position of the matrix
		move(bOnRWall, bOnLWall);
	}
}

// Is the only function that modify the coordinates of the block
void AliensBlock::move(bool rWall, bool lWall)
{
	// In order to make the movement indipendent from the frame rate 
	// is calculated the time between each update call
	// it is converted in seconds
	float newTime = DiceInvadersLib::getInstance().get()->getElapsedTime();
	time += (newTime - lastTime) * 1000;
	lastTime = newTime;

	// if the time reach the cycle speed...
	if (time >= speed)
	{
		// ...if is hit the right wall put the directon -1
		if(rWall)
		{
			up   += 10;
			direction = -1;
		}
		// ...if is hit the left wall put the directon 1
		else if (lWall)
		{
			up   += 10;
			direction = 1;
		}

		// ...increase or decrease the x coordinate 
		left += direction;
		time = 0;
	}
}

// Return true if the block is empty
bool AliensBlock::isEmpty()
{
	return (nAliens == 0);
}

// Check if the rocket hits an alien ship
bool AliensBlock::checkRocketHit(const Rocket* r, const Alien* a)
{
	// Create rectangles fro the objects
	GameMath::RectangleOperation::rect rocket; 
	GameMath::RectangleOperation::rect alien;

	// Fix the bordes of the rectangles
	rocket.height = r->sizey - 6;
	rocket.width  = r->sizex - 20;
	rocket.x      = r->pos.x + 10;
	rocket.y      = r->pos.y + 6;

	alien.height = a->sizey - 5;
	alien.width  = a->sizex - 8;
	alien.x      = a->pos.x + 4;
	alien.y      = a->pos.y;

	// Return true if there wes a collision
	return GameMath::RectangleOperation::rectOverlap(rocket, alien);
}

// Check if the bomb hits the player ship
bool AliensBlock::checkBombHit(const Player* p, const Bomb* b)
{
	// Create rectangles fro the objects
	GameMath::RectangleOperation::rect player; 
	GameMath::RectangleOperation::rect alien;

	// Fix the bordes of the rectangles
	player.height = p->sizey;
	player.width  = p->sizex;
	player.x      = p->pos.x;
	player.y      = p->pos.y;

	alien.height = b->sizey - 6;
	alien.width  = b->sizex - 20;
	alien.x      = b->pos.x + 10;
	alien.y      = b->pos.y + 6;

	// Return true if there wes a collision
	return GameMath::RectangleOperation::rectOverlap(player, alien);
}

// Check if the player hits an alien ship
bool AliensBlock::checkPlayerHit(const Player* p, const Alien* a)
{
	// Create rectangles fro the objects
	GameMath::RectangleOperation::rect player; 
	GameMath::RectangleOperation::rect alien;

	// Fix the bordes of the rectangles
	player.height = p->sizey;
	player.width  = p->sizex;
	player.x      = p->pos.x;
	player.y      = p->pos.y;

	alien.height = a->sizey - 2;
	alien.width  = a->sizex - 2;
	alien.x      = a->pos.x;
	alien.y      = a->pos.y;

	// Return true if there wes a collision
	return GameMath::RectangleOperation::rectOverlap(player, alien);
}