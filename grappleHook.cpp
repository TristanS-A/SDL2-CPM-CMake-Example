//
// Created by tropi on 11/21/2022.
//

#include "grappleHook.h"
#include "SDL.h"
#include <algorithm>

void shooting(int &s, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int ghPieceVelY,
              int ghPieceVelX, bool &hit, bool &shoot, bool &retrac, int &track, SDL_Surface *test,
              SDL_Rect &squareRect){

    //Shoots out rectangles until hitting something or until shooting all the graphing hook pieces. I made this an if
    // statement instead of a for loop so that the pieces don't instantly all go out at once before displaying and so
    // that everything else in the main loop also runs together with this.
    if (s < arr.size()) {


        //Resets grappling hook rect locations to the player's location
        arrR[s].x = imRect.x + 100 / 2 - arrR[s].w / 2;
        arrR[s].y = imRect.y + 100 / 2 - arrR[s].h / 2;

        //Blits and adds force to each grappling piece currently shooting out
        for (int b = 0; b < s; b++) {

            SDL_BlitSurface(arr[b], nullptr, test, &arrR[b]);
            arrR[b].y += ghPieceVelY;
            arrR[b].x += ghPieceVelX;
        }

        //Test for if the hook intersects with something
        if (SDL_HasIntersection(&arrR[0], &squareRect)){
            hit = true;
        }

    }

    //Adds to iteration variable to shoot out the next grappling hook piece
    s++;

    //Sets retrac to true to start retracting the grappling hook after nothing was hit
    if (s == arr.size()) {
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

void retracting(int &s, vector<SDL_Rect> &arrR, vector<SDL_Surface *> arr, SDL_Rect &imRect, int ghPieceVelY,
                int ghPieceVelX, bool &hit, bool &shoot, bool &retrac, int &track, SDL_Surface *test,
                SDL_Rect &squareRect, int &yVel, int &xVel, bool mouseUp){

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
                                                             * 0.9 - yVel);
                arrR[b].x = static_cast<int>(arrR[b - 1].x + (arrR[b].x - arrR[b - 1].x)
                                                             * 0.9 - xVel);

                SDL_BlitSurface(arr[track - b], nullptr, test,
                                &arrR[track- b]);

            }

            //Sets player location to the last element currently retracting
            imRect.y = arrR[s].y + arrR[s].h / 2 - imRect.h / 2;
            imRect.x = arrR[s].x + arrR[s].w / 2 - imRect.w / 2;

        } else {

            //If something wasn't hit, this retracts the grappling hook by setting the location of each grappling
            // hook piece to the one before it while also adding the velocity of the player (So like if the player is
            // falling, the grappling hook pieces will also fall), and also blits them at the same index.
            for (int b = 0; b < s; b++) {

                SDL_BlitSurface(arr[b], nullptr, test, &arrR[b]);
                arrR[b].y = arrR[b + 1].y - yVel;
                arrR[b].x = arrR[b + 1].x - xVel;
            }

        }

    }

    //Tests if the player retracts all the way to the position where the object was hit by the hook, where all the grappling
    // hook pieces have been retracted.
    if (s <= 0) {

        //Sets the velocity of the player to 0 so they stay stuck there if the hook had hit something
        if (hit) {
            yVel = 0;
            xVel = 0;
        }

        //Keeps setting the player velocity to zero until the mouse button is not being held, where retracting is set
        // to false and hit is set to false
        if (mouseUp) {
            retrac = false;
            hit = false;
        }
    } else {

        //If something was hit and the mouse button stopped being held before the grappling hook retracted all the way,
        // the player takes the velocity that the grappling hook pieces were shot out at and applies it to the player,
        // launching the player
        if (mouseUp && hit) {
            xVel = -ghPieceVelX / 2;
            yVel = -ghPieceVelY / 2;
            hit = false;
        }

        //Subtracts from the iteration variable to move on to the next grappling hoo piece
        s--;
    }
}
