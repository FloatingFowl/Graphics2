#include "bomb.h"
#include "main.h"

void Bomb::draw(glm::mat4 VP) {
    if (!shot) return;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->cbd1);
    draw3DObject(this->cbd2);
    draw3DObject(this->cbd3);
}

void Bomb::tick() {
    if (!shot) return;
    //yspeed
    float friction = 10.0f;
    float nspeedy = yspeed - friction * (1/60.0);
    float dist = yspeed * (1/60.0) - friction * (1/7200.0);
    position.y += dist;
    yspeed = nspeedy;
    //xspeed
    dist = xspeed / (60.0f);
    position.z += dist * cos(rotation * M_PI / 180.0f);
    position.x += dist * sin(rotation * M_PI / 180.0f);
    if(position.y < -0.25)
        shot = false;
}

void Bomb::start(float x, float y, float z, float rot, bool booster){
    shot = true;
    yspeed = 16.0f;
    if(booster)
        xspeed *= 4;
    xspeed = 16.0f;
    rotation = rot;
    position.x = x + 4.5 * sin(rotation * M_PI / 180.0), position.z = z + 4.5 * cos(rotation * M_PI/180.0), position.y = y + 1.5f;
}

Bomb::Bomb(color_t color1, color_t color2, color_t color3) {
    shot = false;
    position = glm::vec3(0, 1.5f, 4.5f);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    static const GLfloat data_1[] = {
        -0.5f/2,-0.5f/2,-0.5f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,-0.5f/2,-0.5f/2,
        0.5f/2,-0.5f/2,-0.5f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        -0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,-0.5f/2,
        -0.5f/2,0.5f/2,-0.5f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,-0.5f/2,
        -0.5f/2,0.5f/2,-0.5f/2,
        0.5f/2,-0.5f/2,-0.5f/2,
        -0.5f/2,-0.5f/2,-0.5f/2,
        -0.5f/2,0.5f/2,-0.5f/2,
        0.5f/2,0.5f/2,-0.5f/2,
        0.5f/2,-0.5f/2,-0.5f/2,
        -0.5f/2,-0.5f/2,-0.5f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        -0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,-0.5f/2,-0.5f/2,
        -0.5f/2,0.5f/2,-0.5f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,-0.5f/2,
        0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,-0.5f/2,
        0.5f/2,0.5f/2,-0.5f/2,
        0.5f/2,0.5f/2,-0.17f/2,
    };


    static const GLfloat data_2[] = {
        -0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        -0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        -0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,-0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,-0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,-0.17f/2,
        0.5f/2,0.5f/2,0.17f/2,    
    };


    static const GLfloat data_3[] = {
        -0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,0.5f/2,
        -0.5f/2,-0.5f/2,0.5f/2,
        -0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,0.5f/2,
        -0.5f/2,-0.5f/2,0.5f/2,
        0.5f/2,0.5f/2,0.5f/2,
        -0.5f/2,0.5f/2,0.5f/2,
        -0.5f/2,-0.5f/2,0.5f/2,
        0.5f/2,-0.5f/2,0.5f/2,
        0.5f/2,0.5f/2,0.5f/2,
        -0.5f/2,0.5f/2,0.5f/2,
        0.5f/2,0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.5f/2,
        0.5f/2,0.5f/2,0.5f/2,
        0.5f/2,0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.5f/2,
        -0.5f/2,-0.5f/2,0.5f/2,
        -0.5f/2,-0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.17f/2,
        -0.5f/2,0.5f/2,0.5f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,0.5f/2,
        0.5f/2,-0.5f/2,0.5f/2,
        0.5f/2,-0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,0.17f/2,
        0.5f/2,0.5f/2,0.5f/2, 
    };

    this->cbd1 = create3DObject(GL_TRIANGLES, (12)*3, data_1, color1, GL_FILL);
    this->cbd2 = create3DObject(GL_TRIANGLES, (12)*3, data_2, color2, GL_FILL);
    this->cbd3 = create3DObject(GL_TRIANGLES, (12)*3, data_3, color3, GL_FILL);
}


