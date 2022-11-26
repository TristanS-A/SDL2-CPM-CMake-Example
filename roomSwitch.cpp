//
// Created by tropi on 11/26/2022.
//

#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "roomSwitch.h"

using namespace std;

bool switchRooms(vector<SDL_Rect *> &currRects, vector<SDL_Rect *> &nextRects, vector<SDL_Surface *> &surfs, SDL_Rect &imRect, int &yOffset, int &xOffset, int speed, SDL_Surface* test){

    if (yOffset != 0) {

        yOffset -= speed;

        imRect.x -= speed;
        imRect.y -= 0;

        for (auto &currRect: currRects) {
            currRect->x -= speed;
            currRect->y -= 0;
        }

        for (int t = 0; t < nextRects.size(); t++) {
            nextRects[t]->x -= speed;
            nextRects[t]->y += 0;

            //This is a placeholder so that the actual rect position does not get changed by the blit function
            SDL_Rect placeHolder = *nextRects[t];

            SDL_BlitSurface(surfs[t], nextRects[t], test, &placeHolder);
        }
        return false;
    } else {
        return true;
    }

}
