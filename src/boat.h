#include "main.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(color_t colorbase, color_t colorside);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
    double speed;
private:
    VAO *base;
    VAO *side;
};

#endif // BALL_H
