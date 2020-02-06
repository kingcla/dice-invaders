
// Class to draw the text on the screen
class GameHUD
{
public:
	GameHUD(int resx, int resy);
	~GameHUD();
	void printScore(int score);
	void printLifes(int lifes);
	void printLevel(int level);
	void printGameOver(int score, int level);
private:
	int width;
	int height;
};

