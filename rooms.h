//
// Created by tropi on 11/24/2022.
//

#ifndef MYPROJECT_ROOMS_H
#define MYPROJECT_ROOMS_H

#include "SDL.h"
#include <algorithm>

using namespace std;

//Class for the rooms the player will go through
class Rooms{

private:

    int roomNum;
    vector<SDL_Rect *> roomObjs;
    vector<SDL_Surface *> roomSurfs;

public:

    //Constructor
    Rooms(int rNum, vector<SDL_Rect *> &rects, vector<SDL_Surface *> &surfs);

    //Blits the surfaces in the level and handles collision for the rects in the level
    void updateRoom(SDL_Surface * test, SDL_Rect &textRect, SDL_Rect &imRect, int &yVel, int &xVel, bool &jump,
                    int &ghPieceVelY, int &ghPieceVelX){

        //Handle collision, where it determines the side of the rect that the player is intersecting on from the
        // distance the player is overlapping into the rect in relation to player's velocity for each obj in the
        // current room
        for (int t = 0; t < roomSurfs.size(); t++){

            if (SDL_HasIntersection(roomObjs[t], &imRect)){

                //Top side
                if ((imRect.y + imRect.h - roomObjs[t]->y <= fabs(yVel) && yVel <= 0) || (imRect.y + imRect.h - roomObjs[t]->y <= fabs(ghPieceVelY) && ghPieceVelY <= 0)){
                    imRect.y = roomObjs[t]->y - imRect.h;
                    yVel = 0;
                    xVel = 0;
                    jump = true;
                }

                //Bottom side
                else if ((roomObjs[t]->y + roomObjs[t]->h - imRect.y <= fabs(yVel) && yVel >= 0) || (roomObjs[t]->y + roomObjs[t]->h - imRect.y <= fabs(ghPieceVelY) && ghPieceVelY >= 0)){
                    imRect.y = roomObjs[t]->y + roomObjs[t]->h;
                    yVel = 0;
                }

                //Left and Right side
                else if ((imRect.x + imRect.w - roomObjs[t]->x <= fabs(xVel) && xVel < 0) || (roomObjs[t]->x + roomObjs[t]->w - imRect.x <= fabs(xVel) && xVel > 0)) {
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
    }

    //Function to get all the rectangles in the level
    vector<SDL_Rect *> getRects(){
        return roomObjs;
    }
    vector<SDL_Surface *> getSurfs(){
        return roomSurfs;
    }
};

//Constructor description
Rooms::Rooms(int rNum, vector<SDL_Rect *> &rects, vector<SDL_Surface *> &surfs) {
    roomNum = rNum;
    roomObjs = rects;
    roomSurfs = surfs;
}

#endif //MYPROJECT_ROOMS_H
