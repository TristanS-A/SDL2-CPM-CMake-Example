//
// Created by tropi on 11/18/2022.
//

#ifndef MYPROJECT_PLAYERMOVEMENT_H
#define MYPROJECT_PLAYERMOVEMENT_H

#include <iostream>
#include "SDL.h"

using namespace std;

void playerMovement(bool up, bool left, bool right, bool down, int &yVel, int &xVel, SDL_Rect &imRect);

#endif //MYPROJECT_PLAYERMOVEMENT_H
