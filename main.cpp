/*Author: Tristan Schonfeldt-Aultman
Class : CSI-140-05/06
Assignment : Final Project
Due Date : Dec 11 at 11:59pm - Dec 11 4:40pm


Description :

    Submission for the Final Project as alternative individual assignment


Certification of Authenticity :

    I certify that this is entirely my own work, except where I have given fully - documented
    references to the work of others.I understand the definition and consequences of plagiarism and
    acknowledge that the assessor of this assignment may, for the purpose of assessing this assignment :

    Reproduce this assignmentand provide a copy to another member of academic staff;and /or
    Communicate a copy of this assignment to a plagiarism checking service(which may then retain
    a copy of this assignment on its database for the purpose of future plagiarism checking)*/

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
#include "enemies.h"
#include "levels.h"
#include "handleCollision.h"
#include "globalVariables.h"

using namespace std;

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define SCREEN_WIDTH    1440
#define SCREEN_HEIGHT   810

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

        //------------------------Creation and assignment of variables and vectors------------------------

        //Loads circle image for grappling hook body pieces
        SDL_Surface *circle = loadImages("images/circle.png");

        //Loads circle2 image for the hook of the grappling hook
        SDL_Surface *circle2 = loadImages("images/circle2.png");

        //Creates rectangle for player image and is called imRect for image Rectangle
        SDL_Rect imRect = {200, -200, 100, 100};

        //The health of the player
        int playerHealth = 50;

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

        //For if the player dies
        bool dead = false;

        //Angle from the character to the mouse position
        double angle;

        //Mouse coordinates
        int mouseX;
        int mouseY;

        //Bool to test if the mouse button is down
        bool mouseUp;

        //To test what side of an object the hook contacts
        int sideOffsetY = 0;
        int sideOffsetX = 0;

        //Bool to test if grapling hook collides with an enemie
        bool hitEnemie;

        //To keep track of the last grappling hook rect to be shot out
        int track;

        //Vector holding grappling hook images
        vector<SDL_Surface *> arr;

        int u;

        //Filling grappling hook vector with grappling hook piece images
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

        //Filling grappling hook rect vector with grappling hook piece rects
        for (u = 0; u < 20; u++) {
            arrR.push_back(placeHolder);
        }

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

        //Loads parallax bg
        SDL_Surface *paraBG = loadImages("images/parallaxBG.png");

        SDL_Surface *levSelFloor = loadImages("images/levSelFloor.png");
        SDL_Surface *levSelBG = loadImages("images/bgLevSelect.png");

        SDL_Rect paraBGRect;

        //Placeholder for bg rect location
        SDL_Rect pHolder = {0, 0, 0, 0};

        //Checks if the user is exiting a room and what room, gets room number to travel to and the speed to transition
        // (Like in case the screen needs to change left or right
        vector<vector<int>> exitInfo;

        //Loads level door textures
        SDL_Surface * door1 = loadImages("images/door1.png");
        SDL_Surface *door2 = loadImages("images/door2.png");
        SDL_Surface *door3 = loadImages("images/door3.png");
        SDL_Surface *door4 = loadImages("images/door4.png");
        SDL_Surface *door5 = loadImages("images/door5.png");

        //Loads level textures
        SDL_Surface * groundText = loadImages("images/ground.png");
        SDL_Surface *rockPlatform = loadImages("images/rockPlat.png");
        SDL_Surface *spikes = loadImages("images/spikes.png");
        vector<SDL_Surface *> lava = {loadImages("images/lava1.png"), loadImages("images/lava2.png"), loadImages("images/lava3.png"), loadImages("images/lava4.png"), loadImages("images/lava5.png"), loadImages("images/lava6.png"), loadImages("images/lava2.png")};

        //Load enemie images
        SDL_Surface * enemieWalk1L = loadImages("images/enemieWalk1L.png");
        SDL_Surface * enemieWalk2L = loadImages("images/enemieWalk2L.png");
        SDL_Surface * enemieWalk1R = loadImages("images/enemieWalk1R.png");
        SDL_Surface * enemieWalk2R = loadImages("images/enemieWalk2R.png");
        SDL_Surface * enemieHurtL = loadImages("images/hurtL.png");
        SDL_Surface * enemieHurtR = loadImages("images/hurtR.png");

        //Load boss images
        SDL_Surface * bossWalk1L = loadImages("images/bossWalkL1.png");
        SDL_Surface * bossWalk2L = loadImages("images/bossWalkL2.png");
        SDL_Surface * bossWalk1R = loadImages("images/bossWalkR1.png");
        SDL_Surface * bossWalk2R = loadImages("images/bossWalkR2.png");
        SDL_Surface * bossHurtL = loadImages("images/bossHurtL.png");
        SDL_Surface * bossHurtR = loadImages("images/bossHurtR.png");

        //Sets up SDL_Mixer Audio
        int audio_rate = 22050;
        Uint16 audio_format = AUDIO_S16SYS;
        int audio_channels = 2;
        int audio_buffers = 4096;

        //Initializes/Opens mixer audio
        if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0){
            fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError()); exit(1);
        }

        //Loads sounds
        titleMusic = (Mix_LoadMUS("sounds/titleSong.wav"));
        caveMusic = (Mix_LoadMUS("sounds/caveMusic.wav"));
        hitSound = Mix_LoadWAV("sounds/hit.wav");
        chest = Mix_LoadWAV("sounds/chest.wav");
        enemiehit = Mix_LoadWAV("sounds/enemiehit.wav");
        jumpSound = Mix_LoadWAV("sounds/jump.wav");
        woossh = Mix_LoadWAV("sounds/woosh.wav");
        explosion = Mix_LoadWAV("sounds/explosion.wav");
        door = Mix_LoadWAV("sounds/door.wav");

        //Checks if sounds loaded
        if (!titleMusic){
            cout << "Failed to titleMusic sound\n";
        }
        if (!caveMusic){
            cout << "Failed to caveMusic sound\n";
        }
        if (!chest){
            cout << "Failed to chest sound\n";
        }
        if (!hitSound){
            cout << "Failed to hit sound\n";
        }
        if (!enemiehit){
            cout << "Failed to enemiehit sound\n";
        }
        if (!jumpSound){
            cout << "Failed to load sound\n";
        }
        if (!woossh){
            cout << "Failed to load sound\n";
        }
        if (!explosion){
            cout << "Failed to load sound\n";
        }
        if (!door){
            cout << "Failed to load door sound\n";
        }

        //Starts playing beginning music
        Mix_PlayMusic(titleMusic, -1);

        //Creates rect exits into new rooms from the room
        vector<SDL_Rect> exits = {{SCREEN_WIDTH + 50, 0, 100, SCREEN_HEIGHT}};

        //A vector to hold info for the exits like transition speed and to what room the player is traveling to and
        // corresponds to the index in the exit rects vector.
        exitInfo = {{1, 40, SCREEN_WIDTH, -40}};

        //Vector of obstacle rects that can kill the player in a room
        vector<SDL_Rect> placeHolderObsRects;

        //Vector or surfaces for the obstacles in a level
        vector<vector<SDL_Surface *>> placeHolderObsSurfs;

        //Vector of bools to tell is the player can grapple onto the obstacles in a room
        vector<bool> placeHolderObsHookable;

        //Vector of rects for the level select room
        vector<SDL_Rect> levelSelectRects = {{0, 710, SCREEN_WIDTH * 2 + 50, 100}, {0, 0, 100, SCREEN_HEIGHT}, {SCREEN_WIDTH * 2 - 100, 0, 150, SCREEN_HEIGHT}};

        //Vector of surfaces to blit onto the rects in the level select room
        vector<SDL_Surface *> levelSelectSurfs = {levSelFloor, groundText, groundText};

        //Vector of rects for level doors that take you to a level in the level select room
        vector<SDL_Rect> levelDoors = {{2400, 510, 200, 200}};

        //Vector of surfaces to blit to the level doors in the level select room
        vector<vector<SDL_Surface *>> levelDoorSurfs = {{door1, door2, door3, door4, door5}};

        //Creating enemies to be put in a vector of enemies in a room
        Enemies enemie1 = *new Enemies({200, 610, 100, 100}, {enemieWalk1L, enemieWalk2L}, {enemieWalk1R, enemieWalk2R}, enemieHurtL, enemieHurtR, deathAnimation, 10, gravity);
        Enemies enemie2 = *new Enemies({1200, 610, 100, 100}, {enemieWalk1L, enemieWalk2L}, {enemieWalk1R, enemieWalk2R}, enemieHurtL, enemieHurtR, deathAnimation, 10, gravity);

        //Commonly used rects for a room
        SDL_Rect roof = {0, 0, SCREEN_WIDTH, 100};
        SDL_Rect floor = {0, 710, SCREEN_WIDTH, 100};
        SDL_Rect leftWall = {0, 0, 100, SCREEN_HEIGHT};
        SDL_Rect rightWall = {SCREEN_WIDTH - 100, 0, 100, SCREEN_HEIGHT};
        SDL_Rect rightWallExit = {SCREEN_WIDTH - 200, 0, 200, 500};
        SDL_Rect leftWallExit = {0, 0, 200, 500};

        //More custom shapes that will be resigned
        SDL_Rect squareRect1;
        SDL_Rect squareRect2;
        SDL_Rect squareRect3;
        SDL_Rect squareRect4;
        SDL_Rect squareRect5;

        //Vector holding all rects for a room
        vector<SDL_Rect> roomRects = {roof, floor, leftWall, rightWallExit};

        //Creates a vector of surfaces to blit into the Rect objects of the room objects
        vector<SDL_Surface *> roomSurfs = {groundText, rockPlatform, groundText, groundText};

        //Vector for the exit chest rect
        vector<SDL_Rect> chestRect;

        //Bool to tell if the rooms are transitioning from the current room to the next one
        bool transition = false;

        //--------------------------------Creation of rooms--------------------------------

        //Create room object
        Rooms room1 = *new Rooms({200, 605, 0, 0}, roomRects, roomSurfs, exits, exitInfo, {}, {{}}, {}, {}, loadImages("images/bg.png"), chestRect);

        //Reassigns exits for new room exits for the next room
        exits = {{-150, 0, 100, SCREEN_HEIGHT}, {500, SCREEN_HEIGHT + 50, 440, 50}, {SCREEN_WIDTH + 50, 500, 100, 310}};
        exitInfo = {{0, -40, -SCREEN_WIDTH, 40}, {2, 30, -30, SCREEN_HEIGHT}, {3, 40, SCREEN_WIDTH, -40}};

        //Rects of next room
        squareRect1 = {0, 710, 500, 100};
        squareRect2 = {500 + 440, 710, 500, 100};
        squareRect3 = {SCREEN_WIDTH / 2 - 150, 500, 300, 50};

        //Vector holding all rects of the next room
        roomRects = {squareRect3, roof, squareRect1, squareRect2, leftWallExit, rightWallExit};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {rockPlatform, groundText, rockPlatform, rockPlatform, groundText, groundText};

        //Assigns rects, surfaces, and tests if the player can hook onto the obstacle for obstacles in the next room
        placeHolderObsRects = {{100, 100, SCREEN_WIDTH - 200, 100}};
        placeHolderObsSurfs = {{spikes}};
        placeHolderObsHookable = {true};

        //Creates next room object
        Rooms room2 = *new Rooms({200, 605, 0, 0}, roomRects, roomSurfs, exits, exitInfo, placeHolderObsRects, placeHolderObsSurfs, placeHolderObsHookable, {}, loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{500, -100, 440, 50}};
        exitInfo = {{1, -30, 30, -SCREEN_HEIGHT}};

        //Rects for next room
        squareRect1 = {0, 0, 500, 100};
        squareRect2 = {500 + 440, 0, 500, 100};
        squareRect3 = {400, 100, 100, 200};
        squareRect4 = {500 + 440, 100, 100, 200};

        //Vector holding rect for the next room
        roomRects = {squareRect1, squareRect2, floor, leftWall, rightWall, squareRect3, squareRect4};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, groundText, rockPlatform, groundText, groundText, groundText, groundText};

        //Creates next room object
        Rooms room3 = *new Rooms({SCREEN_WIDTH / 2 - 50, 605, 0, 0}, roomRects, roomSurfs, exits, exitInfo, {}, {{}}, {}, {enemie1, enemie2}, loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{-150, 0, 100, SCREEN_HEIGHT}, {SCREEN_WIDTH + 50, 0, 100, 310}};
        exitInfo = {{1, -40, -SCREEN_WIDTH, 40}, {4, 40, SCREEN_WIDTH, -40}};

        squareRect1 = {SCREEN_WIDTH - 300, 300, 300, SCREEN_HEIGHT - 300};
        squareRect2 = {200, 450, 200, 50};
        squareRect3 = {650, 300, SCREEN_WIDTH - 650, 50};

        //Vector holding rect for the next room
        roomRects = {roof, floor, squareRect3, leftWallExit, squareRect1, squareRect2};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, rockPlatform, groundText, groundText, groundText, groundText};

        Enemies enemie3 = *new Enemies({800, 610, 100, 100}, {enemieWalk1L, enemieWalk2L}, {enemieWalk1R, enemieWalk2R}, enemieHurtL, enemieHurtR, deathAnimation, 5, gravity);
        //Creates next room object
        Rooms room4 = *new Rooms({200, 605, 0, 0}, roomRects, roomSurfs, exits, exitInfo, {}, {{}}, {}, {enemie3},loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{-150, 0, 100, SCREEN_HEIGHT}, {SCREEN_WIDTH + 50, 0, 100, SCREEN_HEIGHT}};
        exitInfo = {{3, -40, -SCREEN_WIDTH, 40}, {5, 40, SCREEN_WIDTH, -40}};

        squareRect1 = {0, 300, 200, SCREEN_HEIGHT - 200};
        squareRect2 = {SCREEN_WIDTH - 200, 300, 200, SCREEN_HEIGHT - 300};
        squareRect3 = {400, 600, 200, 50};
        squareRect4 = {800, 450, 200, 50};

        //Vector holding rect for the next room
        roomRects = {squareRect1, squareRect2, squareRect3, squareRect4, roof};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, groundText, groundText, groundText, groundText};

        //Assigns rects, surfaces, and tests if the player can hook onto the obstacle for obstacles in the next room
        placeHolderObsRects = {{200, 700, SCREEN_WIDTH - 400, 200}};
        placeHolderObsSurfs = {lava};
        placeHolderObsHookable = {false};

        Enemies enemie4 = *new Enemies({SCREEN_WIDTH - 100, 200, 100, 100}, {enemieWalk1L, enemieWalk2L}, {enemieWalk1R, enemieWalk2R}, enemieHurtL, enemieHurtR, deathAnimation, 5, gravity);

        Rooms room5 = *new Rooms({100, 200, 0, 0}, roomRects, roomSurfs, exits, exitInfo, placeHolderObsRects, placeHolderObsSurfs, placeHolderObsHookable, {enemie4},loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{-150, 0, 100, SCREEN_HEIGHT}, {SCREEN_WIDTH + 50, 0, 100, SCREEN_HEIGHT}};
        exitInfo = {{4, -40, -SCREEN_WIDTH, 40}, {6, 40, SCREEN_WIDTH, -40}};

        squareRect1 = {0, 300, 200, SCREEN_HEIGHT - 150};
        squareRect2 = {SCREEN_WIDTH - 400, 660, 400, SCREEN_HEIGHT - 300};

        //Vector holding rect for the next room
        roomRects = {squareRect1, squareRect2, rightWallExit, roof};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, groundText, groundText, groundText};

        //Assigns rects, surfaces, and tests if the player can hook onto the obstacle for obstacles in the next room
        placeHolderObsRects = {{200, 700, SCREEN_WIDTH - 600, 200}};
        placeHolderObsSurfs = {lava};
        placeHolderObsHookable = {false};

        Rooms room6 = *new Rooms({100, 200, 0, 0}, roomRects, roomSurfs, exits, exitInfo, placeHolderObsRects, placeHolderObsSurfs, placeHolderObsHookable, {},loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{-150, 0, 100, SCREEN_HEIGHT}, {SCREEN_WIDTH + 50, 0, 100, SCREEN_HEIGHT}};
        exitInfo = {{5, -40, -SCREEN_WIDTH, 40}, {7, 40, SCREEN_WIDTH, -40}};

        squareRect1 = {0, 660, 300, 500};
        squareRect2 = {SCREEN_WIDTH - 400, 660, 400, 500};

        //Vector holding rect for the next room
        roomRects = {leftWallExit, squareRect1, rightWallExit, squareRect2, roof};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, groundText, groundText, groundText, groundText};

        //Assigns rects, surfaces, and tests if the player can hook onto the obstacle for obstacles in the next room
        placeHolderObsRects = {{200, 700, SCREEN_WIDTH - 600, 200}};
        placeHolderObsSurfs = {lava};
        placeHolderObsHookable = {false};

        Rooms room7 = *new Rooms({200, 555, 0, 0}, roomRects, roomSurfs, exits, exitInfo, placeHolderObsRects, placeHolderObsSurfs, placeHolderObsHookable, {},loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{-150, 0, 100, SCREEN_HEIGHT}, {0, SCREEN_HEIGHT + 50, SCREEN_WIDTH, 100}};
        exitInfo = {{6, -40, -SCREEN_WIDTH, 40}, {8, 30, -30, SCREEN_HEIGHT}};

        squareRect1 = {0, 660, 300, SCREEN_HEIGHT - 660};

        //Vector holding rect for the next room
        roomRects = {rightWall, squareRect1, leftWallExit, roof};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, groundText, groundText, groundText};

        Rooms room8 = *new Rooms({200, 555, 0, 0}, roomRects, roomSurfs, exits, exitInfo, {}, {{}}, {}, {},loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{0, -150, SCREEN_WIDTH, 100}, {0, SCREEN_HEIGHT + 50, SCREEN_WIDTH, 100}};
        exitInfo = {{7, -30, 30, -SCREEN_HEIGHT}, {9, 30, -30, SCREEN_HEIGHT}};

        squareRect1 = {0, 0, 300, 100};

        //Vector holding rect for the next room
        roomRects = {leftWall, rightWall, squareRect1};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, groundText, groundText};

        Rooms room9 = *new Rooms({SCREEN_WIDTH / 2 - imRect.w / 2, 0, 0, 0}, roomRects, roomSurfs, exits, exitInfo, {}, {{}}, {}, {},loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{0, -150, SCREEN_WIDTH, 100}, {SCREEN_WIDTH + 50, 0, 100, SCREEN_HEIGHT}};
        exitInfo = {{8, -30, 30, -SCREEN_HEIGHT}, {10, 40, SCREEN_WIDTH, -40}};

        squareRect1 = {SCREEN_WIDTH - 100, 0, 100, 500};

        //Vector holding rect for the next room
        roomRects = {leftWall, floor, squareRect1};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, rockPlatform, groundText};

        //Creating enemies to be put in a vector of enemies in a room
        Enemies enemie5 = *new Enemies({200, 610, 100, 100}, {enemieWalk1L, enemieWalk2L}, {enemieWalk1R, enemieWalk2R}, enemieHurtL, enemieHurtR, deathAnimation, 10, gravity);
        Enemies enemie6 = *new Enemies({1200, 610, 100, 100}, {enemieWalk1L, enemieWalk2L}, {enemieWalk1R, enemieWalk2R}, enemieHurtL, enemieHurtR, deathAnimation, 10, gravity);

        Rooms room10 = *new Rooms({SCREEN_WIDTH / 2 - imRect.w / 2, 605, 0, 0}, roomRects, roomSurfs, exits, exitInfo, {}, {{}}, {}, {enemie5, enemie6},loadImages("images/bg.png"), chestRect);

        //Reassigns rect exits and exit info for new room exits for the next room
        exits = {{-150, 0, 100, SCREEN_HEIGHT}};
        exitInfo = {{9, -40, -SCREEN_WIDTH, 40}};

        squareRect1 = {950, 500, SCREEN_WIDTH - 950, SCREEN_HEIGHT - 500};
        squareRect2 = {0, 0, SCREEN_WIDTH, 200};

        //Vector holding rect for the next room
        roomRects = {leftWallExit, rightWall, floor, squareRect2, squareRect1};

        //Resigns surfaces to blit into the Rect objects of the room objects
        roomSurfs = {groundText, groundText, rockPlatform, groundText, groundText};

        Enemies boss = *new Enemies({500, 310, 400, 400}, {bossWalk1L, bossWalk2L}, {bossWalk1R, bossWalk2R}, bossHurtL, bossHurtR, deathAnimation, 25, gravity);

        //Assigns chest location
        chestRect = {{1000, 305, 300, 200}};

        Rooms room11 = *new Rooms({100, 605, 0, 0}, roomRects, roomSurfs, exits, exitInfo, {}, {{}}, {}, {boss},loadImages("images/bg.png"), chestRect);

        //Vector of all the rooms
        vector<Rooms> roomsArr = {room1, room2, room3, room4, room5, room6, room7, room8, room9, room10, room11};


        //--------------------------------Creation of Levels--------------------------------

        //Creates level object to make a level
        Levels level1 = *new Levels(roomsArr, paraBG);

        //Vector of all the levels
        vector<Levels> levels = {level1};


        // --------------------------------Main Game loop--------------------------------

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

                        if (!transition && !dead && !levelSelect) {
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
                        } else {
                            down = false;
                        }
                        if (keystates[SDL_SCANCODE_LSHIFT]){
                            sprint = true;
                        } else {
                            sprint = false;
                        }

                        //Function for player movement
                        playerMovement(up, left, right, down, yVel, xVel, imRect);
                    }

                    //Placeholder for the parallax bg rect
                    paraBGRect = {paraBGx % 1440, paraBGy % 810, 0, 0};

                    //Blits parallax background
                    SDL_BlitSurface(paraBG, nullptr, test, &paraBGRect);

                    //Blits no-parallax bg
                    SDL_BlitSurface(levSelBG, nullptr, test, &pHolder);


                    //Applying gravity to the charter
                    yVel -= gravity;

                    //Applies y-axis velocity to the character
                    imRect.y -= yVel;

                    //Applies x-axis velocity to the character
                    imRect.x -= xVel;

                    //Makes it appear that the camera is following the player by moving everything but the player
                    if (cameraOffset + imRect.x + imRect.w / 2 < SCREEN_WIDTH * 2 - SCREEN_WIDTH / 2 && (imRect.x + imRect.w / 2 > SCREEN_WIDTH / 2 || (cameraOffset > 0))){

                        imRect.x = SCREEN_WIDTH / 2 - imRect.w / 2;
                        //Adds the offset to the parallax bg at a different rate than the rest to add the parallax effect
                        paraBGx += xVel / 2;

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

                    //Adds the offset to the non-parallax bg
                    pHolder.x = -cameraOffset;

                    //Resets this
                    readyToEnter = false;

                    //Goes through all doors in levelDoors
                    for (int d = 0; d < levelDoors.size(); d ++){

                        //Blits doors with placeholder
                        SDL_Rect doorHolder = levelDoors[d];
                        SDL_BlitSurface(levelDoorSurfs[d][0], nullptr, test, &doorHolder);

                        //Tests is player enters a door
                        if (SDL_HasIntersection(&levelDoors[d], &imRect)){

                            readyToEnter = true;

                            if (up){
                                //So sound only plays once
                                if (!enteringLevel) {
                                    Mix_FadeOutMusic(1000);
                                    Mix_PlayChannel(-1, door, 0);
                                }
                                enteringLevel = true;

                                //So the player doesn't jump when entering a level or move
                                if (yVel > 0) {
                                    imRect.y += yVel * 2;
                                    yVel = 0;
                                }
                                xVel = 0;

                                //Does door entering animation and then lowers the curtain
                                deathCurrTime = static_cast<int>(SDL_GetTicks());
                                if (enterIndex < static_cast<int>(levelDoorSurfs[d].size()) - 1) {
                                    if (deathCurrTime > deathPrevTime + 1000 / 20) {
                                        deathPrevTime = deathCurrTime;
                                        enterIndex++;
                                    }
                                } else {
                                    //Placeholder for curtain and blits it with blit function
                                    SDL_Rect curtainRect = {0, curtainOffset, 0, 0};
                                    SDL_BlitSurface(curtain, nullptr, test, &curtainRect);

                                    if (curtainOffset < 0) {
                                        curtainOffset = static_cast<int>(curtainOffset / 1.1);

                                        //Check so the curtain doesn't lower more than the screen height
                                        if (curtainOffset > 0) {
                                            curtainOffset = 0;
                                        }
                                    } else {

                                        //Gets curtain ready to raise
                                        curtainOffset = 5;
                                        currLevel = d;
                                        levels[currLevel].resetLevel();
                                        levels[currLevel].getRoom().roomAndPlayerReset(imRect, playerHealth, yVel, xVel,
                                                                                       arrR);
                                        readyToEnter = false;
                                        Mix_PlayMusic(caveMusic, -1);
                                        raiseCurtain = true;
                                        dropCurtain = false;
                                        up = false;
                                        dead = false;
                                        levelSelect = false;
                                    }
                                }

                                //Blits animation over opening door
                                SDL_BlitSurface(levelDoorSurfs[d][enterIndex], nullptr, test, &doorHolder);
                            }
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
                        handleCollision(levelSelectRects[h], imRect, yVel, xVel, waitToSet);

                        //This is a placeholder so that the actual rect position does not get changed by the blit function
                        SDL_Rect holder = levelSelectRects[h];
                        SDL_Rect g = {0, 0, holder.w, holder.h};

                        SDL_BlitSurface(levelSelectSurfs[h], &g, test, &holder);

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
                            if (!enteringLevel) {
                                Mix_PlayMusic(titleMusic, -1);

                                //Sets player position to the door position after exiting the level
                                if (cameraOffset > 0 && (cameraOffset + imRect.x + imRect.w / 2 < SCREEN_WIDTH * 2 - SCREEN_WIDTH / 2)) {
                                    imRect.x = SCREEN_WIDTH / 2 - imRect.w / 2;
                                    imRect.y = levelDoors[currLevel].y + levelDoors[currLevel].h - imRect.h;
                                } else {
                                    imRect.x = levelDoors[currLevel].x + levelDoors[currLevel].w / 2 - imRect.w / 2;
                                    imRect.y = levelDoors[currLevel].y + levelDoors[currLevel].h - imRect.h;
                                }
                            }

                            //So you can't enter until curtain has been risen
                            up = false;

                            //Resets the level entering variables, so you can enter again.
                            enteringLevel = false;

                            //Raises curtain
                            curtainOffset = -static_cast<int>(fabs(curtainOffset * 1.39));

                            //Curtain placeholder and blit function
                            SDL_Rect curtainRect = {0, curtainOffset, 0, 0};
                            SDL_BlitSurface(curtain, nullptr, test, &curtainRect);

                        } else {

                            enterIndex = -1;

                            goToLevelSelScreen = false;
                            exiting = false;

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
                            } else {
                                down = false;
                            }
                            if ((keystates[SDL_SCANCODE_R])) {
                                goToLevelSelScreen = true;
                                dropCurtain = true;
                            }
                            if (keystates[SDL_SCANCODE_LSHIFT]){
                                sprint = true;
                            } else {
                                sprint = false;
                            }


                            //Function for player movement
                            playerMovement(up, left, right, down, yVel, xVel, imRect);

                            //Placeholder for the parallax bg rect
                            paraBGRect = {paraBGx % 1440, paraBGy % 810, 0, 0};

                            //Blits parallax background
                            SDL_BlitSurface(paraBG, nullptr, test, &paraBGRect);

                            //Blits no-parallax bg
                            SDL_BlitSurface(roomsArr[currRoom].getBG(), nullptr, test, &pHolder);

                            //So chest blits over grappling hook
                            if (!levels[currLevel].getRoom().getChestRect().empty()){
                                if (SDL_HasIntersection(&levels[currLevel].getRoom().getChestRect()[0], &arrR[0])){
                                    if (!exiting) {
                                        Mix_PlayChannel(-1, chest, 0);
                                        deathCurrTime = static_cast<int>(SDL_GetTicks());
                                        deathPrevTime = deathCurrTime;
                                    }

                                    exiting = true;
                                }
                                if (exiting){
                                    deathCurrTime = static_cast<int>(SDL_GetTicks());
                                    if (deathCurrTime > deathPrevTime + 3000){
                                        dropCurtain = true;
                                    }
                                    SDL_Rect exitHolder = levels[currLevel].getRoom().getChestRect()[0];
                                    SDL_BlitSurface(chestSurfs[1], nullptr, test, &exitHolder);
                                } else {
                                    SDL_Rect exitHolder = levels[currLevel].getRoom().getChestRect()[0];
                                    SDL_BlitSurface(chestSurfs[0], nullptr, test, &exitHolder);
                                }
                            }

                            //Test if grappling hook is shooting
                            if (shoot) {

                                //Gets rects that can be hit with the hook in the current level
                                roomRects = levels[currLevel].getRoom().getRects();

                                //Function for shooting the grappling hook
                                shooting(arrR, arr, imRect, track, test, roomRects, levels[currLevel].getRoom().getHittableRects(),
                                         levels[currLevel].getRoom().getHitTest(), sideOffsetY, sideOffsetX, yVel, xVel,
                                         hitEnemie);
                            }

                            //Test if grappling hook is retracting
                            if (retrac) {

                                //Function for retracting the grappling hook
                                retracting(arrR, arr, imRect, track, test, yVel, xVel, mouseUp,
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

                    levels[currLevel].levelUpdate(test, imRect, playerHealth, right, left, transition, yVel, xVel, textRect, dead, arrR, arr, track,
                                                  sideOffsetX, sideOffsetY, mouseUp, hitEnemie,
                                                  deathAnimation, deathAnimationIndex, dropCurtain, raiseCurtain,
                                                  curtainOffset, curtain, goToLevelSelScreen, levelSelect,
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

            // Update screen
            SDL_RenderPresent(renderer);
        }

        // Destroy renderer
        SDL_DestroyRenderer(renderer);

        //Frees surfaces
        SDL_FreeSurface(test);
        SDL_FreeSurface(im);

        //Frees sounds
        Mix_FreeMusic(titleMusic);
        Mix_FreeMusic(caveMusic);
        Mix_FreeChunk(hitSound);
        Mix_FreeChunk(enemiehit);
        Mix_FreeChunk(explosion);
        Mix_FreeChunk(chest);
        Mix_FreeChunk(woossh);
        Mix_FreeChunk(jumpSound);

        //Closes audio
        Mix_CloseAudio();

        // Destroy window
        SDL_DestroyWindow(window);
    }

    // Quit SDL
    SDL_Quit();

    return 0;
}