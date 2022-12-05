#define SDL_MAIN_HANDLED true
#define _USE_MATH_DEFINES
#include <iostream>
#include <algorithm>
#include <cmath>

#include "playerMovement.h"
#include "screenSizeChange.h"
#include "grappleHook.h"
#include "rooms.h"
#include "loadImages.h"
#include "roomSwitch.h"
#include "enemies.h"
#include "levels.h"
#include "handleCollision.h"

using namespace std;

#include <SDL.h>
#include <SDL_image.h>

#define SCREEN_WIDTH    1440
#define SCREEN_HEIGHT   810

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not be initialized!" << endl
             << "SDL_Error: " << SDL_GetError() << endl;
        return 0;
    }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
   if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
   {
       cout << "SDL can not disable compositor bypass!" << endl;
       return 0;
   }
#endif

   bool error;

    // Create window
    SDL_Window *window = SDL_CreateWindow("Basic C++ SDL project",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if(!window)
    {
        cout << "Window could not be created!" << endl
             << "SDL_Error: " << SDL_GetError() << endl;
        error = true;
    }
    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        cout << "Renderer could not be created!" << endl
             << "SDL_Error: " << SDL_GetError() << endl;
        error = true;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        cout << "SDL_Image could not be created!" << endl
             << "SDL_Error: " << SDL_GetError() << endl;
        error = true;
    }

    if (!error) {

        //Loads cat image
        SDL_Surface *im = loadImages("images/cat.png");

        //Loads circle image
        SDL_Surface *circle = loadImages("images/circle.png");

        //Loads circle2 image
        SDL_Surface *circle2 = loadImages("images/circle2.png");

        //Creates rectangle for cat image
        SDL_Rect imRect = {200, 200, 100, 100};

        //Death animation images
        vector<SDL_Surface *> deathAnimation;
        deathAnimation.push_back(loadImages("images/deathAnimation/ex1.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex2.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex3.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex4.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex5.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex6.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex7.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex8.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex9.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex10.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex11.png"));
        deathAnimation.push_back(loadImages("images/deathAnimation/ex12.png"));

        //Death index to display images
        int deathAnimationIndex = 0;

        //To play death animation at specific speed
        int deathCurrTime = static_cast<int>(SDL_GetTicks());
        int deathPrevTime = 0;

        //Curtain to fall after death
        SDL_Surface *curtain = loadImages("images/curtain.png");

        //To activate curtain to drop
        bool dropCurtain;

        //To activate curtain to rise
        bool raiseCurtain;

        //To move curtain
        int curtainOffset = -810;

        //Exit flag
        bool quit = false;

        //Movement flags
        bool left;
        bool right;
        bool up;
        bool down;

        //gravity
        int gravity = 1;

        //Character velocity's
        int xVel = 0;
        int yVel = 0;

        //Jump variable to tell when the character can jump
        bool jump = false;

        //For if the player dies
        bool dead = false;

        //Angle from the character to the mouse position
        double angle;

        //Mouse coordinates
        int mouseX;
        int mouseY;

        //Bool to test if the mouse button is down
        bool mouseUp;

        //Bool for if grappling hook is shooting out
        bool shoot = false;

        //Bool for if the grappling hook is retracting
        bool retrac;

        //To test what side of an object the hook contacts
        int sideOffsetY = 0;
        int sideOffsetX = 0;

        //Iteration variable to keep track of grappling hook vector indexes as it shoots and retracts
        int s;

        //Bool for if the grappling hook collides with a rect
        bool hit = false;

        //Bool to test if grapling hook collides with an enemie
        bool hitEnemie;

        //To keep track of the last grappling hook rect to be shot out
        int track;

        //Vector holding grappling hook images
        vector<SDL_Surface *> arr;

        int u;

        for (u = 0; u < 20; u++) {
            if (u == 0) {
                arr.push_back(circle2);
            } else {
                arr.push_back(circle);
            }
        }

        //Vector holding grappling hook rects
        vector<SDL_Rect> arrR;

        SDL_Rect placeHolder = {0, 0, 50, 50};

        for (u = 0; u < 20; u++) {
            arrR.push_back(placeHolder);
        }

        //Ints holding x and y velocity for each grappling hook piece
        int ghPieceVelX = 0;
        int ghPieceVelY = 0;

        //Set up for delta time FPS calculator
        Uint32 currTime;
        Uint32 prevTime = 0;

        //Creates surface that will be used for blitting
        SDL_Surface *test = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                                 32, 0, 0, 0, 0);

        //Creates texture from test surface to be able to render test surface and the images blitted to it with
        // renderer
        SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, test);

        //Creates Rect for test surface so that its dimensions can be rescaled and repositioned
        SDL_Rect textRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

        //Sets up key presses
        const Uint8 *keystates = SDL_GetKeyboardState(nullptr);

        //Vector holding all rects for a level
        SDL_Rect squareRect1 = {0, 0, SCREEN_WIDTH, 100};
        SDL_Rect squareRect2 = {0, 710, SCREEN_WIDTH, 100};
        SDL_Rect squareRect3 = {0, 100, 100, SCREEN_HEIGHT - 200};

        vector<SDL_Rect> roomRects = {squareRect1, squareRect2, squareRect3};

        squareRect1 = {0, 0, 500, 100};
        squareRect2 = {0 + 500 + 440, 0, 500, 100};
        squareRect3 = {0, 710, 500, 100};
        SDL_Rect squareRect4 = {0 + 500 + 440, 710, 500, 100};

        vector<SDL_Rect> roomRects2 = {{600, 300, 150, 150}, squareRect1, squareRect2, squareRect3, squareRect4};

        squareRect1 = {0, 0 + 710, 500, 100};
        squareRect2 = {500 + 440, 0 + 710, 500, 100};
        squareRect3 = {0, 0, SCREEN_WIDTH, 100};

        vector<SDL_Rect> roomRects3 = {squareRect1, squareRect2, squareRect3};

        squareRect1 = {0, 0, 500, 100};
        squareRect2 = {500 + 440, 0, 500, 100};
        squareRect3 = {0, 0 + 710, SCREEN_WIDTH, 100};

        vector<SDL_Rect> roomRects4 = {squareRect1, squareRect2, squareRect3};

        //To keep track of the current room the player is in
        int currRoom = 0;

        //To keep track of levels
        int currLevel = 0;

        //To tell if player is on the level select screen
        bool levelSelect = true;

        //To tell when the screen should change to levelSelect screen
        bool goToLevelSelScreen = false;

        //Checks if the player is entering a level
        bool enteringLevel = false;

        //To keep track of entering animation
        int enterIndex = -1;

        //Handles camera movement in levelSelectScreen
        int cameraOffset = 0;

        //Makes it so the transition doesn't happen immediately again in the opposite direction
        bool noSwitch;

        int paraBGx = 0;
        int paraBGy = 0;

        SDL_Surface *paraBG = loadImages("images/parallaxBG.png");

        SDL_Rect paraBGRect;

        //Placeholder for bg rect location
        SDL_Rect pHolder = {0, 0, 0, 0};

        //Checks if the user is exiting a room and what room, gets room number to travel to and the speed to transition
        // (Like in case the screen needs to change left or right
        vector<int> exitInfo;

        SDL_Surface * colorImage = loadImages("images/color.png");

        //Creates a vector of surfaces to blit into the Rect objects of the room object
        vector<SDL_Surface *> roomSkellSurfs = {colorImage, colorImage, colorImage};
        vector<SDL_Surface *> roomSkellSurfs2 = {colorImage, colorImage,
                                                 colorImage, colorImage, colorImage};

        vector<SDL_Rect> exits = {{SCREEN_WIDTH + 50, 0, 100, SCREEN_HEIGHT}};

        vector<SDL_Rect> placeHolderObsRects = {{500, 400, 100, 100}};

        vector<vector<SDL_Surface *>> placeHolderObsSurfs = {{colorImage, loadImages("images/color2.png")}};

        vector<bool> placeHolderObsHookable = {true};

        vector<SDL_Rect> levelSelectRects = {{0, 710, SCREEN_WIDTH, 100}};
        vector<SDL_Surface *> levelSelectSurfs = {loadImages("images/color2.png")};
        vector<SDL_Rect> levelDoors = {{200, 510, 200, 200}};
        vector<vector<SDL_Surface *>> levelDoorSurfs = {{loadImages("images/color2.png"), loadImages("images/color3.png"), loadImages("images/color.png")}};

        Enemies enemie1 = *new Enemies({100, 200, 100, 100}, {colorImage, loadImages("images/color2.png")}, loadImages("images/color3.png"), deathAnimation, 10, gravity);

        Enemies enemie2 = *new Enemies({500, 200, 100, 100}, {colorImage, loadImages("images/color2.png")}, loadImages("images/color3.png"), deathAnimation, 10, gravity);

        //Creates room objects
        Rooms room1 = *new Rooms({200, 605, 0, 0}, roomRects, roomSkellSurfs, exits, {{1, 40, SCREEN_WIDTH, -40}}, {}, {{}}, {}, {enemie1, enemie2}, loadImages("images/bg.png"));

        exits = {{-150, 0, 100, SCREEN_HEIGHT}, {500, -100, 440, 50}, {500, SCREEN_HEIGHT + 50, 440, 50}};

        Rooms room2 = *new Rooms({200, 605, 0, 0}, roomRects2, roomSkellSurfs2, exits, {{0, -40, -SCREEN_WIDTH, 40}, {2, -30, 30, -SCREEN_HEIGHT}, {3, 30, -30, SCREEN_HEIGHT}}, {}, {{}}, {}, {}, loadImages("images/bg.png"));

        exits = {{500, SCREEN_HEIGHT + 50, 440, 50}};

        Rooms room3 = *new Rooms({200, 605, 0, 0}, roomRects3, roomSkellSurfs2, exits, {{1, 30, -30, SCREEN_HEIGHT}}, placeHolderObsRects, placeHolderObsSurfs, placeHolderObsHookable, {}, loadImages("images/bg.png"));

        exits = {{500, -100, 440, 50}};

        Rooms room4 = *new Rooms({200, 605, 0, 0}, roomRects4, roomSkellSurfs, exits, {{1, -30, 30, -SCREEN_HEIGHT}}, {}, {{}}, {}, {}, loadImages("images/bg.png"));

        //Vector of all the rooms
        vector<Rooms> roomsArr = {room1, room2, room3, room4};

        Levels level1 = *new Levels(roomsArr, paraBG);

        vector<Levels> levels = {level1};

        bool transition = false;

        // Event loop
        while (!quit) {
            //Gets console ticks for delta time calculations
            currTime = SDL_GetTicks();

            //Set to 60 FPS
            if (currTime > prevTime + 1000 / 60) {
                prevTime = currTime;

                //Clears surface that were blitted to
                SDL_FillRect(test, nullptr, 0x000000);

                //Gets events
                SDL_Event e;

                // Wait indefinitely for the next available event
                //SDL_WaitEvent(&e);

                //I used poll instead of wait because the key presses had a delay with wait.
                SDL_PollEvent(&e);

                //Switch loop for events
                switch (e.type) {
                    // User requests quit
                    case SDL_QUIT:
                        quit = true;
                        break;

                    //Presses mouse button
                    case SDL_MOUSEBUTTONDOWN:

                        //To test if the mouse button is pressed and/or held down
                        mouseUp = false;

                        if (!transition && !dead) {
                            //So you cant fire the grappling hook while already firring it or retracting it
                            if (!shoot && !retrac) {

                                //Gets mouse location
                                SDL_GetMouseState(&mouseX, &mouseY);

                                //If the user changed the screen dimension, this will translate the mouse position to the
                                // new screen/window dimensions
                                mouseY = mouseY * (SCREEN_HEIGHT) / textRect.h - textRect.y * (SCREEN_HEIGHT) /
                                                                                 textRect.h;
                                mouseX = mouseX * (SCREEN_WIDTH) / textRect.w - textRect.x * (SCREEN_WIDTH) /
                                                                                textRect.w;

                                //Calculates the angle between the character and the mouse.
                                angle = -(atan(((imRect.y + 100 / 2.0) -
                                                (static_cast<double>(mouseY))) /
                                               ((imRect.x + 100 / 2.0) -
                                                static_cast<double>(mouseX))) *
                                          (180.0 / M_PI));

                                //Calculates the x and y velocity to be added to each piece of the grappling hook when
                                // shooting by checking if the mouse position is on the left or right of the character
                                // because I calculated the angle to be between -90 and 90 so if the mouse position is on
                                // the left side, it must be flipped. After calculating an x and y velocity though, with
                                // the x vel being 90 - fabs(angle) and the y vel being fabs(90 - angle) - 90 I had to
                                // convert these values to a circle, because graphing all the possible vels would
                                // show a cube rotated 90 degrees with the player in the center, and that was the max
                                // distance that the grappling hook would reach given the direction. So the rest of the
                                // math makes it so if you graphed all the possible vels, it would show a circle so that
                                // no matter what direction you shot the grappling hook in, it would always travel the
                                // same distance from the player. The * 0.4 is just a dampener that shortens the length
                                // that the hook shoots
                                if (mouseX <= (imRect.x + 100 / 2)) {

                                    //Calculates x and y velocity from angle when mouse is on the left side of the
                                    // character
                                    ghPieceVelY = static_cast<int>((((fabs(90 - angle) - 90) * -1) * (90 /
                                                                                                      sqrt(pow(
                                                                                                              ((fabs(90 -
                                                                                                                     angle) -
                                                                                                                90) *
                                                                                                               -1), 2)
                                                                                                           + pow(((90 -
                                                                                                                   fabs(angle)) *
                                                                                                                  -1),
                                                                                                                 2)))) *
                                                                   0.3);
                                    ghPieceVelX = static_cast<int>((((90 - fabs(angle)) * -1) * (90
                                                                                                 / sqrt(pow(
                                            ((90 - fabs(angle)) * -1), 2)
                                                                                                        + pow(((fabs(
                                            90 - angle) - 90) * -1), 2)))) * 0.3);


                                } else {

                                    //Calculates x and y velocity from angle when mouse is on the right side of the
                                    // character
                                    ghPieceVelY = static_cast<int>(((fabs(90 - angle) - 90) * (90
                                                                                               / sqrt(pow(
                                            (fabs(90 - angle) - 90), 2)
                                                                                                      + pow((90 -
                                                                                                             fabs(angle)),
                                                                                                            2)))) *
                                                                   0.3);
                                    ghPieceVelX = static_cast<int>(((90 - fabs(angle)) * (90
                                                                                          / sqrt(pow((90 - fabs(angle)),
                                                                                                     2)
                                                                                                 +
                                                                                                 pow((fabs(90 - angle) -
                                                                                                      90), 2)))) * 0.3);

                                }

                                //Triggers shoot function
                                shoot = true;

                                //Resets iterator variable for shoot and retrac functions
                                s = 0;

                            }
                        }

                        break;

                    //User lets go of the mouse button
                    case SDL_MOUSEBUTTONUP:
                        mouseUp = true;
                }

                //Function for resizing the screen
                screenSizeChange(textRect, window);

                //Tests if the player is in the level selecting area
                if (levelSelect){

                    if (!enteringLevel) {
                        //Gets key inputs
                        if ((keystates[SDL_SCANCODE_W])) {
                            up = true;
                        } else {
                            up = false;
                        }
                        if (keystates[SDL_SCANCODE_A]) {
                            left = true;
                        } else {
                            left = false;
                        }
                        if ((keystates[SDL_SCANCODE_D])) {
                            right = true;
                        } else {
                            right = false;
                        }
                        if ((keystates[SDL_SCANCODE_S])) {
                            down = true;
                            dead = true;
                        } else {
                            down = false;
                        }

                        //Function for player movement
                        playerMovement(jump, up, left, right, down, yVel, xVel, imRect);
                    }

                    //Placeholder for the parallax bg rect
                    paraBGRect = {paraBGx % 1440, paraBGy % 810, 0, 0};

                    //Blits parallax background
                    SDL_BlitSurface(paraBG, nullptr, test, &paraBGRect);

                    //Blits no-parallax bg
                    SDL_BlitSurface(roomsArr[currRoom].getBG(), nullptr, test, &pHolder);


                    //Applying gravity to the charter
                    yVel -= gravity;

                    //Applies y-axis velocity to the character
                    imRect.y -= yVel;

                    //Applies x-axis velocity to the character
                    imRect.x -= xVel;


                    //Makes it appear that the camera is following the player by moving everything but the player
                    if (imRect.x + imRect.w / 2 > SCREEN_WIDTH / 2 || cameraOffset > 0){

                        //Adds the offset to the parallax bg at a different rate than the rest to add the parallax effect
                        paraBGx += xVel / 2;

                        //Adds the offset to the non-parallax bg
                        pHolder.x = -cameraOffset;

                        //Adds the offset to the levelSelectRects
                        for (auto & levelSelectRect : levelSelectRects){
                            levelSelectRect.x += xVel;
                        }

                        //Adds the offset to the level doors
                        for (auto & levelDoor : levelDoors){
                            levelDoor.x += xVel;
                        }

                        //Keeps the player position stagnant
                        imRect.x += xVel;

                        //Adds to the camera offset
                        cameraOffset -= xVel;
                    }

                    //Goes through all doors in levelDoors
                    for (int d = 0; d < levelDoors.size(); d ++){

                        //Blits doors with placeholder
                        SDL_Rect doorHolder = levelDoors[d];
                        SDL_BlitSurface(levelDoorSurfs[d][0], &doorHolder, test, &doorHolder);

                        //Tests is player enters a door
                        if (SDL_HasIntersection(&levelDoors[d], &imRect) && up){

                            enteringLevel = true;

                            //So the player doesn't jump when entering a level or move
                            if (yVel > 0){
                                imRect.y += yVel * 2;
                                yVel = 0;
                            }
                            xVel = 0;

                            //Does door entering animation and then lowers the curtain
                            deathCurrTime = static_cast<int>(SDL_GetTicks());
                                if (enterIndex < static_cast<int>(levelDoorSurfs[d].size()) - 1) {
                                    if (deathCurrTime > deathPrevTime + 1000 / 5) {
                                        deathPrevTime = deathCurrTime;
                                        enterIndex++;
                                    }
                                } else {
                                    //Placeholder for curtain and blits it with blit function
                                    SDL_Rect curtainRect = {0, curtainOffset, 0, 0};
                                    SDL_BlitSurface(curtain, nullptr, test, &curtainRect);

                                    if (curtainOffset < 0){
                                        curtainOffset = static_cast<int>(curtainOffset / 1.1);

                                        //Check so the curtain doesn't lower more than the screen height
                                        if (curtainOffset > 0){
                                            curtainOffset = 0;
                                        }
                                    } else {

                                        //Gets curtain ready to raise
                                        curtainOffset = 5;
                                        currLevel = d;
                                        levels[currLevel].resetLevel();
                                        raiseCurtain = true;
                                        dropCurtain = false;
                                        up = false;
                                        dead = false;
                                        levelSelect = false;
                                    }
                                }

                            //Blits animation over door
                            SDL_BlitSurface(levelDoorSurfs[d][enterIndex], &doorHolder, test, &doorHolder);
                        }
                    }

                    //So that when testing for multiple rect collisions, it doesnt reset the x velocity before testing
                    // all the possible collisions
                    bool waitToSet = false;

                    //This is a placeholder so that the actual rect position does not get changed by the blit function
                    SDL_Rect playerHolder = imRect;

                    SDL_BlitSurface(im, nullptr, test, &playerHolder);

                    //Blits level select room rects
                    for (int h = 0; h < levelSelectRects.size(); h++){
                        handleCollision(levelSelectRects[h], imRect, yVel, xVel, waitToSet, jump);

                        //This is a placeholder so that the actual rect position does not get changed by the blit function
                        SDL_Rect holder = levelSelectRects[h];

                        SDL_BlitSurface(levelSelectSurfs[h], &holder, test, &holder);

                    }

                    //Sets xVel to 0 after colliding with something
                    if (waitToSet){
                        xVel = 0;
                    }

                    //Blits curtain over everything
                    if (enteringLevel){
                        //Placeholder for curtain and blits it with blit function
                        SDL_Rect curtainRect = {0, curtainOffset, 0, 0};
                        SDL_BlitSurface(curtain, nullptr, test, &curtainRect);
                    }

                    //For raising the curtain after exiting level.
                    if (raiseCurtain){
                        if (curtainOffset > -810) {

                            //So you can't enter until curtain has been risen
                            up = false;

                            //Raises curtain
                            curtainOffset = -static_cast<int>(fabs(curtainOffset * 1.39));

                            //Curtain placeholder and blit function
                            SDL_Rect curtainRect = {0, curtainOffset, 0, 0};
                            SDL_BlitSurface(curtain, nullptr, test, &curtainRect);

                        } else {

                            //Resets the level entering variables, so you can enter again.
                            enteringLevel = false;
                            enterIndex = -1;

                            goToLevelSelScreen = false;

                            //Resets curtain after raising all the way
                            curtainOffset = -810;
                            raiseCurtain = false;

                        }
                    }

                } else {

                    if (!dead) {
                        if (!transition) {

                            //Gets key inputs
                            if ((keystates[SDL_SCANCODE_W])) {
                                up = true;
                            } else {
                                up = false;
                            }
                            if (keystates[SDL_SCANCODE_A]) {
                                left = true;
                            } else {
                                left = false;
                            }
                            if ((keystates[SDL_SCANCODE_D])) {
                                right = true;
                            } else {
                                right = false;
                            }
                            if ((keystates[SDL_SCANCODE_S])) {
                                down = true;
                                dead = true;
                            } else {
                                down = false;
                            }
                            if ((keystates[SDL_SCANCODE_R])) {
                                goToLevelSelScreen = true;
                                dropCurtain = true;
                            }


                            //Function for player movement
                            playerMovement(jump, up, left, right, down, yVel, xVel, imRect);

                            //Placeholder for the parallax bg rect
                            paraBGRect = {paraBGx % 1440, paraBGy % 810, 0, 0};

                            //Blits parallax background
                            SDL_BlitSurface(paraBG, nullptr, test, &paraBGRect);

                            //Blits no-parallax bg
                            SDL_BlitSurface(roomsArr[currRoom].getBG(), nullptr, test, &pHolder);

                            //Test if grappling hook is shooting
                            if (shoot) {

                                //Gets rects that can be hit with the hook in the current level
                                roomRects = levels[currLevel].getRoom().getRects();

                                //Function for shooting the grappling hook
                                shooting(s, arrR, arr, imRect, ghPieceVelY, ghPieceVelX, hit, shoot,
                                         retrac, track, test, roomRects, levels[currLevel].getRoom().getHittableRects(),
                                         levels[currLevel].getRoom().getHitTest(), sideOffsetY, sideOffsetX, yVel, xVel,
                                         hitEnemie);
                            }

                            //Test if grappling hook is retracting
                            if (retrac) {

                                //Function for retracting the grappling hook
                                retracting(s, arrR, arr, imRect, ghPieceVelY, ghPieceVelX, hit,
                                           retrac, track, test, yVel, xVel, mouseUp,
                                           sideOffsetY, roomRects, sideOffsetX, hitEnemie);
                            }

                            //Applying gravity to the charter
                            yVel -= gravity;

                            //Applies y-axis velocity to the character
                            imRect.y -= yVel;

                            //Applies x-axis velocity to the character
                            imRect.x -= xVel;

                        }
                    }

                    levels[currLevel].levelUpdate(test, imRect, im, right, left, transition, shoot,
                                                  retrac, hit, yVel, xVel, textRect, jump,
                                                  ghPieceVelY, ghPieceVelX, dead, arrR, arr, track,
                                                  sideOffsetX, sideOffsetY, mouseUp, s, hitEnemie,
                                                  deathAnimation, deathAnimationIndex, dropCurtain, raiseCurtain,
                                                  curtainOffset, curtain, goToLevelSelScreen, levelSelect, deathCurrTime, deathPrevTime,
                                                  paraBGRect, paraBGx, paraBGy);
                }

                //Updates text texture into a texture, so it can be rendered with new blit info
                SDL_UpdateTexture(text, nullptr, test->pixels, test->pitch);

            }
            // Initialize renderer color white for the background
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            // Clear screen
            SDL_RenderClear(renderer);

            //Prepares text to be rendered at the location of textRect and also resizes/soft scales it to the
            // dimensions of textRect
            SDL_RenderCopy(renderer, text, nullptr, &textRect);

            // Set renderer color red to draw the square
//            SDL_SetRenderDrawColor(renderer, 0xFF, 0x0F, 0x00, 0xFF);
//
//            // Draw filled square
//            SDL_RenderFillRect(renderer, &squareRect);

            // Update screen
            SDL_RenderPresent(renderer);
        }

        // Destroy renderer
        SDL_DestroyRenderer(renderer);

        //Frees surfaces
        SDL_FreeSurface(test);
        SDL_FreeSurface(im);

        // Destroy window
        SDL_DestroyWindow(window);
    }

    // Quit SDL
    SDL_Quit();

    return 0;
}