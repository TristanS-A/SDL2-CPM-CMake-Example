//
// Created by tropi on 11/24/2022.
//

#ifndef MYPROJECT_ROOMS_H
#define MYPROJECT_ROOMS_H

#include "SDL.h"
#include <algorithm>
#include <utility>

using namespace std;

//Class for the rooms the player will go through
class Rooms{

private:

    int roomNum;
    vector<SDL_Rect> roomObjs;
    vector<SDL_Surface *> roomSurfs;
    vector<SDL_Rect> roomExits;
    vector<vector<int>> roomExitInfo;

public:

    //Constructor
    Rooms(int rNum, vector<SDL_Rect> &rects, vector<SDL_Surface *> &surfs, vector<SDL_Rect> exits, vector<vector<int>> exitInfo);

    //Blits the surfaces in the level and handles collision for the rects in the level
    void updateRoom(SDL_Surface * test, SDL_Rect &textRect, SDL_Rect &imRect, int &yVel, int &xVel, bool &jump,
                    int &ghPieceVelY, int &ghPieceVelX);

    //Function to tell if the player is exiting the current room
    vector<int> exitRoom(SDL_Rect &imRect);

    //Function to get all the rectangles in the level
    vector<SDL_Rect> getRects();

    //Function to get all the surfaces in the level
    vector<SDL_Surface *> getSurfs();
};

#endif //MYPROJECT_ROOMS_H
