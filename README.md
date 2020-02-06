# dice-invaders

Simple "Space Invaders" clone written in C++.

![demo](https://github.com/kingcla/dice-invaders/blob/master/demo/screen.png)

## Development

To develop the game Visual Studio 2010 was initially used. The project was recently upgraded for Visual Studio 2019.

Once analysed all the requirements I started to create an interface for all the game objects which have a sprite texture that has to be updated every tick.

All these game objects will have a position coordinates (x, y) and the size of the sprite. I decide to encapsulate the information of position and the sprite to have flexibility in case it will need to change the internal logic in the future.

Afterwards, I separated the “DiceInvadersLib” class from the Main file and I made it a singleton due to make possible use its functions from different class and without create multiple instances of it.

The player ship and its rocket are both managed into the main cycle, while I decide to separate the logic of alien ships since them will be organized in a group and not controlled separately.

Finally, I created the graphical interface, drawing messages, implementing levels (and the difficulty) and let the user decide to continue to play by restarting the level.

That was an interesting experiment with C++ and STL. Quite a steep learning curve but worth the time to understand better the dynamics of a game engine basics.

## Challenges

The most interesting challenges I have faced were inside the Alien Block. First I had to find a fast and not memory-expensive way to cycle inside the matrix and check the collision with other game object, so I decided to use the function “update” with specific parameter to check all the collision with just one cycle. After, I had to face how to move the entire block in a fluid way. That was one big problem. I solved it by creating the function “move” where I update the movement (to the left or to the right depends of which was touched last) independently to the frame rate.

The last challenge was about the bomb and the decision of releasing it just more than one time. I have finally decided to just keep one and increase the falling speed.

## Play

If you want play the game, the executable version for Windows is available in the releases tab.
