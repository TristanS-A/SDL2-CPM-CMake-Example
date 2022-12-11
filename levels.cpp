//
// Created by tropi on 12/3/2022.
//

#include "levels.h"

#include <utility>
#include "rooms.h"
#include "roomSwitch.h"
#include "grappleHook.h"
#include "globalVariables.h"

Levels::Levels(vector<Rooms> levRooms, SDL_Surface *parallaxBG){
    levelRooms = std::move(levRooms);
    paraBG = parallaxBG;
    currRoom = 0;
}

void Levels::levelUpdate(SDL_Surface *test, SDL_Rect &imRect, int &playerHealth, bool &right, bool &left, bool &transition, int &yVel, int &xVel, SDL_Rect &textRect, bool &dead, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, int &track, int &sideOffsetX, int &sideOffsetY, bool &mouseUp, bool &hitEnemie, vector<SDL_Surface *> deathAnimation, int &deathAnimationIndex, bool &dropCurtain, bool &raiseCurtain, int &curtainOffset, SDL_Surface *curtain, bool &goToLevSelScreen, bool &levelSelect, SDL_Rect &paraBGRect, int &paraBGx, int &paraBGy) {

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
                //So the player cannot leave the room after opening the chest to leave the level
                if (!exiting) {
                    transition = true;
                    arrR[0].x = -100;
                    arrR[0].y = -100;
                    retrac = false;
                    shoot = false;
                    nextRoom = exitInfo[0];
                    transitionSpeed = exitInfo[1];
                    offsetX = exitInfo[2];
                    offseetY = exitInfo[3];
                    if (offseetY < -transitionSpeed) {
                        yVel = 30;
                    }
                } else {
                    if (imRect.x > 1440 + 50){
                        imRect.x = 1440 + 50;
                    } else if (imRect.x < -50){
                        imRect.x = -50;
                    }
                    if (imRect.y > 810 + 50){
                        imRect.y = 810 + 50;
                    } else if (imRect.y < -50){
                        imRect.y = -50;
                    }
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
            levelRooms[currRoom].updateRoom(test, textRect, imRect, yVel, xVel, dead, arrR, s, hitEnemie, playerHealth);

            if (playerHealth <= 0){
                dead = true;
            }
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
                            imRect, offseetY, offsetX, exitInfo[2] + exitInfo[3], transitionSpeed, test, currBG, nextBG,
                            paraBGRect, paraBG, paraBGx, paraBGy, levelRooms[currRoom].getChestRect(), levelRooms[nextRoom].getChestRect())) {
                transition = false;
                noSwitch = true;
                currRoom = exitInfo[0];

                //So that if you leave the room while shooting the grappling hook, it resets, so it doesn't
                // stay if you go back to that room before shooting the hook again
                arrR[0].x = imRect.x;
                arrR[0].y = imRect.y;

                paraBGRect.x = paraBGx % 1440;
                paraBGRect.y = paraBGy % 810;

                //Blits parallax background
                SDL_BlitSurface(paraBG, nullptr, test, &paraBGRect);

                SDL_Rect pHolder = {0, 0, 0, 0};

                //Blits no-parallax bg
                SDL_BlitSurface(levelRooms[currRoom].getBG(), nullptr, test, &pHolder);

                if (!levelRooms[currRoom].getChestRect().empty()){
                    if (exiting){
                        SDL_Rect exitHolder = levelRooms[currRoom].getChestRect()[0];
                        SDL_BlitSurface(chestSurfs[1], nullptr, test, &exitHolder);
                    } else {
                        SDL_Rect exitHolder = levelRooms[currRoom].getChestRect()[0];
                        SDL_BlitSurface(chestSurfs[0], nullptr, test, &exitHolder);
                    }
                }

                //Placeholder so that imRect does not get altered by the blitting function
                SDL_Rect placeH = imRect;

                //Blits cat image to test at the location, and showing the dimensions, of imRect (the image
                // rectangle)
                SDL_BlitSurface(im, nullptr, test, &placeH);

                //Tests blitting for room objects
                levelRooms[currRoom].updateRoom(test, textRect, imRect, yVel, xVel, dead, arrR, s, hitEnemie, playerHealth);
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

        if (!levelRooms[currRoom].getChestRect().empty()){
            if (exiting){
                SDL_Rect exitHolder = levelRooms[currRoom].getChestRect()[0];
                SDL_BlitSurface(chestSurfs[1], nullptr, test, &exitHolder);
            } else {
                SDL_Rect exitHolder = levelRooms[currRoom].getChestRect()[0];
                SDL_BlitSurface(chestSurfs[0], nullptr, test, &exitHolder);
            }
        }

        //Tests blitting for room objects
        levelRooms[currRoom].updateRoom(test, textRect, imRect, yVel, xVel, dead, arrR, s, hitEnemie, playerHealth);

        deathCurrTime = static_cast<int>(SDL_GetTicks());

        //This is for if the player dies normally in the level
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

                    //Drops curtain after death animation is done and when the curtain has
                    // been raised all the way
                    curtainOffset = -810;
                    dropCurtain = true;
                }

                deathPrevTime = deathCurrTime;

            }
        }
    }

    if (!raiseCurtain && dropCurtain){

        //This is for if the player dies while exiting to the level select screen because I wanted the curtain to drop
        // after the death animation has played, so I need to know what to do if the player dies while the curtain is
        // lowering.
        if (dead){

            deathCurrTime = static_cast<int>(SDL_GetTicks());

            //Placeholder for image rects so that the blit function doesn't change the rectangle location, and
            // also centers the images to the player
            SDL_Rect imPlaceHolder = {imRect.x + imRect.w / 2 - 100, imRect.y + imRect.h / 2 - 100, 0, 0};

            if (deathAnimationIndex < deathAnimation.size() - 1) {

                    //Blits death animation
                    SDL_BlitSurface(deathAnimation[deathAnimationIndex], nullptr, test, &imPlaceHolder);

                if (deathCurrTime > deathPrevTime + 1000 / 10) {

                    //Moves through death animation images
                    deathAnimationIndex++;

                    deathPrevTime = deathCurrTime;
                }

            }
        }

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

            if (goToLevSelScreen || exiting){
                levelSelect = true;
            }

            //Gets curtain ready to raise
            curtainOffset = 5;
            levelRooms[currRoom].roomAndPlayerReset(imRect, playerHealth, yVel, xVel, arrR);
            raiseCurtain = true;
            dropCurtain = false;
            dead = false;
            damageCoolDown = 0;
            deathAnimationIndex = 0;
            im = rightMovement[0];
            saveDir = false;
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

void Levels::resetLevel() {

    currRoom = 0;

    for (auto & levelRoom : levelRooms){
        levelRoom.roomReset();
    }

}
