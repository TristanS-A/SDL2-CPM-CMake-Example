//
// Created by tropi on 12/5/2022.
//

#include "globalVariables.h"
#include "loadImages.h"
#include "SDL.h"

//Loads cat image
SDL_Surface *im = loadImages("images/cat.png");

//Loads player image
SDL_Surface *playerImage = loadImages("images/cat.png");

//Loads player getting damaged image
SDL_Surface *playerDamaged = loadImages("images/playerDamaged.png");

//To give a cool down for damaging the player
int damageCoolDown = 0;

//To play death animation at specific speed
int deathCurrTime = static_cast<int>(SDL_GetTicks());
int deathPrevTime = 0;
