#include "main.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(color_t colorbase, color_t colorside, color_t colorpole, color_t colorsail, color_t colorcanon, color_t colorcover);
    glm::vec3 position;
    float rotation;
    float motor_speed;
    float yspeed;
    float windspeedx, windspeedz, wind;
    float reactionspeed;
    float booster;
    float crot;
    int points;
    int life;
    bool jump;
    void draw(glm::mat4 VP);
    void tick();
private:
    VAO *base;
    VAO *side;
    VAO *pole;
    VAO *sail;
    VAO *canon;
    VAO *cover;
};

#endif // BALL_H
