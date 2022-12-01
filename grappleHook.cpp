//
// Created by tropi on 11/21/2022.
//

#include "grappleHook.h"
#include "SDL.h"
#include <algorithm>

//Function for shooting the grappling hook
void shooting(int &s, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int ghPieceVelY,
              int ghPieceVelX, bool &hit, bool &shoot, bool &retrac, int &track, SDL_Surface *test,
              vector<SDL_Rect> &hitObjects, vector<SDL_Rect> possibleHit, vector<bool> hitTest, int &sideOffsetY, int &sideOffsetX, int &yVel, int &xVel){

    //Shoots out rectangles until hitting something or until shooting all the graphing hook pieces. I made this an if
    // statement instead of a for loop so that the pieces don't instantly all go out at once before displaying and so
    // that everything else in the main loop also runs together with this.
    if (s < arr.size()) {


        //Resets grappling hook rect locations to the player's location, but since the first grappling hook rec is a
        // bigger rect, it will spawn under the player if not for the if statement
        if (s == 0){
            arrR[s].x = imRect.x + 100 / 2 - arrR[s].w / 2;
            arrR[s].y = imRect.y;
        } else {
            arrR[s].x = imRect.x + 100 / 2 - arrR[s].w / 2;
            arrR[s].y = imRect.y + 100 / 2 - arrR[s].h / 2;
        }

        //Blits and adds force to each grappling piece currently shooting out
        for (int b = 0; b < s; b++) {

            //This is for not extending grappling hook pieces through an object
//            for (auto & hitObject : hitObjects)
//                if (SDL_HasIntersection(&arrR[b], &hitObject)){
//                    if ((arrR[b].y + arrR[b].h - hitObject.y <= fabs(ghPieceVelY) && ghPieceVelY >= 0) || (arrR[b].y + arrR[b].h - hitObject.y <= fabs(ghPieceVelY) && ghPieceVelY >= 0)){
//                        arrR[b].y = hitObject.y - arrR[b].h;
//                        sideOffsetY = arrR[b].h;
//                    }
//                    else if ((hitObject.y + hitObject.h - arrR[b].y <= fabs(ghPieceVelY) && ghPieceVelY <= 0) || (hitObject.y + hitObject.h - arrR[b].y <= fabs(ghPieceVelY) && ghPieceVelY <= 0)){
//                        arrR[b].y = hitObject.y + hitObject.h;
//                        sideOffsetY = 0;
//                    }
//                    else if ((arrR[b].x + arrR[b].w - hitObject.x <= fabs(ghPieceVelX) && ghPieceVelX > 0) || (hitObject.x + hitObject.w - arrR[b].x <= fabs(ghPieceVelX) && ghPieceVelX > 0)) {
//                        arrR[b].x = hitObject.x - arrR[b].w;
//                        sideOffsetY = arrR[b].w;
//                    }
//                    else if ((arrR[b].x + arrR[b].w - hitObject.x <= fabs(ghPieceVelX) && ghPieceVelX < 0) || (hitObject.x + hitObject.w - arrR[b].x <= fabs(ghPieceVelX) && ghPieceVelX < 0)) {
//                        arrR[b].x = hitObject.x + hitObject.w;
//                        sideOffsetY = 0;
//                    }
//                }

            SDL_BlitSurface(arr[b], nullptr, test, &arrR[b]);
            arrR[b].y += ghPieceVelY;
            arrR[b].x += ghPieceVelX;
        }

        //Test for if the hook intersects with something
        for (auto & hitObject : hitObjects) {
            if (SDL_HasIntersection(&arrR[0], &hitObject)) {
                if ((arrR[0].y + arrR[0].h - hitObject.y <= fabs(ghPieceVelY) && ghPieceVelY >= 0) || (arrR[0].y +
                                                                                                       arrR[0].h -
                                                                                                       hitObject.y <=
                                                                                                       fabs(ghPieceVelY) &&
                                                                                                       ghPieceVelY >=
                                                                                                       0)) {
                    arrR[0].y = hitObject.y - arrR[0].h + arrR[0].h / 2;
                    sideOffsetY = -arrR[0].h / 2;
                    sideOffsetX = -imRect.w / 2 + arrR[0].w / 2;
                } else if ((hitObject.y + hitObject.h - arrR[0].y <= fabs(ghPieceVelY) && ghPieceVelY <= 0) ||
                           (hitObject.y + hitObject.h - arrR[0].y <= fabs(ghPieceVelY) && ghPieceVelY <= 0)) {
                    arrR[0].y = hitObject.y + hitObject.h - arrR[0].h / 2;
                    sideOffsetY = arrR[0].h / 2;
                    sideOffsetX = -imRect.w / 2 + arrR[0].w / 2;
                } else if ((arrR[0].x + arrR[0].w - hitObject.x <= fabs(ghPieceVelX) && ghPieceVelX > 0) ||
                           (hitObject.x + hitObject.w - arrR[0].x <= fabs(ghPieceVelX) && ghPieceVelX > 0)) {
                    arrR[0].x = hitObject.x - arrR[0].w / 2;
                    sideOffsetY = -imRect.h / 2 + arrR[0].h / 2;
                    sideOffsetX = -arrR[0].w / 2;
                } else if ((arrR[0].x + arrR[0].w - hitObject.x <= fabs(ghPieceVelX) && ghPieceVelX < 0) ||
                           (hitObject.x + hitObject.w - arrR[0].x <= fabs(ghPieceVelX) && ghPieceVelX < 0)) {
                    arrR[0].x = hitObject.x + hitObject.w - arrR[0].w / 2;
                    sideOffsetY = -imRect.h / 2 + arrR[0].h / 2;
                    sideOffsetX = arrR[0].w / 2;
                }

                if (yVel > 5) {
                    yVel = 5;
                } else if (yVel < -5) {
                    yVel = -5;
                }

                if (xVel > 5) {
                    xVel = 5;
                } else if (xVel < -5) {
                    xVel = -5;
                }

                hit = true;
            }
        }

        for (int l = 0; l < possibleHit.size(); l++){
            if (hitTest[l]){
                if (SDL_HasIntersection(&arrR[0], &possibleHit[l])) {
                    if ((arrR[0].y + arrR[0].h - possibleHit[l].y <= fabs(ghPieceVelY) && ghPieceVelY >= 0) || (arrR[0].y +
                                                                                                           arrR[0].h -
                                                                                                           possibleHit[l].y <=
                                                                                                           fabs(ghPieceVelY) &&
                                                                                                           ghPieceVelY >=
                                                                                                           0)) {
                        arrR[0].y = possibleHit[l].y - arrR[0].h + arrR[0].h / 2;
                        sideOffsetY = -arrR[0].h / 2;
                        sideOffsetX = -imRect.w / 2 + arrR[0].w / 2;
                    } else if ((possibleHit[l].y + possibleHit[l].h - arrR[0].y <= fabs(ghPieceVelY) && ghPieceVelY <= 0) ||
                               (possibleHit[l].y + possibleHit[l].h - arrR[0].y <= fabs(ghPieceVelY) && ghPieceVelY <= 0)) {
                        arrR[0].y = possibleHit[l].y + possibleHit[l].h - arrR[0].h / 2;
                        sideOffsetY = arrR[0].h / 2;
                        sideOffsetX = -imRect.w / 2 + arrR[0].w / 2;
                    } else if ((arrR[0].x + arrR[0].w - possibleHit[l].x <= fabs(ghPieceVelX) && ghPieceVelX > 0) ||
                               (possibleHit[l].x + possibleHit[l].w - arrR[0].x <= fabs(ghPieceVelX) && ghPieceVelX > 0)) {
                        arrR[0].x = possibleHit[l].x - arrR[0].w / 2;
                        sideOffsetY = -imRect.h / 2 + arrR[0].h / 2;
                        sideOffsetX = -arrR[0].w / 2;
                    } else if ((arrR[0].x + arrR[0].w - possibleHit[l].x <= fabs(ghPieceVelX) && ghPieceVelX < 0) ||
                               (possibleHit[l].x + possibleHit[l].w - arrR[0].x <= fabs(ghPieceVelX) && ghPieceVelX < 0)) {
                        arrR[0].x = possibleHit[l].x + possibleHit[l].w - arrR[0].w / 2;
                        sideOffsetY = -imRect.h / 2 + arrR[0].h / 2;
                        sideOffsetX = arrR[0].w / 2;
                    }

                    if (yVel > 5) {
                        yVel = 5;
                    } else if (yVel < -5) {
                        yVel = -5;
                    }

                    if (xVel > 5) {
                        xVel = 5;
                    } else if (xVel < -5) {
                        xVel = -5;
                    }

                    hit = true;
                }
            }
        }

    }

    //Adds to iteration variable to shoot out the next grappling hook piece
    s++;

    //Sets retrac to true to start retracting the grappling hook after nothing was hit
    if (s == arr.size()) {

        //To fix glitch were hit is true and s == arrsize() is true too
        if (hit){
            hit = false;
        }
        shoot = false;
        retrac = true;

    }

    //If something was hit, sets retrac to true to start retracting the grappling hook and pull the player to the object
    if (hit){
        shoot = false;
        retrac = true;

        //Track is set to s to keep track of the last grappling hook piece shot out, to set the players location to
        // that piece later
        track = s;

        //Resets the location of the last piece of the grappling hook that was shot out to the players location
        arrR[track].y = imRect.y + imRect.h / 2  - arrR[track].h / 2;
        arrR[track].x = imRect.x + imRect.w / 2  - arrR[track].w / 2;
    }
}

