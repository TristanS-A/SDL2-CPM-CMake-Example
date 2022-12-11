//
// Created by tropi on 11/30/2022.
//
#include "enemies.h"
#include "globalVariables.h"

#include <SDL_mixer.h>
#include <utility>
using namespace std;

Enemies::Enemies(SDL_Rect enemieRec, vector<SDL_Surface *> enemieImagesLeft, vector<SDL_Surface *> enemieImagesRight, SDL_Surface *hurtImageLeft, SDL_Surface *hurtImageRight, vector<SDL_Surface *> deathAni, int health, int grav) {

    hitbox = enemieRec;
    resetHitbox = enemieRec;
    animationCycleL = std::move(enemieImagesLeft);
    hitImageL = hurtImageLeft;
    animationCycleR = std::move(enemieImagesRight);
    hitImageR = hurtImageRight;
    maxIndex = static_cast<int>(animationCycleL.size());
    index = 0;
    prevTime = 0;
    yVel = 0;
    xVel = 0;
    gravity = grav;
    dead = false;
    enemieHealth = health;
    resetHealth = health;
    bufferPrevTime = 0;
    damageTime = 0;
    deathAnimation = std::move(deathAni);
    deathIndex = 0;
}

void Enemies::update(SDL_Surface *test, SDL_Rect &imRect) {

    //Applying gravity to the enemies
    yVel -= gravity;

    //To make the enemie follow the player
    if (imRect.x + imRect.w / 2 > hitbox.x + hitbox.w / 2){
        if (xVel > -5) {
            xVel -= 1;
            dir = false;
        }
    } else {
        if (xVel < 5) {
            xVel += 1;
            dir = true;
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

    //In case enemie falls out of stage
    if (hitbox.y > 810){
        dead = true;
    }

    if (damageTime == 0) {
        if (dir) {
            currImage = animationCycleL[index];
        } else {
            currImage = animationCycleR[index];
        }
    } else if (damageTime > 0){
        if (dir) {
            currImage = hitImageL;
        } else {
            currImage = hitImageR;
        }
        damageTime -= 1;
    }

    SDL_BlitSurface(currImage, nullptr, test, &enemieTemp);

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

void Enemies::addForce(int xForce) {
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
        Mix_PlayChannel(-1, hitSound, 0);
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
    if (dir) {
        return animationCycleL[0];
    } else {
        return animationCycleR[0];
    }
}

void Enemies::resetEnemie() {
    enemieHealth = resetHealth;
    hitbox.x = resetHitbox.x;
    hitbox.y = resetHitbox.y;
    xVel = 0;
    yVel = 0;
    damageTime = 0;
    dead = false;
    deathIndex = 0;

}

void Enemies::enemieDeath(SDL_Surface *test) {
    SDL_Rect deathHolder = {hitbox.x + hitbox.w / 2 - 100, hitbox.y + hitbox.h / 2 - 100, 0, 0};

    currTime = static_cast<int>(SDL_GetTicks());

    if (currTime > prevTime + 1000 / 10) {
        if (deathIndex == 0){
            Mix_PlayChannel(-1, explosion, 0);
        }
        prevTime = currTime;
        deathIndex++;
    }
    SDL_BlitSurface(deathAnimation[deathIndex], nullptr, test, &deathHolder);
}

bool Enemies::deathAnimationDone() {
    return deathIndex == deathAnimation.size() - 1;
}

