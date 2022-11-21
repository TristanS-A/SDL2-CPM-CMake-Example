//
// Created by tropi on 11/18/2022.
//

#include <iostream>
#include <SDL.h>

using namespace std;

void screenSizeChange(SDL_Rect &textRect, SDL_Window *window){
    //Gets window height and width
    textRect.w = SDL_GetWindowSurface(window)->w;
    textRect.h = SDL_GetWindowSurface(window)->h;

    //My computer for some reason set the height to 1009 whn full screening even though my screen is 1080,
    // so I changed it here because it annoyed me
    if (textRect.h == 1009 || textRect.h == 1017){
        textRect.h = 1080;
        SDL_GetWindowSurface(window)->h = 1080;
    }

    //Since I am keeping the dimensions at a 16:9 ratio, this checks weather the width or height is
    // unable to expand more and then sets the other side to the 16:9 ration with respects to the
    // restricted side
    if (textRect.w / 16 > textRect.h / 9) {
        //Sets dimension of the Rect if the height cannot expand more at a 16:9 ratio
        textRect.w = 16 * (textRect.h / 9);
        textRect.h = textRect.h;
    } else if (textRect.w / 16 < textRect.h / 9) {
        //Sets dimension of the Rect if the width cannot expand more at a 16:9 ration
        textRect.h = 9 * (textRect.w / 16);
        textRect.w = textRect.w;
    }

    //Creates equal length margins on the side that has empty space
    textRect.x = (SDL_GetWindowSurface(window)->w - textRect.w) / 2;
    textRect.y = (SDL_GetWindowSurface(window)->h - textRect.h) / 2;
}