#include "GameHUD.h"
#include "DiceInvadersLib.h"
#include <sstream>

GameHUD::GameHUD(int resx, int resy)
{
	// Check if the window is set before or throw an exception
	if (DiceInvadersLib::getInstance().isInititialized())
	{
		width  = resx;
		height = resy;
	}
	else
		throw;
}

GameHUD::~GameHUD(void)
{
}

void GameHUD::printScore(int score)
{
	// init the string space
	char mesg[100];	
	//std::strcpy(mesg,"Score: ");
	strcpy_s(mesg,"Score: ");
	std::stringstream ss;
	// add number to the stream
    ss << score;
    //std::strcat(mesg, ss.str().c_str());
	strcat_s(mesg, ss.str().c_str());

	// Draw the text in the window
	DiceInvadersLib::getInstance().get()->drawText(10, 10, mesg);
}

void GameHUD::printLifes(int lifes)
{
	// init the string space
	char mesg[80];	
	//std::strcpy(mesg,"Lifes: ");
	strcpy_s(mesg,"Lifes: ");
	std::stringstream ss;
	// add number to the stream
    ss << lifes;
    //std::strcat(mesg, ss.str().c_str());
	strcat_s(mesg, ss.str().c_str());

	// Draw the text in the window
	DiceInvadersLib::getInstance().get()->drawText(10, 30, mesg);
}

void GameHUD::printLevel(int level)
{
	// init the string space
	char mesg[80];	
	int left = 0;
	//std::strcpy(mesg,"Level: ");
	strcpy_s(mesg,"Level: ");
	std::stringstream ss;
	// add number to the stream
    ss << level;
    //std::strcat(mesg, ss.str().c_str());
	strcat_s(mesg, ss.str().c_str());

	// calc the space from the right side of the window
	left = (std::strlen(mesg) + 60);

	// Draw the text in the window
	DiceInvadersLib::getInstance().get()->drawText(width - left, 10, mesg);
}

void GameHUD::printGameOver(int score, int level)
{
	// init the string space
	char mesg[150];	
	std::stringstream ss1;
	std::stringstream ss2;

	strcpy_s(mesg,"Level: ");
	
	// add number to the stream
    ss1 << level;
	strcat_s(mesg, ss1.str().c_str());

	strcat_s(mesg, " - Score: ");

	// add number to the stream
    ss2 << score;
	strcat_s(mesg, ss2.str().c_str());

	// Print the screen of game over
	DiceInvadersLib::getInstance().get()->drawText(width/2 - 25, height/2, "Game Over!");
	DiceInvadersLib::getInstance().get()->drawText(width/2 - 85, height/2 + 50, mesg);
	DiceInvadersLib::getInstance().get()->drawText(width/2 - 85, height/2 + 70, "Press \"spacebar\" to restart");
}