//
// Created by tropi on 11/21/2022.
//


#ifndef MYPROJECT_GRAPPLEHOOK_H
#define MYPROJECT_GRAPPLEHOOK_H

#include <iostream>
#include <algorithm>
#include "SDL.h"

using namespace std;

//Function for shooting the grappling hook
void shooting(int &s, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int ghPieceVelY,
              int ghPieceVelX, bool &hit, bool &shoot, bool &retrac, int &track, SDL_Surface *test,
              vector<SDL_Rect> &hitObjects, int &sideOffsetY, int &sideOffsetX);

//Function for retracting the grappling hook
void retracting(int &s, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int &ghPieceVelY,
                int &ghPieceVelX, bool &hit, bool &shoot, bool &retrac, int &track, SDL_Surface *test,
                SDL_Rect &squareRect, int &yVel, int &xVel, bool mouseUp, int &sideOffsetY,
                vector<SDL_Rect> &hitObjects, int &sideOffsetX);

#endif //MYPROJECT_GRAPPLEHOOK_H
