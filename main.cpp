#define SDL_MAIN_HANDLED true
#include <iostream>
#include <algorithm>

using namespace std;

#include <SDL.h>

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

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

            //Creates image rectangle
            SDL_Rect imRect = {0, 0, 100, 100};

            //Exit flag
            bool quit = false;

            //Movement flags
            bool left;
            bool right;
            bool up;
            bool down;

            //Set up for delta time FPS calculator
            Uint32 currTime;
            Uint32 prevTime = 0;

            //Creates surface that will mimic window size where the ms Surface will be scaled and blitted to
            SDL_Surface* test = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

            //Creates texture from test surface to be able to render with renderer
            SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, test);

            //Creates Rect for test surface so that its dimensions can mimic the window size
            SDL_Rect textRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            //Creates surface that will be soft scaled and blitted to test where test will be rendered to the screen
            SDL_Surface* ms = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

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

                    //Clears both surfaces that were blitted to
                    SDL_FillRect(ms, nullptr, 0x000000);
                    SDL_FillRect(test, nullptr, 0x000000);

                    //Gets events
                    SDL_Event e;

                    // Wait indefinitely for the next available event
                    //SDL_WaitEvent(&e);

                    //I used poll instead of wait because the key presses had a delay with wait.
                    SDL_PollEvent(&e);

                    // User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    //User resizes window
                    else if (e.type == SDL_WINDOWEVENT_RESIZED){
                        //Resizes test surface to window size so ms can be soft scaled to its size (Because you can only
                        // soft scale when blitting a surface to a surface, so I am soft scaling ms when it is blitting to
                        // test so test can then be rendered)
                        textRect.w = SDL_GetWindowSurface(window)->w;
                        textRect.h = SDL_GetWindowSurface(window)->h;
                        test = SDL_CreateRGBSurface(0, textRect.w, textRect.h - 5, 32, 0, 0, 0, 0);
                    }

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
                    if (up) {
                        imRect.y -= 5;
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

                    //Blit cat to ms
                    SDL_BlitSurface(im, nullptr, ms, &imRect);

                    //blit ms to test and soft scaled to test's size (Which is set to the window size)
                    SDL_BlitScaled(ms, nullptr, test, &textRect);

                    //Turns test surface into a texture, so it can be rendered
                    text = SDL_CreateTextureFromSurface(renderer, test);

                }
                // Initialize renderer color white for the background
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Clear screen
                SDL_RenderClear(renderer);

                // Set renderer color red to draw the square
               // SDL_SetRenderDrawColor(renderer, 0xFF, 0x0F, 0x00, 0xFF);

                // Draw filled square
                //SDL_RenderFillRect(renderer, &squareRect);

                //Prepares text to be rendered
                SDL_RenderCopy(renderer, text, &textRect, nullptr);

                // Update screen
                SDL_RenderPresent(renderer);
            }

            // Destroy renderer
            SDL_DestroyRenderer(renderer);
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