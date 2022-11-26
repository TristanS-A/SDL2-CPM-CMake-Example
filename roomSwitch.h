//
// Created by tropi on 11/26/2022.
//

#ifndef MYPROJECT_ROOMSWITCH_H
#define MYPROJECT_ROOMSWITCH_H

#include <iostream>
#include "SDL.h"
#include <algorithm>

using namespace std;

bool switchRooms(vector<SDL_Rect *> &currRects, vector<SDL_Rect *> &nextRects, vector<SDL_Surface *> &surfs, SDL_Rect &imRect, int &yOffset, int &xOffset, int speed, SDL_Surface* test);

#endif //MYPROJECT_ROOMSWITCH_H
