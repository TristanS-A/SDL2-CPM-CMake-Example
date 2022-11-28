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
    vector<SDL_Rect *> roomObjs;
    vector<SDL_Surface *> roomSurfs;
    vector<SDL_Rect> roomExits;
    vector<vector<int>> roomExitInfo;

public:

    //Constructor
    Rooms(int rNum, vector<SDL_Rect *> &rects, vector<SDL_Surface *> &surfs, vector<SDL_Rect> exits, vector<vector<int>> exitInfo);

    //Blits the surfaces in the level and handles collision for the rects in the level
    void updateRoom(SDL_Surface * test, SDL_Rect &textRect, SDL_Rect &imRect, int &yVel, int &xVel, bool &jump,
                    int &ghPieceVelY, int &ghPieceVelX){

        //Bool to set xVel to 0 after running through every object so other objects can get collisions
        bool d;

        //Handle collision, where it determines the side of the rect that the player is intersecting on from the
        // distance the player is overlapping into the rect in relation to player's velocity for each obj in the
        // current room
        for (int t = 0; t < roomObjs.size(); t++){

            if (SDL_HasIntersection(roomObjs[t], &imRect)){

                //Top side
                if ((imRect.y + imRect.h - roomObjs[t]->y <= fabs(yVel) && yVel <= 0) || (imRect.y + imRect.h - roomObjs[t]->y <= fabs(ghPieceVelY) && ghPieceVelY <= 0)){
                    imRect.y = roomObjs[t]->y - imRect.h;
                    yVel = 0;
                    d = true;
                    jump = true;
                }

                //Bottom side
                else if ((roomObjs[t]->y + roomObjs[t]->h - imRect.y <= fabs(yVel) && yVel >= 0) || (roomObjs[t]->y + roomObjs[t]->h - imRect.y <= fabs(ghPieceVelY) && ghPieceVelY >= 0)){
                    imRect.y = roomObjs[t]->y + roomObjs[t]->h;
                    yVel = 0;
                }

                //Left and Right side
                if ((imRect.x + imRect.w - roomObjs[t]->x <= fabs(xVel) && xVel <= 0) || (roomObjs[t]->x + roomObjs[t]->w - imRect.x <= fabs(xVel) && xVel >= 0)) {
                    //Left side
                    if (xVel < 0) {
                        imRect.x = roomObjs[t]->x - imRect.w;
                        xVel = 0;
                    }

                    //Right side
                    else {
                        imRect.x = roomObjs[t]->x + roomObjs[t]->w;
                        xVel = 0;
                    }
                }
            }

            //This is a placeholder so that the actual rect position does not get changed by the blit function
            SDL_Rect re = *roomObjs[t];

            //Blits surfaces
            SDL_BlitSurface(roomSurfs[t], roomObjs[t], test, &re);
        }
        if (d){
            xVel = 0;
        }
    }

    //Function to tell if the player is exiting the current room
    vector<int> exitRoom(SDL_Rect &imRect){
        for (int r = 0; r < roomExits.size(); r ++) {
            if (SDL_HasIntersection(&imRect, &roomExits[r])){
                return roomExitInfo[r];
            }
        }
        return {-1, 0, 0};
    }

    int getRoomNum(){
        return roomNum;
    }

    //Function to get all the rectangles in the level
    vector<SDL_Rect *> getRects(){
        return roomObjs;
    }

    //Function to get all the surfaces in the level
    vector<SDL_Surface *> getSurfs(){
        return roomSurfs;
    }
};

//Constructor description
Rooms::Rooms(int rNum, vector<SDL_Rect *> &rects, vector<SDL_Surface *> &surfs, vector<SDL_Rect> exits, vector<vector<int>> exitInfo) {
    roomNum = rNum;
    roomObjs = rects;
    roomSurfs = surfs;
    roomExits = std::move(exits);
    roomExitInfo = std::move(exitInfo);
}

#endif //MYPROJECT_ROOMS_H
