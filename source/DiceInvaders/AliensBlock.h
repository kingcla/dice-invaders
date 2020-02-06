#define ROW         6
#define COL         11
#define CYCLE_SPEED 45.0f
#define SPACE_UP    64
#define SPACE_LEFT  119

// Class that manage the alien block
class AliensBlock
{
public:
	AliensBlock(int width, int height, int difficulty);
	~AliensBlock();

	void update(const Player* p, const Rocket* r, bool &out_isShipHit, bool &out_isPlayerHit, bool &out_isGroundHit);
	Bomb* releaseBomb();
	bool isEmpty();
	bool AliensBlock::checkBombHit(const Player* p, const Bomb* b);
private:
	void move(bool rWall, bool lWall);
	bool checkRocketHit(const Rocket* r, const Alien* a);
	bool checkPlayerHit(const Player* p, const Alien* a);
	int row;
	int col;
	int nAliens;
	int left;
	int up;
	int width;
	int height;
	float speed;
	int direction;
	float lastTime;
	float time;
	int difficulty;
	int sizex;
	int sizey;
	// Instace of the alien matrix
	Alien* matrix[ROW][COL];
};

