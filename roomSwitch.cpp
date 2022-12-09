//
// Created by tropi on 11/26/2022.
//

#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "roomSwitch.h"
#include "enemies.h"
#include "globalVariables.h"

using namespace std;

//Function for switching rooms
bool switchRooms(vector<SDL_Rect> &currRects, vector<SDL_Rect> &nextRects, vector<SDL_Surface *> &surfsNext, vector<SDL_Surface *> &surfsCurr, vector<SDL_Rect> enemieRects, vector <SDL_Surface *> enemieSurf, vector<SDL_Rect> nextEnemieRects, vector<SDL_Surface *> nextEnemieSurfs, vector<Enemies> currEnemies, vector<Enemies> nextEnemies, vector<SDL_Rect> obsRects, vector<SDL_Surface *> obsSurfs, vector<SDL_Rect> nextObsRects, vector<SDL_Surface *> nextObsSurfs, SDL_Rect &imRect, int &yOffset, int &xOffset, int constDis, int speed, SDL_Surface* test, SDL_Surface *currBG, SDL_Surface *nextBG, SDL_Rect &g, SDL_Surface *paraBG, int &paraBGx, int &paraBGy){

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

        //Creates rects for the location of the non-parallax backgrounds
        SDL_Rect phBG1 = {0, 0, 0, 0};
        SDL_Rect phBG2 = {0, 0, 0, 0};

        //Moves the room backgrounds
        if (xOffset != -speed) {
            phBG1.x = - constDis + xOffset;
            phBG2.x = xOffset;
        }
        if (yOffset != -speed) {
            phBG1.y = -constDis + yOffset;
            phBG2.y = yOffset;
        }

        //Moves the parallax bg
        if (xOffset != -speed) {
            paraBGx -= speed / 2;
        }
        if (yOffset != -speed) {
            paraBGy -= speed / 2;
        }

        //Sets the location of the parallax bg
        g.x = paraBGx % 1440;
        g.y = paraBGy % 810;

        //Keeps these variables negative so the moving of the parallax bg works.
        if (paraBGy > 0){
            paraBGy = -810;
        }

        //Keeps these variables negative so the moving of the parallax bg works.
        if (paraBGx > 0){
            paraBGy = -1440;
        }

        //Blits parallax background
        SDL_BlitSurface(paraBG, nullptr, test, &g);

        //Blits non-parallax backgrounds for incoming and current room
        SDL_BlitSurface(currBG, nullptr, test, &phBG1);
        SDL_BlitSurface(nextBG, nullptr, test, &phBG2);

        //Placeholder so imRect doesn't get moved when blitted
        SDL_Rect placeH = imRect;

        //Blits character since character only blits when the rooms are not transitioning
        SDL_BlitSurface(im, nullptr, test, &placeH);

        SDL_Rect tempCurrEnemie;

        for (int j = 0; j < enemieRects.size(); j++){
            if (!currEnemies[j].isDead()) {//Sets placeholder to rect info
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
                    SDL_BlitSurface(enemieSurf[j], nullptr, test, &tempCurrEnemie);
                }
            }
        }

        SDL_Rect tempNextEnemie;

        for (int e = 0; e < nextEnemieRects.size(); e++){
            if (!nextEnemies[e].isDead()){//Sets placeholder to rect info
                tempNextEnemie = nextEnemieRects[e];
                if (xOffset != -speed) {
                    tempNextEnemie.x = tempNextEnemie.x + xOffset;
                }
                if (yOffset != -speed) {
                    tempNextEnemie.y = tempNextEnemie.y + yOffset;
                }

                //Blits rect object with offset
                if (xOffset != -speed || yOffset != -speed) {
                    SDL_BlitSurface(nextEnemieSurfs[e], nullptr, test, &tempNextEnemie);
                }
            }
        }

        SDL_Rect tempObs;

        for (int q = 0; q < obsRects.size(); q++){
            //Sets placeholder to rect info
            tempObs = obsRects[q];

            //Applies offset to placeholder
            if (xOffset != -speed) {
                tempObs.x = tempObs.x - constDis + xOffset;
            }
            if (yOffset != -speed) {
                tempObs.y = tempObs.y - constDis + yOffset;
            }

            //This is a placeholder so that the surface blits at the 0, 0 location on the rect, but still only blits inside
            // the w and h of the rect
            SDL_Rect o = {0,0,tempObs.w,tempObs.h};

            //Blits rect object with offset
            if (xOffset != -speed || yOffset != -speed) {
                SDL_BlitSurface(obsSurfs[q], &o, test, &tempObs);
            }
        }

        SDL_Rect tempNextObs;

        for (int z = 0; z < nextObsRects.size(); z++){
            //Sets placeholder to rect info
            tempNextObs = nextObsRects[z];
            if (xOffset != -speed) {
                tempNextObs.x = tempNextObs.x + xOffset;
            }
            if (yOffset != -speed) {
                tempNextObs.y = tempNextObs.y + yOffset;
            }

            //This is a placeholder so that the surface blits at the 0, 0 location on the rect, but still only blits inside
            // the w and h of the rect
            SDL_Rect o = {0,0, tempNextObs.w,tempNextObs.h};

            //Blits rect object with offset
            if (xOffset != -speed || yOffset != -speed) {
                SDL_BlitSurface(nextObsSurfs[z], &o, test, &tempNextObs);
            }
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

            //This is a placeholder so that the surface blits at the 0, 0 location on the rect, but still only blits inside
            // the w and h of the rect
            SDL_Rect o = {0,0,tempCurr.w,tempCurr.h};

            //Blits rect object with offset
            if (xOffset != -speed || yOffset != -speed) {
            SDL_BlitSurface(surfsCurr[p], &o, test, &tempCurr);
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

            //This is a placeholder so that the surface blits at the 0, 0 location on the rect, but still only blits inside
            // the w and h of the rect
            SDL_Rect o = {0,0,temp.w,temp.h};

            //Blits rect object with offset
            SDL_BlitSurface(surfsNext[t], &o, test, &temp);
        }
        return false;
    } else {
        return true;
    }

}
