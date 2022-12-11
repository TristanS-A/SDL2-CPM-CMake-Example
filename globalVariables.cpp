//
// Created by tropi on 12/5/2022.
//

#include "loadImages.h"
#include "SDL.h"
#include <algorithm>
#include <SDL_mixer.h>

using namespace std;

//Loads player image and is called im for image
SDL_Surface *im = loadImages("images/left1.png");

//Loads player image
SDL_Surface *playerImage = loadImages("images/left1.png");

//For the walking left animation (IDK how to rotate or flip surfaces)
vector <SDL_Surface *> leftMovement = {loadImages("images/left1.png"), loadImages("images/left2.png")};

//For the walking right animation (IDK how to rotate or flip surfaces)
vector <SDL_Surface *> rightMovement = {loadImages("images/right1.png"), loadImages("images/right2.png")};

//For the walking while grappling left animation (IDK how to rotate or flip surfaces)
vector <SDL_Surface *> grappleWalkL = {loadImages("images/flingWalkLeft1.png"), loadImages("images/flingWalkLeft2.png")};

//For the walking while grappling right animation (IDK how to rotate or flip surfaces)
vector <SDL_Surface *> grappleWalkR = {loadImages("images/flingWalkRight1.png"), loadImages("images/flingWalkRight2.png")};

//To keep track of walking animation frames
int walkIndex;

//To tell what direction the player was heading in after stopping moving
bool saveDir = false;

//Loads player getting damaged left image
SDL_Surface *playerDamagedL = loadImages("images/hurtImageL.png");

//Loads player getting damaged right image
SDL_Surface *playerDamagedR = loadImages("images/hurtImageR.png");

//Loads player getting damaged left image
SDL_Surface *grapplingImageR = loadImages("images/flingRight.png");

//Loads player getting damaged right image
SDL_Surface *grapplingImageL = loadImages("images/flingLeft.png");

//To give a cool down for damaging the player
int damageCoolDown = 0;

//To play death animation at specific speed
int deathCurrTime = static_cast<int>(SDL_GetTicks());
int deathPrevTime = 0;

//To play death animation at specific speed
int walkingCurrTime = static_cast<int>(SDL_GetTicks());
int walkingPrevTime = 0;

//Bool for if grappling hook is shooting out
bool shoot = false;

//Bool for if the grappling hook is retracting
bool retrac;

//Iteration variable to keep track of grappling hook vector indexes as it shoots and retracts
int s;

//Bool for if the grappling hook collides with a rect
bool hit = false;

//Ints holding x and y velocity for each grappling hook piece
int ghPieceVelX = 0;
int ghPieceVelY = 0;

//Jump variable to tell when the character can jump
bool jump = false;

//To tell if the player is sprinting
bool sprint = false;

//Sets up key presses
const Uint8 *keystates = SDL_GetKeyboardState(nullptr);

//Vector for holding exit chest surfaces
vector<SDL_Surface *> chestSurfs = {loadImages("images/chest1.png"), loadImages("images/chest2.png")};

//To tell if the player is leaving the level after getting the thing
bool exiting = false;

//So the jump sound doesn't play when you enter a level and so it can tell if you are about to enter a level
bool readyToEnter = false;

Mix_Music *titleMusic;
Mix_Music *caveMusic;
Mix_Chunk *hitSound;
Mix_Chunk *chest;
Mix_Chunk *enemiehit;
Mix_Chunk *jumpSound;
Mix_Chunk *woossh;
Mix_Chunk *explosion;
Mix_Chunk *door;
