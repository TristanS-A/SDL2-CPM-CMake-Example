//
// Created by tropi on 11/26/2022.
//

#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "roomSwitch.h"

using namespace std;

bool switchRooms(vector<SDL_Rect *> &currRects, vector<SDL_Rect *> &nextRects, vector<SDL_Surface *> &surfs, SDL_Rect &imRect, int &yOffset, int &xOffset, int speed, SDL_Surface* test){

    if (yOffset != -speed || xOffset != -speed) {

        if (yOffset != -speed) {
            yOffset -= speed;
        }

        if (xOffset != -speed){
            xOffset -= speed;
        }

        if (xOffset != -speed) {
            imRect.x -= speed;
        }
        if (yOffset != -speed) {
            imRect.y -= speed;
        }

        for (auto &currRect: currRects) {
            if (xOffset != -speed) {
                currRect->x -= speed;
            }
            if (yOffset != -speed) {
                currRect->y -= speed;
            }
        }

        for (int t = 0; t < nextRects.size(); t++) {
            if (xOffset != -speed) {
                nextRects[t]->x -= speed;
            }
            if (yOffset != -speed) {
                nextRects[t]->y -= speed;
            }

            //This is a placeholder so that the actual rect position does not get changed by the blit function
            SDL_Rect placeHolder = *nextRects[t];

            SDL_BlitSurface(surfs[t], nextRects[t], test, &placeHolder);
        }
        return false;
    } else {
        return true;
    }

}
