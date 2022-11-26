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
        // Declare rect of square
        SDL_Rect squareRect;

        // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
        squareRect.w = min(SCREEN_WIDTH, SCREEN_HEIGHT) / 8;
        squareRect.h = min(SCREEN_WIDTH, SCREEN_HEIGHT) / 8;

        // Square position: In the middle of the screen
        squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
        squareRect.y = 200;

        //Loads cat image
        SDL_Surface *im = loadImages("cat.png");

        //Loads circle image
        SDL_Surface *circle = loadImages("circle.png");

        //Loads circle2 image
        SDL_Surface *circle2 = loadImages("circle2.png");

        //Creates rectangle for cat image
        SDL_Rect imRect = {100, 100, 100, 100};

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

        //Angle from the character to the mouse position
        double angle;

        //Mouse coordinates
        int mouseX;
        int mouseY;

        //Bool to test if the mouse button is down
        bool mouseUp;

        //Bool for if grappling hook is shooting out
        bool shoot;

        //Bool for if the grappling hook is retracting
        bool retrac;

        //To test what side of an object the hook contacts
        int sideOffsetY = 0;
        int sideOffsetX = 0;

        //Iteration variable to keep track of grappling hook vector indexes as it shoots and retracts
        int s;

        //Bool for if the grappling hook collides with a rect
        bool hit = false;

        //To keep track of the last grappling hook rect to be shot out
        int track;

        //Vector holding grappling hook images
        vector<SDL_Surface *> arr;

        int u;

        for (u = 0; u < 15; u++) {
            if (u == 0) {
                arr.push_back(circle2);
            } else {
                arr.push_back(circle);
            }
        }

        //Vector holding grappling hook rects
        vector<SDL_Rect> arrR;

        SDL_Rect placeHolder = {0, 0, 50, 50};

        for (u = 0; u < 15; u++) {
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
        SDL_Rect squareRect2 = {0, 0, SCREEN_WIDTH, 100};
        SDL_Rect squareRect3 = {0, 700, SCREEN_WIDTH, 100};
        vector<SDL_Rect *> roomRects = {&squareRect, &squareRect2, &squareRect3};

        SDL_Rect squareRect4 = {SCREEN_WIDTH, 0, SCREEN_WIDTH, 100};
        SDL_Rect squareRect5 = {SCREEN_WIDTH, 700, SCREEN_WIDTH, 100};

        vector<SDL_Rect *> roomRects2 = {&squareRect4, &squareRect5};

        //To keep track of the current room the player is in
        int currRoom = 0;

        //To assign the next room to transition to
        int nextRoom = 0;

        //Speed the room switch will happen at
        int speed = 0;

        //Makes it so the transition doesn't happen immediately again in the opposite direction
        bool noSwitch;

        //Creates a vector of surfaces to blit into the Rect objects of the room object
        vector<SDL_Surface *> roomSkellSurfs = {loadImages("color.png"), loadImages("color.png"),
                                                loadImages("color.png")};
        vector<SDL_Surface *> roomSkellSurfs2 = {loadImages("color.png"), loadImages("color.png")};

        //Creates room objects
        Rooms room1 = *new Rooms(1, roomRects, roomSkellSurfs);
        Rooms room2 = *new Rooms(2, roomRects2, roomSkellSurfs2);

        //Vector of all the rooms
        vector<Rooms> roomsArr = {room1, room2};

        bool transition;

        int y = SCREEN_WIDTH;
        int x = SCREEN_WIDTH;

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
                                        sqrt(pow(((fabs(90 - angle) - 90) * -1), 2)
                                        + pow(((90 - fabs(angle)) * -1), 2)))) * 0.4);
                                ghPieceVelX = static_cast<int>((((90 - fabs(angle)) * -1) * (90 
                                        / sqrt(pow(((90 - fabs(angle)) * -1), 2)
                                        + pow(((fabs(90 - angle) - 90) * -1), 2)))) * 0.4);


                            } else {

                                //Calculates x and y velocity from angle when mouse is on the right side of the
                                // character
                                ghPieceVelY = static_cast<int>(((fabs(90 - angle) - 90) * (90 
                                        / sqrt(pow((fabs(90 - angle) - 90), 2)
                                        + pow((90 - fabs(angle)), 2)))) * 0.4);
                                ghPieceVelX = static_cast<int>(((90 - fabs(angle)) * (90 
                                        / sqrt(pow((90 - fabs(angle)), 2)
                                        + pow((fabs(90 - angle) - 90), 2)))) * 0.4);

                            }
                            
                            //Triggers shoot function
                            shoot = true;

                            //Resets iterator variable for shoot and retrac functions
                            s = 0;

                        }

                        break;

                    //User lets go of the mouse button
                    case SDL_MOUSEBUTTONUP:
                        mouseUp = true;
                }

                //Function for resizing the screen
                screenSizeChange(textRect, window);

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
                    } else {
                        down = false;
                    }

                    //Function for player movement
                    playerMovement(jump, up, left, right, down, yVel, xVel, imRect);

                    //Test if grappling hook is shooting
                    if (shoot) {

                        //Gets rects that can be hit with the hook in the current level
                        roomRects = roomsArr[currRoom].getRects();

                        //Function for shooting the grappling hook
                        shooting(s, arrR, arr, imRect, ghPieceVelY, ghPieceVelX, hit, shoot,
                                 retrac, track, test, roomRects, sideOffsetY, sideOffsetX);
                    }

                    //Test if grappling hook is retracting
                    if (retrac) {

                        //Function for retracting the grappling hook
                        retracting(s, arrR, arr, imRect, ghPieceVelY, ghPieceVelX, hit,
                                   retrac, track, test, yVel, xVel, mouseUp,
                                   sideOffsetY, roomRects, sideOffsetX);
                    }

                    //Applying gravity to the charter
                    yVel -= gravity;

                    //Applies y-axis velocity to the character
                    imRect.y -= yVel;

                    //Applies x-axis velocity to the character
                    imRect.x -= xVel;

                    if (imRect.x >= SCREEN_WIDTH && !noSwitch){
                        transition = true;
                        nextRoom = 1;
                        speed = 20;
                        y = SCREEN_WIDTH;
                    } else if (imRect.x + imRect.w <= 0 && !noSwitch){
                        transition = true;
                        nextRoom = -1;
                        speed = -20;
                        y = -SCREEN_WIDTH;
                    } else if (imRect.x + imRect.w > 0 && imRect.x < SCREEN_WIDTH){
                        noSwitch = false;
                    }

                } else {

                    vector<SDL_Rect *> currObjs = roomsArr[currRoom].getRects();
                    vector<SDL_Rect *> nextObjs = roomsArr[currRoom + nextRoom].getRects();
                    vector<SDL_Surface *> nextSurfs = roomsArr[currRoom + nextRoom].getSurfs();

                    if (switchRooms(currObjs, nextObjs, nextSurfs, imRect, y, x, speed, test)){
                        transition = false;
                        noSwitch = true;
                        currRoom = currRoom + nextRoom;
                    }
                }

                //Placeholder so that imRect does not get altered by the blitting function
                SDL_Rect placeH = imRect;

                //Blits cat image to test at the location, and showing the dimensions, of imRect (the image
                // rectangle)
                SDL_BlitSurface(im, nullptr, test, &placeH);

                //Tests blitting for room objects
                roomsArr[currRoom].updateRoom(test, textRect, imRect, yVel, xVel, jump,
                                              ghPieceVelY, ghPieceVelX);

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