//
// Created by tropi on 11/24/2022.
//

#include "rooms.h"
#include "enemies.h"
#include "handleCollision.h"

#include <utility>
#include "SDL.h"
#include <iostream>
using namespace std;

//Constructor description
Rooms::Rooms(SDL_Rect resetLocation, vector<SDL_Rect> &rects, vector<SDL_Surface *> &surfs, vector<SDL_Rect> exits, vector<vector<int>> exitInfo, vector<SDL_Rect> obstacles, vector<vector<SDL_Surface *>> obsSurfs, vector<bool> hookable, vector<Enemies> enemies, SDL_Surface *backImage){
    respawnLocation = resetLocation;
    roomObjs = rects;
    roomSurfs = surfs;
    roomEnemies = std::move(enemies);
    roomExits = std::move(exits);
    roomExitInfo = std::move(exitInfo);
    roomObs = std::move(obstacles);
    roomObsSurfs = std::move(obsSurfs);
    roomObsHookable = std::move(hookable);
    bgImage = backImage;
    for (int j = 0; j < roomObs.size(); j++){
        cycleLoopMax.push_back(static_cast<int>(roomObsSurfs.size()));
        cycleLoopIndex.push_back(0);
    }
    currTime = static_cast<int>(SDL_GetTicks());
    prevTime = 0;
}

void Rooms::updateRoom(SDL_Surface *test, SDL_Rect &textRect, SDL_Rect &imRect, int &yVel, int &xVel, bool &jump,
                       int &ghPieceVelY, int &ghPieceVelX, bool &dead, vector<SDL_Rect> &grappleArr, int track, bool &hitEnemie) {

    for (auto & roomEnemie : roomEnemies){

        if (!roomEnemie.isDead()) {
            roomEnemie.update(test, imRect);
            SDL_Rect r = roomEnemie.getHitBox();

            if (SDL_HasIntersection(&grappleArr[0], &r) && !hitEnemie) {
                roomEnemie.addForce(ghPieceVelX, ghPieceVelY);
                if (roomEnemie.dealDamage(1)) {
                    roomEnemie.setDead(true);
                } else {
                    hitEnemie = true;
                }
            }
        } else if (!roomEnemie.deathAnimationDone()){
            roomEnemie.enemieDeath(test);
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
    bool d = false;

    //Reset jump and sets to true if the player is on the ground
    jump = false;

    //Handle collision, where it determines the side of the rect that the player is intersecting on from the
    // distance the player is overlapping into the rect in relation to player's velocity for each obj in the
    // current room
    for (int t = 0; t < roomObjs.size(); t++){

        //Handles collision for enemies
        for(auto & roomEnemie : roomEnemies){
            roomEnemie.collisionTest(roomObjs[t]);
        }

        //Handles collision for player
        handleCollision(roomObjs[t], imRect, yVel, xVel, d, jump);

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

vector<SDL_Rect> Rooms::getObsRects() {
    return roomObs;
}

vector<SDL_Surface *> Rooms::getObsSurfs() {
    vector<SDL_Surface *> temp;
    for (int t = 0; t < roomObs.size(); t++){
        temp.push_back(roomObsSurfs[t][cycleLoopIndex[t]]);
    }
    return temp;
}

vector<SDL_Rect> Rooms::getEnemieRects() {
    vector<SDL_Rect> temp;
    for (auto & roomEnemie : roomEnemies){
        temp.push_back(roomEnemie.getHitBox());
    }
    return temp;
}

vector<SDL_Surface *> Rooms::getEnemieSurfs() {
    vector<SDL_Surface *> temp;
    for (auto & roomEnemie : roomEnemies){
        temp.push_back(roomEnemie.getCurrImage());
    }
    return temp;
}

vector<SDL_Surface *> Rooms::getEnemieDefaultSurfs() {
    vector<SDL_Surface *> temp;
    for (auto & roomEnemie : roomEnemies){
        temp.push_back(roomEnemie.getDefaultImage());
    }
    return temp;
}

SDL_Rect Rooms::getRespawnLocation() {
    return respawnLocation;
}

void Rooms::roomReset(SDL_Rect &imRect, int &yVel, int &xVel, vector<SDL_Rect> &arrR, bool &shoot, bool &hit, bool &retrac) {
    for (auto & enemie : roomEnemies){
        enemie.resetEnemie();
    }

    imRect.x = respawnLocation.x;
    imRect.y = respawnLocation.y;

    for (auto &arr : arrR){
        arr.x = -100;
        arr.y = -100;
    }

    yVel = 0;
    xVel = 0;

    shoot = false;
    retrac = false;
    hit = false;

}

SDL_Surface* Rooms::getBG() {

    return bgImage;

}

