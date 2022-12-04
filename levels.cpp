//
// Created by tropi on 12/3/2022.
//

#include "levels.h"

#include <utility>
#include "rooms.h"
#include "roomSwitch.h"
#include "grappleHook.h"

Levels::Levels(vector<Rooms> levRooms, SDL_Surface *parallaxBG){
    levelRooms = std::move(levRooms);
    paraBG = parallaxBG;
    currRoom = 0;
}

void Levels::levelUpdate(SDL_Surface *test, SDL_Rect &imRect, SDL_Surface *im, bool &right, bool &left, bool &transition, bool &shoot, bool &retrac, bool &hit, int &yVel, int &xVel, SDL_Rect &textRect, bool &jump, int &ghPieceVelY, int &ghPieceVelX, bool &dead, vector<SDL_Rect> arrR, vector<SDL_Surface *> arr, int &track, int &sideOffsetX, int &sideOffsetY, bool &mouseUp, int &s, bool &hitEnemie, vector<SDL_Surface *> deathAnimation, int &deathAnimationIndex, bool &dropCurtain, bool &raiseCurtain, int &curtainOffset, SDL_Surface *curtain, int &deathCurrTime, int &deathPrevTime, SDL_Rect &paraBGRect, int &paraBGx, int &paraBGy) {

    if (!dead) {
        if (!transition) {

            exitInfo = levelRooms[currRoom].exitRoom(imRect);

            if (exitInfo[1] > 0) {
                if (right) {
                    noSwitch = false;
                }
            } else if (exitInfo[1] < 0) {
                if (left) {
                    noSwitch = false;
                }
            }

            if (exitInfo[0] != -1 && !noSwitch) {
                transition = true;
                retrac = false;
                shoot = false;
                nextRoom = exitInfo[0];
                transitionSpeed = exitInfo[1];
                offsetX = exitInfo[2];
                offseetY = exitInfo[3];
                if (offseetY < -transitionSpeed) {
                    yVel = 30;
                }
            } else if (exitInfo[0] == -1) {
                noSwitch = false;
            }

            //Placeholder so that imRect does not get altered by the blitting function
            SDL_Rect placeH = imRect;

            //Blits cat image to test at the location, and showing the dimensions, of imRect (the image
            // rectangle)
            SDL_BlitSurface(im, nullptr, test, &placeH);

            //Tests blitting for room objects
            levelRooms[currRoom].updateRoom(test, textRect, imRect, yVel, xVel, jump,
                                            ghPieceVelY, ghPieceVelX, dead, arrR, s, hitEnemie);
        } else {
            vector<SDL_Rect> currObjs = levelRooms[currRoom].getRects();
            vector<SDL_Surface *> currSurfs = levelRooms[currRoom].getSurfs();
            vector<SDL_Rect> nextObjs = levelRooms[nextRoom].getRects();
            vector<SDL_Surface *> nextSurfs = levelRooms[nextRoom].getSurfs();
            vector<SDL_Rect> currObs = levelRooms[currRoom].getObsRects();
            vector<SDL_Surface *> currObsSurfs = levelRooms[currRoom].getObsSurfs();
            vector<SDL_Rect> nextObs = levelRooms[nextRoom].getObsRects();
            vector<SDL_Surface *> nextObsSurfs = levelRooms[nextRoom].getObsSurfs();
            vector<SDL_Rect> currEnemieRects = levelRooms[currRoom].getEnemieRects();
            vector<SDL_Surface *> currEnemieSurfs = levelRooms[currRoom].getEnemieSurfs();
            vector<SDL_Rect> nextEnemieRects = levelRooms[nextRoom].getEnemieRects();
            vector<SDL_Surface *> nextEnemieSurfs = levelRooms[nextRoom].getEnemieDefaultSurfs();
            SDL_Surface *currBG = levelRooms[currRoom].getBG();
            SDL_Surface *nextBG = levelRooms[nextRoom].getBG();

            if (switchRooms(currObjs, nextObjs, nextSurfs, currSurfs, currEnemieRects, currEnemieSurfs,
                            nextEnemieRects, nextEnemieSurfs, levelRooms[currRoom].getEnemies(),
                            levelRooms[nextRoom].getEnemies(), currObs, currObsSurfs, nextObs, nextObsSurfs,
                            imRect, offseetY, offsetX, exitInfo[2] + exitInfo[3], transitionSpeed, test, im, currBG, nextBG,
                            paraBGRect, paraBG, paraBGx, paraBGy)) {
                transition = false;
                noSwitch = true;
                currRoom = exitInfo[0];

                //So that if you leave the room while shooting the grappling hook, it resets, so it doesn't
                // stay if you go back to that room before shooting the hook again
                arrR[0].x = -100;
                arrR[0].y = -100;

                paraBGRect.x = paraBGx % 1440;
                paraBGRect.y = paraBGy % 810;

                //Blits parallax background
                SDL_BlitSurface(paraBG, nullptr, test, &paraBGRect);

                SDL_Rect pHolder = {0, 0, 0, 0};

                //Blits no-parallax bg
                SDL_BlitSurface(levelRooms[currRoom].getBG(), nullptr, test, &pHolder);

                //Tests blitting for room objects
                levelRooms[currRoom].updateRoom(test, textRect, imRect, yVel, xVel, jump,
                                                ghPieceVelY, ghPieceVelX, dead, arrR, s, hitEnemie);
            }
        }
    } else {

        SDL_Rect pHolder = {0, 0, 0, 0};

        paraBGRect.x = paraBGx % 1440;
        paraBGRect.y = paraBGy % 810;

        //Blits parallax background
        SDL_BlitSurface(paraBG, nullptr, test, &paraBGRect);

        //Blits no-parallax bg
        SDL_BlitSurface(levelRooms[currRoom].getBG(), nullptr, test, &pHolder);

        //Tests blitting for room objects
        levelRooms[currRoom].updateRoom(test, textRect, imRect, yVel, xVel, jump,
                                      ghPieceVelY, ghPieceVelX, dead, arrR, s, hitEnemie);

        deathCurrTime = static_cast<int>(SDL_GetTicks());

        if (!dropCurtain) {

            //Placeholder for image rects so that the blit function doesn't change the rectangle location, and
            // also centers the images to the player
            SDL_Rect imPlaceHolder = {imRect.x + imRect.w / 2 - 100, imRect.y + imRect.h / 2 - 100, 0, 0};

            //Blits death animation
            SDL_BlitSurface(deathAnimation[deathAnimationIndex], nullptr, test, &imPlaceHolder);

            if (deathCurrTime > deathPrevTime + 1000 / 10) {
                if (deathAnimationIndex < deathAnimation.size() - 1) {

                    //Moves through death animation images
                    deathAnimationIndex++;
                } else {

                    //Resets death animation and drops curtain after it is done and when the curtain has
                    // been raised all the way
                    deathAnimationIndex = 0;
                    curtainOffset = -810;
                    dropCurtain = true;
                }

                deathPrevTime = deathCurrTime;

            }
        }
        else if (!raiseCurtain){

            //Placeholder for curtain and blits it with blit function
            SDL_Rect curtainRect = {0, curtainOffset, 0, 0};
            SDL_BlitSurface(curtain, nullptr, test, &curtainRect);

            if (curtainOffset < 0){
                curtainOffset = static_cast<int>(curtainOffset / 1.1);

                //Check so the curtain doesn't lower more than the screen hight
                if (curtainOffset > 0){
                    curtainOffset = 0;
                }
            } else {

                //Gets curtain ready to raise
                curtainOffset = 5;
                levelRooms[currRoom].roomReset(imRect, yVel, xVel, arrR, shoot, hit, retrac);
                raiseCurtain = true;
                dropCurtain = false;
                dead = false;
            }
        }
    }

    if (raiseCurtain){
        if (curtainOffset > -810) {

            //Raises curtain
            curtainOffset = -static_cast<int>(fabs(curtainOffset * 1.39));

            //Curtain placeholder and blit function
            SDL_Rect curtainRect = {0, curtainOffset, 0, 0};
            SDL_BlitSurface(curtain, nullptr, test, &curtainRect);

        } else {

            //Resets curtain after raising all the way
            curtainOffset = -810;
            raiseCurtain = false;

        }
    }

}

Rooms Levels::getRoom() {
    return levelRooms[currRoom];
}
