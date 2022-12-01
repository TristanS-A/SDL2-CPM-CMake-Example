//
// Created by tropi on 11/24/2022.
//

#ifndef MYPROJECT_ROOMS_H
#define MYPROJECT_ROOMS_H

#include "enemies.h"

#include "SDL.h"
#include <algorithm>
using namespace std;

//Class for the rooms the player will go through
class Rooms{

private:

    int roomNum;
    vector<SDL_Rect> roomObjs;
    vector<SDL_Surface *> roomSurfs;
    vector<SDL_Rect> roomExits;
    vector<vector<int>> roomExitInfo;
    vector<SDL_Rect> roomObs;
    vector<vector<SDL_Surface *>> roomObsSurfs;
    vector<bool> roomObsHookable;

    //For looping through obstacle animations
    vector<int> cycleLoopIndex;
    vector<int> cycleLoopMax;

    //For keeping track of time
    int currTime;
    int prevTime;

    vector<Enemies> roomEnemies;

public:

    //Constructor
    Rooms(int rNum, vector<SDL_Rect> &rects, vector<SDL_Surface *> &surfs, vector<SDL_Rect> exits, vector<vector<int>> exitInfo, vector<SDL_Rect> obstacles, vector<vector<SDL_Surface *>> obsSurfs, vector<bool> hookable, vector<Enemies> enemies);

    //Blits the surfaces in the level and handles collision for the rects in the level
    void updateRoom(SDL_Surface * test, SDL_Rect &textRect, SDL_Rect &imRect, int &yVel, int &xVel, bool &jump,
                    int &ghPieceVelY, int &ghPieceVelX, bool &dead, vector<SDL_Rect> &grappleArr, int track, bool &hitEnemie);

    //Function to tell if the player is exiting the current room
    vector<int> exitRoom(SDL_Rect &imRect);

    //Function to get all the rectangles in the level
    vector<SDL_Rect> getRects();

    //Function to get all the surfaces in the level
    vector<SDL_Surface *> getSurfs();

    vector<SDL_Rect> getHittableRects();

    vector<bool> getHitTest();

    vector<Enemies> getEnemies();

    vector<SDL_Rect> getObsRects();

    vector<SDL_Surface *> getObsSurfs();

    vector<SDL_Rect> getEnemieRects();

    vector<SDL_Surface *> getEnemieSurfs();

    vector<SDL_Surface *> getEnemieDefaultSurfs();

};

#endif //MYPROJECT_ROOMS_H