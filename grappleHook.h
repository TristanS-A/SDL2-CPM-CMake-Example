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
void shooting(vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int &track, SDL_Surface *test,
              vector<SDL_Rect> &hitObjects, vector<SDL_Rect> possibleHit, vector<bool> hitTest, int &sideOffsetY, int &sideOffsetX, int &yVel, int &xVel, bool &hitEnemie);

//Function for retracting the grappling hook
void retracting(vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int &track, SDL_Surface *test,
                int &yVel, int &xVel, bool mouseUp, int &sideOffsetY,
                vector<SDL_Rect> &hitObjects, int &sideOffsetX, bool &hitEnemie);

#endif //MYPROJECT_GRAPPLEHOOK_H
