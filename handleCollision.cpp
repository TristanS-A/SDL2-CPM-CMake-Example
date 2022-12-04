//
// Created by tropi on 12/4/2022.
//

#include "handleCollision.h"
#include "SDL.h"
using namespace std;

void handleCollision(SDL_Rect &collideRect, SDL_Rect &rectToMove, int &yVel, int &xVel, bool &d, bool &jump){
    if (SDL_HasIntersection(&collideRect, &rectToMove)){

        //Top side
        if ((rectToMove.y + rectToMove.h - collideRect.y <= fabs(yVel) && yVel <= 0)){
            rectToMove.y = collideRect.y - rectToMove.h;
            yVel = 0;
            d = true;
            jump = true;
        }

            //Bottom side
        else if ((collideRect.y + collideRect.h - rectToMove.y <= fabs(yVel) && yVel >= 0)){
            rectToMove.y = collideRect.y + collideRect.h;
            yVel = 0;
        }

        //Left and Right side
        if ((rectToMove.x + rectToMove.w - collideRect.x <= fabs(xVel) && xVel <= 0) || (collideRect.x + collideRect.w - rectToMove.x <= fabs(xVel) && xVel >= 0)) {
            //Left side
            if (xVel < 0) {
                rectToMove.x = collideRect.x - rectToMove.w;
                xVel = 0;
            }

                //Right side
            else {
                rectToMove.x = collideRect.x + collideRect.w;
                xVel = 0;
            }
        }
    }
}