//Function for retracting the grappling hook
void retracting(int &s, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int &ghPieceVelY,
                int &ghPieceVelX, bool &hit, bool &retrac, int &track, SDL_Surface *test, int &yVel, int
                &xVel, bool mouseUp, int &sideOffsetY,
                vector<SDL_Rect> &hitObjects, int &sideOffsetX){

    //Variable to tell if the player is intersecting with an object in the level when retracting
    int noCancel;

    //I made this an if statement instead of a for loop so that the pieces don't instantly all go out at once before
    // displaying and so that everything else in the main loop also runs together with this. This goes from the last
    // element of the vector to the beginning.
    if (s >= 0) {

        //If something was hit
        if (hit) {

            //Sets each grappling hook piece location to the location of the one before it, while also adding velocity
            // of the player (So like if the player is falling, the grappling hook pieces will also fall), for the
            // pieces currently retracting and also blits them. The pieces are blitted in the opposite direction though,
            // so where the locations are set on the loop where b goes from the last element to 0, the blitting happens
            // in the loop where track - b goes from 0 to the last element so that it looks like the grappling hook
            // pieces are retracting to the player rather than the pieces retracting to the place where it hit on the
            // object
            for (int b = track; b > track - s; b--) {

                arrR[b].y = static_cast<int>(arrR[b - 1].y + (arrR[b].y - arrR[b - 1].y)
                                                             * 0.9 - static_cast<int>(yVel / 2));
                arrR[b].x = static_cast<int>(arrR[b - 1].x + (arrR[b].x - arrR[b - 1].x)
                                                             * 0.9 - xVel * 2);

                SDL_BlitSurface(arr[track - b], nullptr, test,
                                &arrR[track- b]);

            }

            //Sets player location to the last element currently retracting
            imRect.y = arrR[s].y - imRect.h / 2 + arrR[s].h / 2;
            imRect.x = arrR[s].x - imRect.w / 2 + arrR[s].w / 2;

            //Sets noCancel to false
            noCancel = false;

            //If the player is intersecting with an object noCancel is set to true
            for (auto & hitObject : hitObjects)
                if (SDL_HasIntersection(&imRect, &hitObject)){
                    noCancel = true;
                }


        } else {

            //If something wasn't hit, this retracts the grappling hook by setting the location of each grappling
            // hook piece to the one before it while also adding the velocity of the player (So like if the player is
            // falling, the grappling hook pieces will also fall), and also blits them at the same index.
            for (int b = 0; b < s; b++) {
                if (b == 0){
                    arrR[b].y = arrR[b + 1].y - yVel - arrR[b + 1].h / 2;
                    arrR[b].x = arrR[b + 1].x - xVel - arrR[b + 1].w / 2;
                } else {
                    arrR[b].y = arrR[b + 1].y - yVel;
                    arrR[b].x = arrR[b + 1].x - xVel;
                }
            }

            //Blits here so the hook piece of the grappling hook blits over the other pieces
            for (int h = s - 1; h >= 0; h--) {

                //Placeholder so that the rects don't get changed when blitted by the blit function
                SDL_Rect p = arrR[h];

                SDL_BlitSurface(arr[h], nullptr, test, &p);
            }

        }

    }

    //Tests if the player retracts all the way to the position where the object was hit by the hook, where all the grappling
    // hook pieces have been retracted.
    if (s <= 0) {

        //Sets the velocity of the player to 0, so they stay stuck there if the hook had hit something
        if (hit) {
            //Sets player location to the last element currently retracting
            imRect.y = arrR[s].y + sideOffsetY;
            imRect.x = arrR[s].x + sideOffsetX;
            yVel = 0;
            xVel = 0;
        }

        //Keeps setting the player velocity to zero until the mouse button is not being held, where retracting is set
        // to false and hit is set to false
        if (mouseUp) {
            retrac = false;
            hit = false;
            ghPieceVelY = 0;
            ghPieceVelX = 0;

            //So that it doesn't hit the enemies again after retracting
            arrR[0].x = -100;
            arrR[0].y = -100;
        }
    } else {

        //Sets player velocity to grappling hook piece velocity if the player lets go of the mouse button before the
        // hook retracts all the way, unless the player is intersecting with an object in the level.
        if (mouseUp && hit && !noCancel) {
            xVel = -ghPieceVelX;
            yVel = -ghPieceVelY;
            hit = false;
            ghPieceVelY = 0;
            ghPieceVelX = 0;

            //So that it doesn't hit the enemies again after retracting
            arrR[0].x = -100;
            arrR[0].y = -100;
        }

        //Subtracts from the iteration variable to move on to the next grappling hoo piece
        s--;
    }
}
