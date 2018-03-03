#include "main.h"

#ifndef ROCK_H
#define ROCK_H


class Rock {
public:
    Rock() {}
    Rock(color_t colorrock);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
    void start(float x=0.0f, float y=0.0f, float rot=0.0f);
private:
    VAO *rock;
};

#endif 
