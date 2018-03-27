#include "main.h"

#ifndef BARREL_H
#define BARREL_H


class Barrel {
public:
    Barrel(){}
    Barrel(float x, float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    int type;
private:
    VAO *barrel;
    VAO *extra;
};

#endif 
