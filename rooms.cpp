//
// Created by tropi on 11/24/2022.
//

#include "rooms.h"
#include "enemies.h"
#include "handleCollision.h"
#include "globalVariables.h"

#include <utility>
#include "SDL.h"
#include <iostream>
using namespace std;

//Constructor description
Rooms::Rooms(SDL_Rect resetLocation, vector<SDL_Rect> &rects, vector<SDL_Surface *> &surfs, vector<SDL_Rect> exits, vector<vector<int>> exitInfo, vector<SDL_Rect> obstacles, vector<vector<SDL_Surface *>> obsSurfs, vector<bool> hookable, vector<Enemies> enemies, SDL_Surface *backImage, vector<SDL_Rect> &leaveLevelRect){
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
    for (auto & roomObsSurf : roomObsSurfs){
        cycleLoopMax.push_back(static_cast<int>(roomObsSurf.size() - 1));
        cycleLoopIndex.push_back(0);
    }
    currTime = static_cast<int>(SDL_GetTicks());
    prevTime = 0;
    levelExitRect = leaveLevelRect;
}

void Rooms::updateRoom(SDL_Surface *test, SDL_Rect &textRect, SDL_Rect &imRect, int &yVel, int &xVel, bool &dead, vector<SDL_Rect> &grappleArr, int track, bool &hitEnemie, int &playerHealth) {

    for (auto & roomEnemie : roomEnemies){

        if (!roomEnemie.isDead()) {
            roomEnemie.update(test, imRect);
            SDL_Rect r = roomEnemie.getHitBox();

            //Tests if player damages enemie
            if (SDL_HasIntersection(&grappleArr[0], &r) && !hitEnemie) {
                roomEnemie.addForce(ghPieceVelX, ghPieceVelY);
                if (roomEnemie.dealDamage(1)) {
                    roomEnemie.setDead(true);
                } else {
                    hitEnemie = true;
                }
            }

            //Kills enemie if they hit an obstacle
            for (auto & roomOb : roomObs){
                if (SDL_HasIntersection(&roomOb, &r)){
                    roomEnemie.setDead(true);
                }
            }

            //Tests if an enemie damages the player.
            if (SDL_HasIntersection(&imRect, &r)) {
                if (r.x + r.w / 2 < imRect.x + imRect.w / 2){
                    xVel = -15;
                    im = playerDamagedL;
                    saveDir = true;
                } else {
                    xVel = +15;
                    im = playerDamagedR;
                    saveDir = false;
                }
                yVel = 15;

                if (damageCoolDown <= 0) {
                    //So the player doesn't die whn exiting the level
                    if (!exiting) {
                        playerHealth -= 10;
                    }
                    damageCoolDown = 10;
                }
            }

        } else if (!roomEnemie.deathAnimationDone()){
            roomEnemie.enemieDeath(test);
        }
    }

    if (damageCoolDown > 0){
        damageCoolDown--;
    }

    //Gets console ticks for delta time calculations
    currTime = static_cast<int>(SDL_GetTicks());

    for (int k = 0; k < roomObs.size(); k++) {

        SDL_Rect placeHolder = roomObs[k];

        if (cycleLoopMax[k] > 0) {
            if (currTime > prevTime + 1000 / 5) {
                prevTime = currTime;
                if (cycleLoopIndex[k] < cycleLoopMax[k]) {
                    cycleLoopIndex[k]++;
                } else {
                    cycleLoopIndex[k] = 0;
                }
            }
        }

        //This is a placeholder so that the surface blits at the 0, 0 location on the rect, but still only blits inside
        // the w and h of the rect
        SDL_Rect o = {0,0,roomObs[k].w,roomObs[k].h};

        SDL_BlitSurface(roomObsSurfs[k][cycleLoopIndex[k]], &o, test, &placeHolder);

        if (SDL_HasIntersection(&roomObs[k], &imRect)) {
            dead = true;
        }

        for (auto &enemie: roomEnemies) {
            SDL_Rect r = enemie.getHitBox();
            if (SDL_HasIntersection(&roomObs[k], &r)) {
                enemie.setDead(true);
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
        handleCollision(roomObjs[t], imRect, yVel, xVel, d);

        //This is a placeholder so that the actual rect position does not get changed by the blit function
        SDL_Rect re = roomObjs[t];

        //This is a placeholder so that the surface blits at the 0, 0 location on the rect, but still only blits inside
        // the w and h of the rect
        SDL_Rect o = {0,0,roomObjs[t].w,roomObjs[t].h};

        //Blits surfaces
        SDL_BlitSurface(roomSurfs[t], &o, test, &re);
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

void Rooms::roomAndPlayerReset(SDL_Rect &imRect, int &playerHealth, int &yVel, int &xVel, vector<SDL_Rect> &arrR) {
    for (auto & enemie : roomEnemies){
        enemie.resetEnemie();
    }

    imRect.x = respawnLocation.x;
    imRect.y = respawnLocation.y;

    playerHealth = 50;

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

void Rooms::roomReset() {
    for (auto & enemie : roomEnemies){
        enemie.resetEnemie();
    }
}

SDL_Surface* Rooms::getBG() {

    return bgImage;

}

vector<SDL_Rect> Rooms::getChestRect() {
    return levelExitRect;
}

