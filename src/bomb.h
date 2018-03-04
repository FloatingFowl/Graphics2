#include "main.h"

#ifndef BOMB_H
#define BOMB_H


class Bomb {
public:
    Bomb() {}
    Bomb(color_t color1, color_t color2, color_t color3);
    glm::vec3 position;
    float rotation;
    float yspeed;
    float xspeed;
    bool shot;
    void draw(glm::mat4 VP);
    void tick();
    void start(float x=0.0f, float y=0.0f, float z=0.0f, float rot=0.0f);
private:
    VAO *cbd1;
    VAO *cbd2;
    VAO *cbd3;
};

#endif 
