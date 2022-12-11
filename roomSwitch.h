//
// Created by tropi on 11/26/2022.
//

#ifndef MYPROJECT_ROOMSWITCH_H
#define MYPROJECT_ROOMSWITCH_H

#include <iostream>
#include "SDL.h"
#include <algorithm>
#include "enemies.h"

using namespace std;

bool switchRooms(vector<SDL_Rect> &currRects, vector<SDL_Rect> &nextRects, vector<SDL_Surface *> &surfsNext, vector<SDL_Surface *> &surfsCurr, vector<SDL_Rect> enemieRects, vector <SDL_Surface *> enemieSurf, vector<SDL_Rect> nextEnemieRects, vector<SDL_Surface *> nextEnemieSurfs, vector<Enemies> currEnemies, vector<Enemies> nextEnemies, vector<SDL_Rect> obsRects, vector<SDL_Surface *> obsSurfs, vector<SDL_Rect> nextObsRects, vector<SDL_Surface *> nextObsSurfs, SDL_Rect &imRect, int &yOffset, int &xOffset, int constDis, int speed, SDL_Surface* test, SDL_Surface *currBG, SDL_Surface *nextBG, SDL_Rect &g, SDL_Surface *paraBG, int &paraBGx, int &paraBGy, vector<SDL_Rect> currChestRect, vector<SDL_Rect> nextChestRect);
#endif //MYPROJECT_ROOMSWITCH_H
