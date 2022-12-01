//
// Created by tropi on 11/26/2022.
//

#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "roomSwitch.h"

using namespace std;

//Function for switching rooms
bool switchRooms(vector<SDL_Rect> &currRects, vector<SDL_Rect> &nextRects, vector<SDL_Surface *> &surfsNext, vector<SDL_Surface *> &surfsCurr, vector<SDL_Rect> enemieRects, vector <SDL_Surface *> enemieSurf, vector<SDL_Rect> nextEnemieRects, vector<SDL_Surface *> nextEnemieSurfs, vector<SDL_Rect> obsRects, vector<SDL_Surface *> obsSurfs, SDL_Rect &imRect, int &yOffset, int &xOffset, int constDis, int speed, SDL_Surface* test, SDL_Surface * im){

    //Doesn't return true until the rooms have fully switched, where the current stage has been moved off screen
    if (yOffset != -speed || xOffset != -speed) {

        //Sets the offset that the y coordinate will be moved by
        if (yOffset != -speed) {
            yOffset -= speed;
        }

        //Sets the offset that the y coordinate will be moved by
        if (xOffset != -speed){
            xOffset -= speed;
        }

        //Moves the player with the screen
        if (xOffset != -speed) {
            imRect.x -= speed;
        }
        if (yOffset != -speed) {
            imRect.y -= speed;
        }

        //Placeholder so imRect doesn't get moved when blitted
        SDL_Rect placeH = imRect;

        //Blits character since character only blits when the rooms are not transitioning
        SDL_BlitSurface(im, nullptr, test, &placeH);

        SDL_Rect tempCurrEnemie;

        for (int j = 0; j < enemieRects.size(); j++){
            //Sets placeholder to rect info
            tempCurrEnemie = enemieRects[j];

            //Applies offset to placeholder
            if (xOffset != -speed) {
                tempCurrEnemie.x = tempCurrEnemie.x - constDis + xOffset;
            }
            if (yOffset != -speed) {
                tempCurrEnemie.y = tempCurrEnemie.y - constDis + yOffset;
            }

            //Blits rect object with offset
            if (xOffset != -speed || yOffset != -speed) {
                SDL_BlitSurface(enemieSurf[j], &tempCurrEnemie, test, &tempCurrEnemie);
            }
        }

        SDL_Rect tempNextEnemie;

        for (int e = 0; e < nextEnemieRects.size(); e++){
            //Sets placeholder to rect info
            tempNextEnemie = nextEnemieRects[e];
            if (xOffset != -speed) {
                tempNextEnemie.x = tempNextEnemie.x + xOffset;
            }
            if (yOffset != -speed) {
                tempNextEnemie.y = tempNextEnemie.y + yOffset;
            }

            //Blits rect object with offset
            if (xOffset != -speed || yOffset != -speed) {
                SDL_BlitSurface(nextEnemieSurfs[e], &tempNextEnemie, test, &tempNextEnemie);
            }
        }

        for (int h = 0; h < obsRects.size(); h++){

        }

        //Placeholder for the current room rect objects, so they don't change when getting blit
        SDL_Rect tempCurr;

        for (int p = 0; p < currRects.size(); p++) {

            //Sets placeholder to rect info
            tempCurr = currRects[p];

            //Applies offset to placeholder
            if (xOffset != -speed) {
                tempCurr.x = tempCurr.x - constDis + xOffset;
            }
            if (yOffset != -speed) {
                tempCurr.y = tempCurr.y - constDis + yOffset;
            }

            //Blits rect object with offset
            if (xOffset != -speed || yOffset != -speed) {
            SDL_BlitSurface(surfsCurr[p], &currRects[p], test, &tempCurr);
            }
        }

        //Placeholder for the next room rect objects, so they don't change when getting blit
        SDL_Rect temp;

        for (int t = 0; t < nextRects.size(); t++) {

            //Sets placeholder to rect info
            temp = nextRects[t];
            if (xOffset != -speed) {
                temp.x = temp.x + xOffset;
            }
            if (yOffset != -speed) {
                temp.y = temp.y + yOffset;
            }

            //Blits rect object with offset
            SDL_BlitSurface(surfsNext[t], &nextRects[t], test, &temp);
        }
        return false;
    } else {
        return true;
    }

}
