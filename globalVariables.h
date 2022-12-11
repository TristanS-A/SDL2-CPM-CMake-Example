//
// Created by tropi on 12/5/2022.
//

#ifndef MYPROJECT_GLOBALVARIABLES_H
#define MYPROJECT_GLOBALVARIABLES_H

#include "SDL.h"
#include <SDL_mixer.h>
#include <utility>
#include <algorithm>

using namespace std;

extern SDL_Surface *im;

extern SDL_Surface *playerImage;

extern vector <SDL_Surface *> leftMovement;

extern vector <SDL_Surface *> rightMovement;

extern vector <SDL_Surface *> grappleWalkL;

extern vector <SDL_Surface *> grappleWalkR;

extern int walkIndex;

extern bool saveDir;

extern SDL_Surface *playerDamagedL;

extern SDL_Surface *playerDamagedR;

extern SDL_Surface *grapplingImageR;

extern SDL_Surface *grapplingImageL;

extern int damageCoolDown;

extern int deathCurrTime;
extern int deathPrevTime;

extern int walkingCurrTime;
extern int walkingPrevTime;

extern bool shoot;
extern bool retrac;

extern int s;

extern bool hit;

extern int ghPieceVelX;
extern int ghPieceVelY;

extern bool jump;

extern bool sprint;

extern const Uint8 *keystates;

extern vector<SDL_Surface *> chestSurfs;

extern bool exiting;

extern bool readyToEnter;

extern Mix_Music *titleMusic;
extern Mix_Music *caveMusic;
extern Mix_Chunk *hitSound;
extern Mix_Chunk *chest;
extern Mix_Chunk *enemiehit;
extern Mix_Chunk *jumpSound;
extern Mix_Chunk *woossh;
extern Mix_Chunk *explosion;
extern Mix_Chunk *door;

#endif //MYPROJECT_GLOBALVARIABLES_H
