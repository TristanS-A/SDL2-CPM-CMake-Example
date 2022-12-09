//
// Created by tropi on 11/18/2022.
//

#include "playerMovement.h"
#include "globalVariables.h"
#include "SDL.h"

void playerMovement(bool &jump, bool up, bool left, bool right, bool down, int &yVel, int &xVel, SDL_Rect &imRect){

    if (damageCoolDown <= 0) {
        walkingCurrTime = static_cast<int>(SDL_GetTicks());
        if (shoot || retrac){
            if (jump && (left || right) && s != 0){
                if (walkingCurrTime > walkingPrevTime + 1000 / 5) {
                    walkingPrevTime = walkingCurrTime;
                    walkIndex++;
                }
                if (walkIndex == grappleWalkL.size()) {
                    walkIndex = 0;
                }

                if (ghPieceVelX < 0) {
                    saveDir = true;
                    im = grappleWalkL[walkIndex];
                } else {
                    saveDir = false;
                    im = grappleWalkR[walkIndex];
                }
            } else if (jump){
                if (ghPieceVelX <= 0){

                    im = grappleWalkL[0];
                    saveDir = true;

                } else {

                    saveDir = false;
                    im = grappleWalkR[0];

                }
            }
            else if (saveDir){

                im = grapplingImageL;

            } else {

                im = grapplingImageR;

            }
        }
        else if (left){
            saveDir = true;
            if (jump) {
                if (walkingCurrTime > walkingPrevTime + 1000 / 5) {
                    walkingPrevTime = walkingCurrTime;
                    walkIndex++;
                }
                if (walkIndex == leftMovement.size()) {
                    walkIndex = 0;
                }
                im = leftMovement[walkIndex];
            } else {
                im = leftMovement[1];
            }
        } else if (right) {
            saveDir = false;
            if (jump) {
                if (walkingCurrTime > walkingPrevTime + 1000 / 5) {
                    walkingPrevTime = walkingCurrTime;
                    walkIndex++;
                }
                if (walkIndex == rightMovement.size()) {
                    walkIndex = 0;
                }

                im = rightMovement[walkIndex];
            } else {
                im = rightMovement[1];
            }

        } else {
            if (jump) {
                if (saveDir) {
                    im = leftMovement[0];
                } else {
                    im = rightMovement[0];
                }
            } else {
                if (saveDir) {
                    im = leftMovement[1];
                } else {
                    im = rightMovement[1];
                }
            }
        }
    }

    //Character control
    if (up && jump) {
        yVel = 30;
        jump = false;
        imRect.y -= yVel;
    }
    if (left) {
        if (xVel < 1) {
            xVel += 5;
        }
    }
    if (right) {
        if (xVel > -1) {
            xVel += -5;
        }
    }
    if (down) {
        yVel -= 5;
    }
}
