#define SDL_MAIN_HANDLED true
#include <iostream>
#include <algorithm>
#include <cmath>

#include "playerMovement.h"
#include "screenSizeChange.h"

using namespace std;

#include <SDL.h>

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

    if (!error) {
        // Declare rect of square
        SDL_Rect squareRect;

        // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
        squareRect.w = min(SCREEN_WIDTH, SCREEN_HEIGHT) / 8;
        squareRect.h = min(SCREEN_WIDTH, SCREEN_HEIGHT) / 8;

        // Square position: In the middle of the screen
        squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
        squareRect.y = 100;

        //Loads cat image
        SDL_Surface *im = SDL_LoadBMP("cat.bmp");
        if (im == nullptr) {
            printf("Unable to load image %s! SDL Error: %s\n",
                   "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
        }
        //Loads circle image
        SDL_Surface *circle = SDL_LoadBMP("circle.bmp");
        if (circle == nullptr) {
            printf("Unable to load image %s! SDL Error: \n ", SDL_GetError());
        }

        //Loads circle2 image
        SDL_Surface *circle2 = SDL_LoadBMP("circle2.bmp");
        if (circle2 == nullptr) {
            printf("Unable to load image %s! SDL Error: \n ", SDL_GetError());
        }

        //Creates rectangle for cat image
        SDL_Rect imRect = {0, 0, 100, 100};

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

        //Iteration variable to keep track of grappling hook vector indexes as it shoots and retracts
        int s;

        //Bool for if the grappling hook collides with a rect
        bool hit = false;

        //
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
        int ghPieceVelX;
        int ghPieceVelY;

        //Set up for delta time FPS calculator
        Uint32 currTime;
        Uint32 prevTime = 0;

        //Creates surface that will be used for blitting
        SDL_Surface *test = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

        //Creates texture from test surface to be able to render test surface and the images blitted to it with renderer
        SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, test);

        //Creates Rect for test surface so that its dimensions can be rescaled and repositioned
        SDL_Rect textRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

        //Sets up key presses
        const Uint8 *keystates = SDL_GetKeyboardState(nullptr);

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

                        mouseUp = false;

                        //So you cant fire the grappling hook while already firring it or retracting it
                        if (!shoot && !retrac) {

                            //Gets mouse location
                            SDL_GetMouseState(&mouseX, &mouseY);

                            //If the user changed the screen dimension, this will translate the mouse position to the
                            // new screen/window dimensions
                            mouseY = mouseY * (SCREEN_HEIGHT) / textRect.h - textRect.y * (SCREEN_HEIGHT) / textRect.h;
                            mouseX = mouseX * (SCREEN_WIDTH) / textRect.w - textRect.x * (SCREEN_WIDTH) / textRect.w;
                            cout << (SCREEN_WIDTH) / textRect.w << " " << (SCREEN_HEIGHT) / textRect.h << " \n";

                            //Calculates the angle between the character and the mouse.
                            angle = -(atan(((imRect.y + 100 / 2.0) -
                                            (static_cast<double>(mouseY))) /
                                           ((imRect.x + 100 / 2.0) -
                                            static_cast<double>(mouseX))) *
                                      (180.0 / M_PI));

                            //Calculates the x and y velocity to be added to each piece of the grappling hook when
                            // shooting by checking if the mouse position is on the left or right of the character
                            // (on the left or right side of the x-axis) because I calculated the angle to be
                            // between -90 and 90 so if the mouse position is on the left side, it must be flipped.
                            if (mouseX <= (imRect.x + 100 / 2)) {

                                //Calculates x and y velocity from angle when mouse is on the left side of the
                                // character
                                ghPieceVelY = -static_cast<int>((fabs(90 - angle) - 90) * 0.6);
                                ghPieceVelX = -static_cast<int>((fabs(-1 * (90 - fabs(angle)))) * 0.6);

                            } else {

                                //Calculates x and y velocity from angle when mouse is on the right side of the
                                // character
                                ghPieceVelY = static_cast<int>((fabs(90 - angle) - 90) * 0.6);
                                ghPieceVelX = static_cast<int>((fabs((90 - fabs(angle)))) * 0.6);

                            }

                            shoot = true;
                            s = 0;

                        }

                        break;
                    case SDL_MOUSEBUTTONUP:
                        mouseUp = true;
                }

                screenSizeChange(textRect, window);

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

                playerMovement(jump, up, left, right, down, yVel, xVel, imRect);

                if (shoot) {
                    if (s < arr.size() && !hit) {

                        //Resets grappling hook rects location
                        arrR[s].x = imRect.x + 100 / 2 - arrR[s].w / 2;
                        arrR[s].y = imRect.y + 100 / 2 - arrR[s].h / 2;

                        for (int b = 0; b < s; b++) {

                            SDL_BlitSurface(arr[b], nullptr, test, &arrR[b]);
                            arrR[b].y += ghPieceVelY;
                            arrR[b].x += ghPieceVelX;
                        }

                        if (SDL_HasIntersection(&arrR[0], &squareRect)){
                            hit = true;
                        }

                    } else if (!hit){
                        for (int k = 0; k < s; k++) {
                            SDL_BlitSurface(arr[k], nullptr, test, &arrR[k]);
                        }
                    }

                    s++;

                    if (s == arr.size()) {
                        shoot = false;
                        retrac = true;
                        s--;
                        track = s;
                    }
                    if (hit){
                        shoot = false;
                        retrac = true;
                        track = s;
                    }
                }

                if (retrac) {
                    if (s >= 0) {
                        if (hit) {

                            for (int b = track; b > track - s; b--) {

                                SDL_BlitSurface(arr[track - b], nullptr, test, &arrR[track - b]);

                                arrR[b].y = arrR[b - 1].y - yVel;
                                arrR[b].x = arrR[b - 1].x - xVel;
                            }

                            imRect.y = arrR[s].y + arrR[s].h / 2 - imRect.h / 2;
                            imRect.x = arrR[s].x + arrR[s].w / 2 - imRect.w / 2;

                        } else {

                            for (int b = 0; b < s; b++) {

                                SDL_BlitSurface(arr[b], nullptr, test, &arrR[b]);
                                arrR[b].y = arrR[b + 1].y - yVel;
                                arrR[b].x = arrR[b + 1].x - xVel;
                            }

                        }


                    } else {

                        for (int k = s; k >= s; k--) {
                            SDL_BlitSurface(arr[k], nullptr, test, &arrR[k]);
                        }
                    }

                    if (s <= 0) {
                        if (hit) {
                            yVel = 0;
                            xVel = 0;
                        }
                        if (mouseUp) {
                            retrac = false;
                            hit = false;
                        }
                    } else {

                        if (mouseUp && hit) {
                            xVel = -ghPieceVelX / 2;
                            yVel = -ghPieceVelY / 2;
                            hit = false;
                        }
                        s--;
                    }
                }

                //Applying gravity to the charter
                yVel -= gravity;

                //Applies y-axis velocity to the character
                imRect.y -= yVel;

                //Applies x-axis velocity to the character
                imRect.x -= xVel;

                //Test for when the charter is grounded so the character stops moving by setting yVel to 0
                if (imRect.y > 500) {
                    yVel = 0;
                    xVel = 0;
                    imRect.y = 500;

                    //Sets jump to true so player can jump after touching thr ground and not while in the air
                    jump = true;
                }

                //Blits cat image to test at the location, and showing the dimensions, of imRect (the image
                // rectangle)
                SDL_BlitSurface(im, nullptr, test, &imRect);

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
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x0F, 0x00, 0xFF);

            // Draw filled square
            SDL_RenderFillRect(renderer, &squareRect);

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