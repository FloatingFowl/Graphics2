#include "main.h"

#ifndef SPAMMON_H
#define SPAMMON_H

class Spammon {
public:
    Spammon(){};
    Spammon(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    bool dead;
    void draw(glm::mat4 VP);
private:
    VAO *rock;
};

#endif 
