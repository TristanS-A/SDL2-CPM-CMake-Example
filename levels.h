//
// Created by tropi on 12/3/2022.
//

#ifndef MYPROJECT_LEVELS_H
#define MYPROJECT_LEVELS_H

#include "rooms.h"

class Levels{
private:
    vector<Rooms> levelRooms;
    SDL_Rect paraBGRect;
    SDL_Surface *paraBG;
    int currRoom;
    int nextRoom;
    vector<SDL_Rect> roomRects;
    vector<int> exitInfo;
    bool noSwitch;
    int transitionSpeed;
    int offsetX;
    int offseetY;


public:
    Levels(vector<Rooms> levRooms, SDL_Surface *parallaxBG);

    void levelUpdate(SDL_Surface *test, SDL_Rect &imRect, int &playerHealth, bool &right, bool &left, bool &transition, int &yVel, int &xVel, SDL_Rect &textRect, bool &jump, bool &dead, vector<SDL_Rect> arrR, vector<SDL_Surface *>arr, int &track, int &sideOffsetX, int &sideOffsetY, bool &mouseUp, bool &hitEnemie, vector<SDL_Surface *> deathAnimation, int &deathAnimationIndex, bool &dropCurtain, bool &raiseCurtain, int &curtainOffset, SDL_Surface *curtain,  bool &goToLevSelScreen, bool &levelSelect, SDL_Rect &paraBGRect, int &paraBGx, int &paraBGy);

    Rooms getRoom();

    void resetLevel();
};


#endif //MYPROJECT_LEVELS_H
