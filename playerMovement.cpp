//
// Created by tropi on 11/18/2022.
//

#include "playerMovement.h"
#include <SDL.h>

void playerMovement(bool jump, bool up, bool left, bool right, bool down, int yVel, int xVel, SDL_Rect imRect){

    //Character control
    if (up && jump) {
        yVel += 30;
        jump = false;
        imRect.y -= yVel;
    }
    if (left) {
        xVel = 5;
    }
    if (right) {
        xVel = -5;
    }
    if (down) {
        yVel -= 5;
    }
}
