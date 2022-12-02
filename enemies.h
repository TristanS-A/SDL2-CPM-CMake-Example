//
// Created by tropi on 11/30/2022.
//

#ifndef MYPROJECT_ENEMIES_H
#define MYPROJECT_ENEMIES_H

#include "SDL.h"
#include <algorithm>

using namespace std;

class Enemies{

private:

    //Body and position of enemies
    SDL_Rect hitbox{};

    //To reset the position of enemies
    SDL_Rect resetHitbox;

    //Images for animation
    vector<SDL_Surface *> animationCycle;

    //Image for when enemies are hurt
    SDL_Surface *hitImage;

    //Indexes to cycle through images
    int maxIndex;
    int index;

    //To keep track of animation speed
    int currTime{};
    int prevTime;

    //To keep track of current image for the enemies
    SDL_Surface *currImage;

    //To move the enemies
    int yVel;
    int xVel;
    int gravity;

    //To let the enemies die
    bool dead;

    //Enemies health
    int enemieHealth;

    //To reset enemies health
    int resetHealth;

    //Buffer so you can't damage the enemies every frame
    int bufferCurrTime;
    int bufferPrevTime;

    //Time showing damageImage
    int damageTime;

public:
    Enemies(SDL_Rect enemieRec, vector<SDL_Surface *> enemieImages, SDL_Surface *hurtImage, int health, int grav);

    void update(SDL_Surface *test, SDL_Rect &imRect);

    SDL_Rect getHitBox();

    void setDead(bool die);

    void collisionTest(SDL_Rect testRect);

    void addForce(int xForce, int yForce);

    bool dealDamage(int damage);

    bool isDead();

    SDL_Surface* getCurrImage();

    SDL_Surface* getDefaultImage();

    void resetEnemie();

};

#endif //MYPROJECT_ENEMIES_H
