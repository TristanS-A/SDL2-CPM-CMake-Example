//
// Created by tropi on 11/30/2022.
//
#include "enemies.h"

#include <utility>
#include <iostream>
using namespace std;

Enemies::Enemies(SDL_Rect enemieRec, vector<SDL_Surface *> enemieImages, SDL_Surface *hurtImage, int health, int grav) {

    hitbox = enemieRec;
    animationCycle = std::move(enemieImages);
    hitImage = hurtImage;
    maxIndex = static_cast<int>(animationCycle.size());
    index = 0;
    prevTime = 0;
    yVel = 0;
    xVel = 0;
    gravity = grav;
    dead = false;
    enemieHealth = health;
    bufferCurrTime = static_cast<int>(SDL_GetTicks());
    bufferPrevTime = 0;
    damageTime = 0;
}

void Enemies::update(SDL_Surface *test, SDL_Rect &imRect) {

    //Applying gravity to the enemies
    yVel -= gravity;

    if (imRect.x + imRect.w / 2 > hitbox.x + hitbox.w / 2){
        if (xVel > -5) {
            xVel -= 1;
        }
    } else {
        if (xVel < 5) {
            xVel += 1;
        }
    }

    //Applies y-axis velocity to the enemies
    hitbox.y -= yVel;

    //Applies x-axis velocity to the enemies
    hitbox.x -= xVel;

    //Placeholder so the blit function doesn't alter the enemieRect
    SDL_Rect enemieTemp = hitbox;

    currTime = static_cast<int>(SDL_GetTicks());

    if (currTime > prevTime + 1000 / 5) {
        prevTime = currTime;
        if (index < maxIndex - 1) {
            index++;
        } else {
            index = 0;
        }
    }

    //To keep the enemies onscreen
    if (hitbox.x < 0){
        hitbox.x = 0;
    }
    if (hitbox.x + hitbox.w > 1440){
        hitbox.x = 1440 - hitbox.w;
    }

    if (damageTime == 0) {
        currImage = animationCycle[index];
    } else if (damageTime > 0){
        currImage = hitImage;
        damageTime -= 1;
    }

    SDL_BlitSurface(currImage, &enemieTemp, test, &enemieTemp);

}

SDL_Rect Enemies::getHitBox() {
    return hitbox;
}

void Enemies::setDead(bool die) {
    dead = die;
}

void Enemies::collisionTest(SDL_Rect testRect) {
    if (SDL_HasIntersection(&testRect, &hitbox)){

        //Top side
        if (hitbox.y + hitbox.h - testRect.y <= fabs(yVel) && yVel <= 0){
            hitbox.y = testRect.y - hitbox.h;
            yVel = 0;
        }

            //Bottom side
        else if (testRect.y + testRect.h - hitbox.y <= fabs(yVel) && yVel >= 0){
            hitbox.y = testRect.y + testRect.h;
            yVel = 0;
        }

        //Left and Right side
        if ((hitbox.x + hitbox.w - testRect.x <= fabs(xVel) && xVel <= 0) || (testRect.x + testRect.w - hitbox.x <= fabs(xVel) && xVel >= 0)) {
            //Left side
            if (xVel < 0) {
                hitbox.x = testRect.x - hitbox.w;
                xVel = 0;
            }

                //Right side
            else {
                hitbox.x = testRect.x + testRect.w;
                xVel = 0;
            }
        }
    }
}

void Enemies::addForce(int xForce, int yForce) {
    if (xForce > 0){
        xVel = -30;
    } else {
        xVel = +30;
    }
    yVel = 15;
}

bool Enemies::dealDamage(int damage) {

    bufferCurrTime = static_cast<int>(SDL_GetTicks());

    if (bufferCurrTime > bufferPrevTime + 1000 / 5) {
        bufferPrevTime = bufferCurrTime;
        enemieHealth -= damage;
        damageTime = 10;
    }

    if (enemieHealth < 0){
        return true;
    } else {
        return false;
    }

}

bool Enemies::isDead() {
    if (dead){
        return true;
    } else {
        return false;
    }
}

SDL_Surface *Enemies::getCurrImage() {
    return currImage;
}

SDL_Surface *Enemies::getDefaultImage() {
    damageTime = 0;
    return animationCycle[0];
}

