//
// Created by tropi on 11/25/2022.
//

#include "SDL.h"
#include <SDL_image.h>
#include <string>
#include <iostream>

using namespace std;

SDL_Surface * loadImages(const char *name){

    //Uses SDL_image to load a image file
    SDL_Surface* image = IMG_Load(name);

    if (image == nullptr) {
        cout << "Error loading image: " << IMG_GetError() << "\n";
        return image;
    }

    return image;
}
