//
// Created by tropi on 11/24/2022.
//

#include "rooms.h"
#include "enemies.h"

#include <utility>
#include "SDL.h"
#include <iostream>
using namespace std;

//Constructor description
Rooms::Rooms(int rNum, vector<SDL_Rect> &rects, vector<SDL_Surface *> &surfs, vector<SDL_Rect> exits, vector<vector<int>> exitInfo, vector<SDL_Rect> obstacles, vector<vector<SDL_Surface *>> obsSurfs, vector<bool> hookable, vector<Enemies> enemies){
    roomNum = rNum;
    roomObjs = rects;
    roomSurfs = surfs;
    roomEnemies = std::move(enemies);
    roomExits = std::move(exits);
    roomExitInfo = std::move(exitInfo);
    roomObs = std::move(obstacles);
    roomObsSurfs = std::move(obsSurfs);
    roomObsHookable = std::move(hookable);
    for (int j = 0; j < roomObs.size(); j++){
        cycleLoopMax.push_back(static_cast<int>(roomObsSurfs.size()));
        cycleLoopIndex.push_back(0);
    }
    currTime = static_cast<int>(SDL_GetTicks());
    prevTime = 0;
}

void Rooms::updateRoom(SDL_Surface *test, SDL_Rect &textRect, SDL_Rect &imRect, int &yVel, int &xVel, bool &jump,
                       int &ghPieceVelY, int &ghPieceVelX, bool &dead, vector<SDL_Rect> &grappleArr) {

    for (auto & roomEnemie : roomEnemies){

        if (!roomEnemie.isDead()) {
            roomEnemie.update(test, imRect);
            SDL_Rect r = roomEnemie.getHitBox();

            if (SDL_HasIntersection(&grappleArr[0], &r)) {
                roomEnemie.addForce(ghPieceVelX, ghPieceVelY);
                if (roomEnemie.dealDamage(1)) {
                    roomEnemie.setDead(true);
                }
            }
        }
    }

    //Gets console ticks for delta time calculations
    currTime = static_cast<int>(SDL_GetTicks());

    for (int k = 0; k < roomObs.size(); k++) {

        SDL_Rect placeHolder = roomObs[k];

        if (currTime > prevTime + 1000) {
            prevTime = currTime;
            if (cycleLoopIndex[k] < cycleLoopMax[k]) {
                cycleLoopIndex[k]++;
            } else {
                cycleLoopIndex[k] = 0;
            }
        }

        SDL_BlitSurface(roomObsSurfs[k][cycleLoopIndex[k]], &roomObs[k], test, &placeHolder);

        if (SDL_HasIntersection(&roomObs[k], &imRect)) {
            dead = true;
        }

        for (auto &hitEnemie: roomEnemies) {
            SDL_Rect r = hitEnemie.getHitBox();
            if (SDL_HasIntersection(&roomObs[k], &r)) {
                hitEnemie.setDead(true);
            }
        }
    }

    //Bool to set xVel to 0 after running through every object so other objects can get collisions
    bool d;

    //Handle collision, where it determines the side of the rect that the player is intersecting on from the
    // distance the player is overlapping into the rect in relation to player's velocity for each obj in the
    // current room
    for (int t = 0; t < roomObjs.size(); t++){

        for(auto & roomEnemie : roomEnemies){
            roomEnemie.collisionTest(roomObjs[t]);
        }

        if (SDL_HasIntersection(&roomObjs[t], &imRect)){

            //Top side
            if ((imRect.y + imRect.h - roomObjs[t].y <= fabs(yVel) && yVel <= 0) || (imRect.y + imRect.h - roomObjs[t].y <= fabs(ghPieceVelY) && ghPieceVelY <= 0)){
                imRect.y = roomObjs[t].y - imRect.h;
                yVel = 0;
                d = true;
                jump = true;
            }

                //Bottom side
            else if ((roomObjs[t].y + roomObjs[t].h - imRect.y <= fabs(yVel) && yVel >= 0) || (roomObjs[t].y + roomObjs[t].h - imRect.y <= fabs(ghPieceVelY) && ghPieceVelY >= 0)){
                imRect.y = roomObjs[t].y + roomObjs[t].h;
                yVel = 0;
            }

            //Left and Right side
            if ((imRect.x + imRect.w - roomObjs[t].x <= fabs(xVel) && xVel <= 0) || (roomObjs[t].x + roomObjs[t].w - imRect.x <= fabs(xVel) && xVel >= 0)) {
                //Left side
                if (xVel < 0) {
                    imRect.x = roomObjs[t].x - imRect.w;
                    xVel = 0;
                }

                    //Right side
                else {
                    imRect.x = roomObjs[t].x + roomObjs[t].w;
                    xVel = 0;
                }
            }
        }


        //This is a placeholder so that the actual rect position does not get changed by the blit function
        SDL_Rect re = roomObjs[t];

        //Blits surfaces
        SDL_BlitSurface(roomSurfs[t], &roomObjs[t], test, &re);
    }
    if (d){
        xVel = 0;
    }

}

vector<int> Rooms::exitRoom(SDL_Rect &imRect) {
    for (int r = 0; r < roomExits.size(); r ++) {
        if (SDL_HasIntersection(&imRect, &roomExits[r])){
            return roomExitInfo[r];
        }
    }
    return {-1, 0, 0};
}

vector<SDL_Rect> Rooms::getRects() {
    return roomObjs;
}

vector<SDL_Surface *> Rooms::getSurfs() {
    return roomSurfs;
}

vector<SDL_Rect> Rooms::getHittableRects() {
    return roomObs;
}

vector<bool> Rooms::getHitTest() {
    return roomObsHookable;
}

vector<Enemies> Rooms::getEnemies() {
    return roomEnemies;
}
