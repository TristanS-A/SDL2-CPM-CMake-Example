#define SDL_MAIN_HANDLED true
#include <iostream>
#include <algorithm>
#include <cmath>

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
    }
    else
    {
        // Create renderer
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer)
        {
            cout << "Renderer could not be created!" << endl
                 << "SDL_Error: " << SDL_GetError() << endl;
        }
        else
        {
            // Declare rect of square
            SDL_Rect squareRect;

            // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
            squareRect.w = min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
            squareRect.h = min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

            // Square position: In the middle of the screen
            squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
            squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;

            //Loads cat image
            SDL_Surface* im = SDL_LoadBMP( "cat.bmp" );
            if( im == nullptr )
            {
                printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
            }

            //Loads circle image
            SDL_Surface* circle = SDL_LoadBMP( "circle.bmp" );
            if( circle == nullptr )
            {
                printf( "Unable to load image %s! SDL Error: %s\n ", SDL_GetError() );
            }

            //Loads circle2 image
            SDL_Surface* circle2 = SDL_LoadBMP( "circle2.bmp" );
            if( circle2 == nullptr )
            {
                printf( "Unable to load image %s! SDL Error: %s\n ", SDL_GetError() );
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
            double angle = 0;

            //Distance from the character to the mouse position
            double dis = 0;

            //Mouse coordinates
            int mouseX;
            int mouseY;

            //Bool for if grappling hook is shooting out
            bool shoot;

            //Bool for if the grappling hook is retracting
            bool retrac;

            //Bool store what quadrant the mouse x position is in reletive to the character
            bool quadX;

            //Bool store what quadrant the mouse y position is in reletive to the character
            bool quadY;

            //Iteration variable to keep track of grappling hook vector indexes as it shoots and retracts
            int s;

            //Bool for if the grappling hook collides with a rect
            bool hit;

            //Vector holding grappling hook images
            vector<SDL_Surface*> arr;

            int u;

            for (u = 0; u < 15; u++){
                if (u == 0){
                    arr.push_back(circle2);
                } else {
                    arr.push_back(circle);
                }
            }

            //Vector holding grappling hook rects
            vector<SDL_Rect> arrR;

            SDL_Rect placeHolder = {0, 0, 50, 50};

            for (u = 0; u < 15; u++){
                arrR.push_back(placeHolder);
            }

            //Vector holding x and y velocity for each grappling hook piece
            vector<int> arrXY = {1, 1};
            vector<vector<int>> arrVel;

            for (u = 0; u < 15; u++){
                arrVel.push_back(arrXY);
            }

            //Set up for delta time FPS calculator
            Uint32 currTime;
            Uint32 prevTime = 0;

            //Creates surface that will be used for blitting
            SDL_Surface* test = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

            //Creates texture from test surface to be able to render test surface and the images blitted to it with renderer
            SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, test);

            //Creates Rect for test surface so that its dimensions can be rescaled and repositioned
            SDL_Rect textRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            //Sets up key presses
            const Uint8* keystates = SDL_GetKeyboardState(nullptr);

            // Event loop
            while(!quit)
            {
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
                    switch(e.type){
                        // User requests quit
                        case SDL_QUIT:
                            quit = true;
                            break;
                            //Presses mouse button
                        case SDL_MOUSEBUTTONDOWN:

                            //So you cant fire the grappling hook while already firring it
                            if (!shoot) {

                                //Gets mouse location
                                SDL_GetMouseState(&mouseX, &mouseY);

                                //Calculates the distance between the character and the mouse
                                dis = sqrt(pow(((imRect.x + 100 / 2.0) -
                                                static_cast<double>(mouseX) * (SCREEN_WIDTH) / textRect.w), 2) +
                                           pow(((imRect.y + 100 / 2.0) -
                                                static_cast<double>(mouseY) * (SCREEN_HEIGHT) / textRect.h), 2));

                                //TODO: Make it so grapple hook balls disappear after getting back to the player and
                                // also make diagonal shots reach as far as up or down shots.

                                //Calculates the angle between the character and the mouse.
                                angle = -(atan(((imRect.y + 100 / 2.0) -
                                                static_cast<double>(mouseY) * (SCREEN_HEIGHT) / textRect.h) /
                                               ((imRect.x + 100 / 2.0) -
                                                static_cast<double>(mouseX) * (SCREEN_WIDTH) / textRect.w)) *
                                          (180.0 / M_PI));
                                shoot = true;
                                s = 0;
                            }
                            break;
                    }

                    //Gets window hight and width
                    textRect.w = SDL_GetWindowSurface(window)->w;
                    textRect.h = SDL_GetWindowSurface(window)->h;

                    //My computer for some reason set the hight to 1009 whn full screening even though my screen is 1080
                    // so I changed it here because it annoyed me
                    if (textRect.h == 1009){
                        textRect.h = 1080;
                        SDL_GetWindowSurface(window)->h = 1080;
                    }

                    //Since I am keeping the dimensions at a 16:9 ratio, this checks weather the width or height is
                    // unable to expand more and then sets the other side to the 16:9 ration with respects to the
                    // restricted side
                    if (textRect.w / 16 > textRect.h / 9){
                        //Sets dimension of the Rect if the height cannot expand more at a 16:9 ratio
                        textRect.w = 16 * (textRect.h / 9);
                        textRect.h = textRect.h;
                    }
                    else if (textRect.w / 16 < textRect.h / 9){
                        //Sets dimension of the Rect if the width cannot expand more at a 16:9 ration
                        textRect.h = 9 * (textRect.w / 16);
                        textRect.w = textRect.w;
                    }

                    //Creates equal length margins on the side that has empty space
                    textRect.x = (SDL_GetWindowSurface(window)->w - textRect.w) / 2;
                    textRect.y = (SDL_GetWindowSurface(window)->h - textRect.h) / 2;

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
                    //Character control
                    if (up && jump) {
                        yVel += 30;
                        jump = false;
                        imRect.y -= yVel;
                    }
                    if (left) {
                        imRect.x -= 5;
                    }
                    if (right) {
                        imRect.x += 5;
                    }
                    if (down) {
                        imRect.y += 5;
                    }

                    //Test for seeing when two rects collide
                    if (SDL_HasIntersection(&imRect, &squareRect)){
                        cout << "touching!!!!\n";
                    }

                    if (shoot){
                        if (s < arr.size()){

                            //Resets grappling hook rects location
                            arrR[s].x = imRect.x + 100 / 2 - arrR[s].w / 2;
                            arrR[s].y = imRect.y + 100 / 2 - arrR[s].h / 2;

                            //Calculates which direction to apply x and y velocity according to the angle, because I
                            // calculated the angle so that it will be withing -90 to 90, so I have to flip the force if
                            // the mouse position is on the left side of the character
                            if ((mouseX * (SCREEN_WIDTH)/textRect.w) <= (imRect.x + 100 / 2)){

                                arrVel[s][1] = -static_cast<int>((fabs(90 - angle) - 90) * 0.6);

                                arrVel[s][0] = -static_cast<int>((fabs(-1 * (90 - fabs(angle)))) * 0.6);
                            }
                            else {

                                arrVel[s][1] = static_cast<int>((fabs(90 - angle) - 90) * 0.6);
                                arrVel[s][0] = static_cast<int>((fabs((90 - fabs(angle)))) * 0.6);
                            }
                            for (int b = 0; b < s; b++){

                                SDL_BlitSurface(arr[b], nullptr, test, &arrR[b]);
                                arrR[b].y += arrVel[s][1];
                                arrR[b].x += arrVel[s][0];
                            }
                        } else {
                            for (int k = 0; k < s; k++){
                                SDL_BlitSurface(arr[k], nullptr, test, &arrR[k]);
                            }
                        }

                        s++;

                        if (s == arr.size()) {
                            shoot = false;
                            retrac = true;
                            s--;
                        }
                    }

                    if (retrac){
                        if (s >= 0){
                            if (hit){

                            } else {

                                for (int b = 0; b <= s; b++){

                                    angle = -(atan(((imRect.y + 100 / 2.0) - static_cast<int>(arrR[b].y + arrR[b].h / 2)) / (((imRect.x + 100 / 2.0) -
                                                                                                                              static_cast<int>(arrR[b].x + arrR[b].w / 2)))) * (180.0 / M_PI));

                                    if (static_cast<int>(arrR[b].x + arrR[b].w / 2) <= (imRect.x + 100 / 2)){

                                        arrVel[b][1] = -static_cast<int>((fabs(90 - angle) - 90));

                                        arrVel[b][0] = -static_cast<int>((fabs(-1 * (90 - fabs(angle)))));
                                    } else {

                                        arrVel[b][1] = static_cast<int>((fabs(90 - angle) - 90));
                                        arrVel[b][0] = static_cast<int>((fabs((90 - fabs(angle)))));
                                    }

                                    SDL_BlitSurface(arr[b], nullptr, test, &arrR[b]);
                                    arrR[b].y -= arrVel[b][1];
                                    arrR[b].x -= arrVel[b][0];
                                }

                            }


                        } else {

                            for (int k = s; k >= s; k--){
                                SDL_BlitSurface(arr[k], nullptr, test, &arrR[k]);
                            }
                        }
                        s--;
                        if (s < 0){
                            retrac = false;
                        }
                    }

                    //Applying gravity to the charter
                    yVel -= gravity;

                    //Applies y-axis velocity to the character
                    imRect.y -= yVel;

                    //Applies x-axis velocity to the character
                    imRect.x -= xVel;

                    //Test for when the charter is grounded so the character stops moving by setting yVel to 0
                    if (imRect.y > 500){
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
                    SDL_UpdateTexture(text, nullptr,test->pixels,test->pitch);

                }
                // Initialize renderer color white for the background
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Clear screen
                SDL_RenderClear(renderer);

                //Prepares text to be rendered at the location of textRect and also resizes/soft scales it to the
                // dimensions of textRect
                SDL_RenderCopy(renderer, text, nullptr, &textRect);

                // Set renderer color red to draw the square
                //SDL_SetRenderDrawColor(renderer, 0xFF, 0x0F, 0x00, 0xFF);

                // Draw filled square
                //SDL_RenderFillRect(renderer, &squareRect);

                // Update screen
                SDL_RenderPresent(renderer);
            }

            // Destroy renderer
            SDL_DestroyRenderer(renderer);

            //Frees surfaces
            SDL_FreeSurface(test);
            SDL_FreeSurface(im);
        }

        // Destroy window
        SDL_DestroyWindow(window);
    }

    // Quit SDL
    SDL_Quit();

    return 0;
}
//#define SDL_MAIN_HANDLED true
//#include <iostream>
//#include <algorithm>
//
//#include <SDL.h>
//
//#define SCREEN_WIDTH    800
//#define SCREEN_HEIGHT   600
//
//int main(int argc, char* argv[])
//{
//    // Unused argc, argv
//    (void) argc;
//    (void) argv;
//
//    // Initialize SDL
//    if(SDL_Init(SDL_INIT_VIDEO) < 0)
//    {
//        std::cout << "SDL could not be initialized!" << std::endl
//                  << "SDL_Error: " << SDL_GetError() << std::endl;
//        return 0;
//    }
//
//#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
//    // Disable compositor bypass
//    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
//    {
//        std::cout << "SDL can not disable compositor bypass!" << std::endl;
//        return 0;
//    }
//#endif
//
//    // Create window
//    SDL_Window *window = SDL_CreateWindow("Basic C++ SDL project",
//                                          SDL_WINDOWPOS_UNDEFINED,
//                                          SDL_WINDOWPOS_UNDEFINED,
//                                          SCREEN_WIDTH, SCREEN_HEIGHT,
//                                          SDL_WINDOW_SHOWN);
//    if(!window)
//    {
//        std::cout << "Window could not be created!" << std::endl
//                  << "SDL_Error: " << SDL_GetError() << std::endl;
//    }
//    else
//    {
//        // Create renderer
//        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//        if(!renderer)
//        {
//            std::cout << "Renderer could not be created!" << std::endl
//                      << "SDL_Error: " << SDL_GetError() << std::endl;
//        }
//        else
//        {
//            // Declare rect of square
//            SDL_Rect squareRect;
//            const Uint8* keystates = SDL_GetKeyboardState(nullptr);
//
//            // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
//            squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
//            squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
//
//            // Square position: In the middle of the screen
//            squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
//            squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;
//
//            SDL_Surface* test = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
////
////            //Creates texture from test surface to be able to render with renderer
//            SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, test);
//
//            SDL_Rect textRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
////
////            //Creates surface that will be soft scaled and blitted to test where test will be rendered to the screen
//            SDL_Surface* ms = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
//
//
//            // Event loop exit flag
//            bool quit = false;
//            bool left;
//            bool right;
//            bool up;
//            bool down;
//            Uint32 currTime;
//            Uint32 prevTime = 0;
//
//            // Event loop
//            while(!quit)
//            {
//                currTime = SDL_GetTicks();
////
////                //Set to 60 FPS
//                if (currTime > prevTime + 1000 / 60) {
//                    prevTime = currTime;
//                    SDL_Event e;
//
//                    // Wait indefinitely for the next available event
//                    SDL_PollEvent(&e);
//
//                    // User requests quit
//                    if (e.type == SDL_QUIT) {
//                        quit = true;
//                    }
//                    else if (e.type == SDL_WINDOWEVENT_RESIZED){
////                        //Resizes test surface to window size so ms can be soft scaled to its size (Because you can only
////                        // soft scale when blitting a surface to a surface, so I am soft scaling ms when it is blitting to
////                        // test so test can then be rendered)
//                        textRect.w = SDL_GetWindowSurface(window)->w;
//                        textRect.h = SDL_GetWindowSurface(window)->h;
//                        test = SDL_CreateRGBSurface(0, textRect.w, textRect.h - 5, 32, 0, 0, 0, 0);
//                    }
//
//                    //Gets key inputs
//                    if ((keystates[SDL_SCANCODE_W])) {
//                        up = true;
//                    } else {
//                        up = false;
//                    }
//                    if (keystates[SDL_SCANCODE_A]) {
//                        left = true;
//                    } else {
//                        left = false;
//                    }
//                    if ((keystates[SDL_SCANCODE_D])) {
//                        right = true;
//                    } else {
//                        right = false;
//                    }
//                    if ((keystates[SDL_SCANCODE_S])) {
//                        down = true;
//                    } else {
//                        down = false;
//                    }
//
//                    //Character control
//                    if (up) {
//                        squareRect.y -= 5;
//                    }
//                    if (left) {
//                        squareRect.x -= 5;
//                    }
//                    if (right) {
//                        squareRect.x += 5;
//                    }
//                    if (down) {
//                        squareRect.y += 5;
//                    }
//                }
//
//                // Initialize renderer color white for the background
//                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//
//                // Clear screen
//                SDL_RenderClear(renderer);
//
//                //text = SDL_CreateTextureFromSurface(renderer, test);
//
//                SDL_RenderCopy(renderer, text, &textRect, nullptr);
//
//                // Set renderer color red to draw the square
//                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
//
//                // Draw filled square
//                SDL_RenderFillRect(renderer, &squareRect);
//
//                // Update screen
//                SDL_RenderPresent(renderer);
//            }
//
//            // Destroy renderer
//            SDL_DestroyRenderer(renderer);
//        }
//
//        // Destroy window
//        SDL_DestroyWindow(window);
//    }
//
//    // Quit SDL
//    SDL_Quit();
//
//    return 0;
//}
