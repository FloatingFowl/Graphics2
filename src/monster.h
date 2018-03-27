#include "main.h"

#ifndef MONSTER_H
#define MONSTER_H

class Monster {
public:
    Monster(){};
    Monster(float x, float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *rock, *block;
};

#endif 
