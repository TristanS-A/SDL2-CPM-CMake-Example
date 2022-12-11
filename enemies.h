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
    SDL_Rect resetHitbox{};

    //Images for left moving animation
    vector<SDL_Surface *> animationCycleL;

    //Image for when enemies are hurt from left side
    SDL_Surface *hitImageL;

    //Images for right moving animation
    vector<SDL_Surface *> animationCycleR;

    //Image for when enemies are hurt from right side
    SDL_Surface *hitImageR;

    //Indexes to cycle through images
    int maxIndex;
    int index;

    //To keep track of animation speed
    int currTime = 0;
    int prevTime;

    //To keep track of current image for the enemies
    SDL_Surface *currImage{};

    //To move the enemies
    int yVel;
    int xVel;
    int gravity;

    //To tell what direction the enemie is moving in
    bool dir = false;

    //To let the enemies die
    bool dead;

    //Enemies health
    int enemieHealth;

    //To reset enemies health
    int resetHealth;

    //Buffer so you can't damage the enemies every frame
    int bufferCurrTime = 0;
    int bufferPrevTime;

    //Time showing damageImage
    int damageTime;

    //Death animation
    vector<SDL_Surface *> deathAnimation;

    //Index for death animation
    int deathIndex;

public:
    Enemies(SDL_Rect enemieRec, vector<SDL_Surface *> enemieImagesLeft, vector<SDL_Surface *> enemieImagesRight, SDL_Surface *hurtImageLeft, SDL_Surface *hurtImageRight, vector<SDL_Surface *> deathAni, int health, int grav);

    void update(SDL_Surface *test, SDL_Rect &imRect);

    SDL_Rect getHitBox();

    void setDead(bool die);

    void collisionTest(SDL_Rect testRect);

    void addForce(int xForce);

    bool dealDamage(int damage);

    bool isDead();

    SDL_Surface* getCurrImage();

    SDL_Surface* getDefaultImage();

    void resetEnemie();

    void enemieDeath(SDL_Surface *test);

    bool deathAnimationDone();

};

#endif //MYPROJECT_ENEMIES_H
