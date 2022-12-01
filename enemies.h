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
    SDL_Rect hitbox{};
    vector<SDL_Surface *> animationCycle;
    SDL_Surface *hitImage;
    int maxIndex;
    int index;

    //To keep track of animation speed
    int currTime{};
    int prevTime;

    //To move the enemies
    int yVel;
    int xVel;
    int gravity;

    //To let the enemies die
    bool dead;

    //Enemie's health
    int enemieHealth;

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

};

#endif //MYPROJECT_ENEMIES_H
