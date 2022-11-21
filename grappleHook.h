//
// Created by tropi on 11/21/2022.
//


#ifndef MYPROJECT_GRAPPLEHOOK_H
#define MYPROJECT_GRAPPLEHOOK_H

#include <iostream>
#include <algorithm>
#include "SDL.h"

using namespace std;

void shooting(int &s, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int ghPieceVelY,
              int ghPieceVelX, bool &hit, bool &shoot, bool &retrac, int &track, SDL_Surface *test,
              SDL_Rect &squareRect);

void retracting(int &s, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int ghPieceVelY,
                int ghPieceVelX, bool &hit, bool &shoot, bool &retrac, int &track, SDL_Surface *test,
                SDL_Rect &squareRect, int &yVel, int &xVel, bool mouseUp);

#endif //MYPROJECT_GRAPPLEHOOK_H
