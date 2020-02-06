#include <windows.h>
#include <cstdio>
#include <list>

#include "DiceInvadersLib.h"
#include "IGameObj.h"
#include "AliensBlock.h"
#include "GameHUD.h"

// Window resolution
#define RESX 640
#define RESY 480

// Score for each alien destroyed
#define SINGLE_SCORE 50

int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	LPSTR commandLine,
	int commandShow)
{
	// Init the main window
	DiceInvadersLib::getInstance().init(RESX, RESY);

	/* ---- Init all the variables used in the update cycle ----- */

	// starting position of player ship
	float horizontalPosition = RESX/2;
	// starting position (vertical) of rocket
	float verticalPosition = 0;
	// horizzontal cordinate for the rocket
	float rockDir  = 0;
	// key pressed by the user
	IDiceInvaders::KeyStatus keys;
	DiceInvadersLib::getInstance().get()->getKeyStatus(keys);
	// When a roket is shoot
	bool shoot     = false;
	// When a bomb is released
	bool bombFreq = false;
	// Boolean flag to check when the rocke, the player ot the ground is hit by aliens
	bool rocketHit = false;
	bool playerHit = false;
	bool groundHit = false;
	// If the player lost all his lifes
	bool gameOver  = false;
	// Time variamble to set the player movment speed
	float newTime  = 0.0f;
	float lastTime = DiceInvadersLib::getInstance().get()->getElapsedTime();
	float move     = 0.0f;
	// Total score of the player
	int totScore   = 0;
	// Initial level
	int level      = 1;

	// Init all the game objects
	Player* user       = new Player();
	AliensBlock* block = new AliensBlock(RESX, RESY, level);
	Rocket* rocket     = 0;
	Bomb* bomb         = 0;
	// Init the HUD to draw the texts
	GameHUD* HUD       = new GameHUD(RESX, RESY);
	
	float verticalBomb = 0;
	
 	while (DiceInvadersLib::getInstance().get()->update())
	{
		if (gameOver)
		{
			// Print the text of game over screen
			HUD->printGameOver(totScore, level);

			// Wait untill the user press the "spacebar" to restart or "esc" to exit the game
			DiceInvadersLib::getInstance().get()->getKeyStatus(keys);
			if (keys.fire)
			{
				// Delete the game objects before create a new one
				delete(bomb);
				delete(rocket);
				delete(user);
				delete(block);

				// reset the variables
				totScore = 0;
				level    = 1;
				gameOver = false;
				shoot    = false;
				bombFreq = false;
				rocket   = 0;
				bomb     = 0;
				rockDir  = 0;
				horizontalPosition = RESX/2;
				verticalPosition   = 0;

				// Draw the player again				
				user = new Player();
				user->pos.x = int(horizontalPosition) - ((user->sizex)/2);
				user->pos.y = RESY - user->sizey;
				user->update();

				// Draw the alien block again				
				block = new AliensBlock(RESX, RESY, level);
				block->update(user, rocket, rocketHit, playerHit, groundHit);
							
				// Just a bit of delay before restart the game
				Sleep(500);
			}
		}
		else // If the player has still lifes
		{
			rocketHit = false;
			playerHit = false;
			groundHit = false;

			// Print the score, the lifes and the level
			HUD->printScore(totScore);
			HUD->printLifes(user->getLifes());
			HUD->printLevel(level);

			// Update the player ship position
			user->pos.x = int(horizontalPosition) - ((user->sizex)/2);
			user->pos.y = RESY - user->sizey;

			// Update the player ship movment speed
			newTime = DiceInvadersLib::getInstance().get()->getElapsedTime();
			move = (newTime - lastTime) * user->getSpeed();	
			lastTime = newTime;
			
			// Get the key pressed by the users
			DiceInvadersLib::getInstance().get()->getKeyStatus(keys);

			// If is pressed one of the movment keys, move the ship and don't let it go outside the window margins
			if (keys.right && horizontalPosition < RESX - (user->sizex/2))
				horizontalPosition += move;
			else if (keys.left && horizontalPosition > (user->sizex/2) + 1)
				horizontalPosition -= move;
			user->update();

			// If is pressed the fire button
			if (keys.fire)
			{
				// If has not been shoot a rocket before
				if (!shoot)
				{
					// Create a rocket at the player ship position
					rocket = new Rocket();
					rockDir = horizontalPosition - float(rocket->sizex/2);
					verticalPosition = RESY - float(rocket->sizey + 22);

					shoot = true;
				}
			}

			// Update the alien block, passing the player ship and the rocket reference,
			// to check if the rocket hits the alien, or the playes is hit by alien ship, or if the aliens reach the bottom
			block->update(user, rocket, rocketHit, playerHit, groundHit);
			
			// If has been shoot a rocket...
			if (shoot)
			{
				// ...check that the rocket is destroyed if it reaches the top of the screen or hits an alien ship
				if (verticalPosition < 0 || rocketHit)
				{					
					delete(rocket);
					rocket = 0;
					shoot = false;
				}

				// ...increase the score if the rocket hits an alien (the score increase with the level)
				if (rocketHit)
					totScore += SINGLE_SCORE*level;

				// ...if the rocket is still intact update its position
				if (rocket != 0)
				{
					rocket->pos.x = int(rockDir);
					rocket->pos.y = int(verticalPosition);
					rocket->update();
					// the vertical position is decreased based on the speed define in the class and the time value calculated before
					verticalPosition -= move*rocket->getSpeed();			
				}
			}

			// If the player ship is hit by an alien decrease its lifes
			if (playerHit)
				user->isHit();
						
			// If no bomb is released, just create a new one
			if (!bombFreq)
			{
				bomb = block->releaseBomb();
				verticalBomb = float(bomb->pos.y);
				bombFreq = true;
			}

			// If a bomb is released...
			if (bombFreq)
			{
				// ...check if this bomb reaches the ground and destroy it
				if (bomb != 0 && verticalBomb >= RESY)
				{									
					delete(bomb);
					bomb=0;
					bombFreq=false;
				}

				// ...check if this bomb hits the player ship and destroy it
				if (bomb != 0 && block->checkBombHit(user, bomb))
				{
					user->isHit();

					delete(bomb);
					bomb=0;
					bombFreq=false;
				}

				// ...if the bomb is still intact update its position
				if (bomb != 0)
				{
					bomb->pos.y = int(verticalBomb);
					bomb->update();

					verticalBomb +=  move*bomb->getSpeed();
				}
			}
			
			// If the alien block reaches the ground or the player finish its lifes the game is over
			if (groundHit || user->getLifes() == 0)								
				gameOver = true;	

			// The alien block is empty, the player has completed a level
			if (block->isEmpty())
			{
				// Delete the game objects before create a new one
				delete(bomb);
				delete(rocket);
				
				// Reset the variable
				shoot    = false;
				bombFreq = false;
				rocket   = 0;
				bomb     = 0;
				rockDir  = 0;
				horizontalPosition = RESX/2;
				verticalPosition   = 0;

				// Pass to the next level
				level++;

				// Draw the player again in the middle
				user->pos.x = int(horizontalPosition) - ((user->sizex)/2);
				user->pos.y = RESY - user->sizey;
				user->update();

				// Draw the alien block again
				delete(block);
				block = new AliensBlock(RESX, RESY, level);
				block->update(user, rocket, rocketHit, playerHit, groundHit);
			}


		} // end if(gameOver)		
	} //end update()
	
	// Delete the game objects
	delete(user);	
	delete(block);
	delete(HUD);
	delete(bomb);
	delete(rocket);	
	
	return 0;
}



