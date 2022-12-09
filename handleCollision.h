//
// Created by tropi on 12/4/2022.
//

#ifndef MYPROJECT_HANDLECOLLISION_H
#define MYPROJECT_HANDLECOLLISION_H

#include "SDL.h"
#include <algorithm>
using namespace std;

void handleCollision(SDL_Rect &collideRects, SDL_Rect &rectToMove, int &yVel, int &xVel, bool &d);

#endif //MYPROJECT_HANDLECOLLISION_H
