#include "boat.h"
#include "main.h"

void Boat::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->base);
    draw3DObject(this->side);
    draw3DObject(this->pole);
    draw3DObject(this->cover);
    //
    {
        Matrices.model = glm::mat4(1.0f);
        translate = glm::translate (this->position);    // glTranslatef
        if(windspeedz > 0.25)
            rotate = glm::rotate((float) (180 * M_PI / 180.0f), glm::vec3(0, 1, 0));
        else if(windspeedz < -0.25)
            rotate = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 1, 0));
        else if (windspeedx > 0.25)
            rotate = glm::rotate((float) ((-90) * M_PI / 180.0f), glm::vec3(0, 1, 0));
        else if (windspeedx < -0.25)
            rotate = glm::rotate((float) ((90) * M_PI / 180.0f), glm::vec3(0, 1, 0));
        else
            rotate = glm::rotate((float) ((this->rotation) * M_PI / 180.0f), glm::vec3(0, 1, 0));
        Matrices.model *= (translate * rotate);
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(this->sail);
    }
    {
        Matrices.model = glm::mat4(1.0f);
        translate = glm::translate (this->position);    // glTranslatef
        glm::mat4 translate2 = glm::translate (glm::vec3(0,0,-4.0f));    // glTranslatef
        glm::mat4 opptranslate2 = glm::translate (glm::vec3(0,0,4.0f));
        glm::mat4 rotate2 = glm::rotate((float) ((crot) * M_PI / 180.0f), glm::vec3(0, 1, 0));
        rotate = glm::rotate((float) ((this->rotation) * M_PI / 180.0f), glm::vec3(0, 1, 0));
        Matrices.model *= (translate * rotate * opptranslate2 * rotate2 * translate2);
        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        draw3DObject(this->canon);
    }
}

void Boat::tick() {
    //wind
    float dist_covered = windspeedx * (1/60.0);
    //reaction
    float friction = 1.0f;
    reactionspeed = reactionspeed - (friction) * (1/60.0);
    dist_covered = (reactionspeed) * (1/60.0) - (1/7200.0) * friction;
    if(dist_covered > 0){
        position.z -= cos(this->rotation * M_PI / 180.0f) * dist_covered;
        position.x -= sin(this->rotation * M_PI / 180.0f) * dist_covered;
    }
    else reactionspeed = 0;
    //wind
    if(std::abs(windspeedz) >= 0.25)
        position.z += windspeedz/4;
    else if(std::abs(windspeedx) <= 0.25)
        position.x += windspeedx/4;
    //horizontal
    friction = 1.0f;
    float new_speed = motor_speed - friction * (1/60.0);
    dist_covered = (motor_speed) * (1/60.0) - (1/7200.0) * friction;
    dist_covered *= booster;
    if (dist_covered < 0){
        motor_speed = 0;
    }
    else {
        position.z += cos(this->rotation * M_PI / 180.0f) * dist_covered;
        position.x += sin(this->rotation * M_PI / 180.0f) * dist_covered;
    }
    motor_speed = fmax(new_speed, 0.0f);
    //vertical
    if(!jump) return;
    friction = 4.0f;
    new_speed = yspeed - friction * (1/60.0);
    dist_covered = yspeed * (1/60.0) - (1/7200.0) * friction;
    position.y += dist_covered;
    yspeed = new_speed;
    if (position.y < 0) {
        position.y = 0;
        jump = false;
    }
}

Boat::Boat(color_t colorbase, color_t colorside, color_t colorpole, color_t colorsail, color_t colorcover, color_t colorcanon) {
    crot = 0;
    life = 3;
    points = 0;
    booster = 1.0f;
    windspeedx = windspeedz = 0;
    reactionspeed = 0;
    position = glm::vec3(0, 0, 0);
    rotation = 0;
    motor_speed = 0;
    jump = false;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    static const GLfloat cover_data[] = {
        //cover
        0.0f,2.0f,6.0f,
        -2.2f,2.0f,4.0f,
        2.2f,2.0f,4.0f,
        -2.2f,2.0f,4.0f,
        2.2f,2.0f,4.0f,
        2.0f,0.5f,4.0f,
        -2.0f,0.5f,4.0f,
        2.0f,0.5f,4.0f,
        -2.2f,2.0f,4.0f,
    };

    static const GLfloat canon_data[] = {
        //shooter
        -0.5f,2.0f,4.0f,
        0.5f,2.0f,5.0f,
        -0.5f,2.0f,5.0f,
        -0.5f,2.0f,4.0f,
        0.5f,2.0f,4.0f,
        0.5f,2.0f,5.0f,
        -0.5f,2.0f,5.0f,
        0.5f,3.0f,5.0f,
        -0.5f,3.0f,5.0f,
        -0.5f,2.0f,5.0f,
        0.5f,2.0f,5.0f,
        0.5f,3.0f,5.0f,
        -0.5f,3.0f,5.0f,
        0.5f,2.0f,4.0f,
        -0.5f,2.0f,4.0f,
        -0.5f,3.0f,5.0f,
        0.5f,3.0f,5.0f,
        0.5f,2.0f,4.0f,
        -0.5f,2.0f,4.0f,
        -0.5f,2.0f,5.0f,
        -0.5f,3.0f,5.0f,
        0.5f,2.0f,4.0f,
        0.5f,2.0f,5.0f,
        0.5f,3.0f,5.0f,
    };

    static const GLfloat sail_data[] = {
        -0.1f,7.44f,-0.4f,
        0.1f,2.44f,-0.4f,
        -0.1f,2.44f,-0.4f,
        -0.1f,7.44f,-0.4f,
        0.1f,7.44f,-0.4f,
        0.1f,2.44f,-0.4f,
        -0.1f,2.44f,-0.4f,
        0.1f,2.44f,-4.0f,
        -0.1f,2.44f,-4.0f,
        -0.1f,2.44f,-0.4f,
        0.1f,2.44f,-0.4f,
        0.1f,2.44f,-4.0f,
        -0.1f,2.44f,-4.0f,
        0.1f,7.44f,-0.4f,
        -0.1f,7.44f,-0.4f,
        -0.1f,2.44f,-4.0f,
        0.1f,2.44f,-4.0f,
        0.1f,7.44f,-0.4f,
        -0.1f,7.44f,-0.4f,
        -0.1f,2.44f,-0.4f,
        -0.1f,2.44f,-4.0f,
        0.1f,7.44f,-0.4f,
        0.1f,2.44f,-0.4f,
        0.1f,2.44f,-4.0f,
    };

    static const GLfloat pole_data[] = {
        0.0f,0.45f,0.0f,
        0.25f,7.45f,0.433f,
        0.25f,0.45f,0.433f,
        0.0f,0.45f,0.0f,
        0.0f,7.45f,0.0f,
        0.25f,7.45f,0.433f,
        0.25f,0.45f,0.433f,
        -0.25f,7.45f,0.433f,
        -0.25f,0.45f,0.433f,
        0.25f,0.45f,0.433f,
        0.25f,7.45f,0.433f,
        -0.25f,7.45f,0.433f,
        -0.25f,0.45f,0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        -0.25f,0.45f,0.433f,
        -0.25f,7.45f,0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.25f,0.45f,0.433f,
        -0.25f,0.45f,0.433f,
        0.0f,7.45f,0.0f,
        0.25f,7.45f,0.433f,
        -0.25f,7.45f,0.433f,
        //pole
        0.0f,0.45f,0.0f,
        -0.5f,7.45f,0.0f,
        -0.5f,0.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.0f,7.45f,0.0f,
        -0.5f,7.45f,0.0f,
        -0.5f,0.45f,0.0f,
        -0.25f,7.45f,0.433f,
        -0.25f,0.45f,0.433f,
        -0.5f,0.45f,0.0f,
        -0.5f,7.45f,0.0f,
        -0.25f,7.45f,0.433f,
        -0.25f,0.45f,0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        -0.25f,0.45f,0.433f,
        -0.25f,7.45f,0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        -0.5f,0.45f,0.0f,
        -0.25f,0.45f,0.433f,
        0.0f,7.45f,0.0f,
        -0.5f,7.45f,0.0f,
        -0.25f,7.45f,0.433f,
        //pole
        0.0f,0.45f,0.0f,
        -0.5f,7.45f,0.0f,
        -0.5f,0.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.0f,7.45f,0.0f,
        -0.5f,7.45f,0.0f,
        -0.5f,0.45f,0.0f,
        -0.25f,7.45f,-0.433f,
        -0.25f,0.45f,-0.433f,
        -0.5f,0.45f,0.0f,
        -0.5f,7.45f,0.0f,
        -0.25f,7.45f,-0.433f,
        -0.25f,0.45f,-0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        -0.25f,0.45f,-0.433f,
        -0.25f,7.45f,-0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        -0.5f,0.45f,0.0f,
        -0.25f,0.45f,-0.433f,
        0.0f,7.45f,0.0f,
        -0.5f,7.45f,0.0f,
        -0.25f,7.45f,-0.433f,
        //oppside
        0.0f,0.45f,0.0f,
        -0.25f,7.45f,-0.433f,
        -0.25f,0.45f,-0.433f,
        0.0f,0.45f,0.0f,
        0.0f,7.45f,0.0f,
        -0.25f,7.45f,-0.433f,
        -0.25f,0.45f,-0.433f,
        -0.25f,7.45f,-0.433f,
        0.25f,0.45f,-0.433f,
        -0.25f,0.45f,-0.433f,
        -0.25f,7.45f,-0.433f,
        0.25f,7.45f,-0.433f,
        0.25f,0.45f,-0.433f,
        -0.0f,7.45f,0.0f,
        -0.0f,0.45f,0.0f,
        0.25f,0.45f,-0.433f,
        0.25f,7.45f,-0.433f,
        -0.0f,7.45f,0.0f,
        -0.0f,0.45f,0.0f,
        -0.25f,0.45f,-0.433f,
        0.25f,0.45f,-0.433f,
        -0.0f,7.45f,0.0f,
        -0.25f,7.45f,-0.433f,
        0.25f,7.45f,-0.433f,
        //pole
        0.0f,0.45f,0.0f,
        0.5f,7.45f,0.0f,
        0.5f,0.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.0f,7.45f,0.0f,
        0.5f,7.45f,0.0f,
        0.5f,0.45f,0.0f,
        0.25f,7.45f,-0.433f,
        0.25f,0.45f,-0.433f,
        0.5f,0.45f,0.0f,
        0.5f,7.45f,0.0f,
        0.25f,7.45f,-0.433f,
        0.25f,0.45f,-0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.25f,0.45f,-0.433f,
        0.25f,7.45f,-0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.5f,0.45f,0.0f,
        0.25f,0.45f,-0.433f,
        0.0f,7.45f,0.0f,
        0.5f,7.45f,0.0f,
        0.25f,7.45f,-0.433f,
        //pole
        0.0f,0.45f,0.0f,
        0.5f,7.45f,0.0f,
        0.5f,0.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.0f,7.45f,0.0f,
        0.5f,7.45f,0.0f,
        0.5f,0.45f,0.0f,
        0.25f,7.45f,0.433f,
        0.25f,0.45f,0.433f,
        0.5f,0.45f,0.0f,
        0.5f,7.45f,0.0f,
        0.25f,7.45f,0.433f,
        0.25f,0.45f,0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.25f,0.45f,0.433f,
        0.25f,7.45f,0.433f,
        0.0f,7.45f,0.0f,
        0.0f,0.45f,0.0f,
        0.5f,0.45f,0.0f,
        0.25f,0.45f,0.433f,
        0.0f,7.45f,0.0f,
        0.5f,7.45f,0.0f,
        0.25f,7.45f,0.433f,

    };

    static const GLfloat side_data[] = {
        -2.4f,2.0f,-4.0f,
        -2.2f,2.0f,4.0f,
        -2.4f,2.0f,4.0f,
        -2.4f,2.0f,-4.0f,
        -2.2f,2.0f,-4.0f,
        -2.2f,2.0f,4.0f,
        -2.4f,2.0f,4.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.0f,4.0f,
        -2.4f,2.0f,4.0f,
        -2.2f,2.0f,4.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.0f,4.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.0f,-4.0f,
        -2.0f,0.0f,4.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.0f,-4.0f,
        -2.2f,2.0f,-4.0f,
        -2.4f,2.0f,-4.0f,
        -2.0f,0.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        -2.2f,2.0f,-4.0f,
        -2.4f,2.0f,-4.0f,
        -2.0f,0.0f,4.0f,
        -2.4f,2.0f,4.0f,
        -2.4f,2.0f,-4.0f,
        -2.0f,0.0f,-4.0f,
        -2.0f,0.0f,4.0f,
        -2.2f,2.0f,-4.0f,
        -2.0f,0.5f,4.0f,
        -2.2f,2.0f,4.0f,
        -2.2f,2.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.5f,4.0f,
        //next side
        -2.4f,2.0f,4.0f,
        0.0f,2.0f,6.0f,
        0.0f,2.0f,6.2f,
        -2.4f,2.0f,4.0f,
        -2.2f,2.0f,4.0f,
        0.0f,2.0f,6.0f,
        0.0f,2.0f,6.2f,
        0.0f,0.5f,6.0f,
        0.0f,0.0f,6.0f,
        0.0f,2.0f,6.2f,
        0.0f,2.0f,6.0f,
        0.0f,0.5f,6.0f,
        0.0f,0.0f,6.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.0f,4.0f,
        0.0f,0.0f,6.0f,
        0.0f,0.5f,6.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.0f,4.0f,
        -2.2f,2.0f,4.0f,
        -2.4f,2.0f,4.0f,
        -2.0f,0.0f,4.0f,
        -2.0f,0.5f,4.0f,
        -2.2f,2.0f,4.0f,
        -2.4f,2.0f,4.0f,
        0.0f,0.0f,6.0f,
        0.0f,2.0f,6.2f,
        -2.4f,2.0f,4.0f,
        -2.0f,0.0f,4.0f,
        0.0f,0.0f,6.0f,
        -2.2f,2.0f,4.0f,
        0.0f,0.5f,6.0f,
        0.0f,2.0f,6.0f,
        -2.2f,2.0f,4.0f,
        -2.0f,0.5f,4.0f,
        0.0f,0.5f,6.0f, 
        //side
        -2.4f,2.0f,-4.0f,
        0.0f,2.0f,-6.0f,
        0.0f,2.0f,-6.2f,
        -2.4f,2.0f,-4.0f,
        -2.2f,2.0f,-4.0f,
        0.0f,2.0f,-6.0f,
        0.0f,2.0f,-6.2f,
        0.0f,0.5f,-6.0f,
        0.0f,0.0f,-6.0f,
        0.0f,2.0f,-6.2f,
        0.0f,2.0f,-6.0f,
        0.0f,0.5f,-6.0f,
        0.0f,0.0f,-6.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.0f,-4.0f,
        0.0f,0.0f,-6.0f,
        0.0f,0.5f,-6.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.0f,-4.0f,
        -2.2f,2.0f,-4.0f,
        -2.4f,2.0f,-4.0f,
        -2.0f,0.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        -2.2f,2.0f,-4.0f,
        -2.4f,2.0f,-4.0f,
        0.0f,0.0f,-6.0f,
        0.0f,2.0f,-6.2f,
        -2.4f,2.0f,-4.0f,
        -2.0f,0.0f,-4.0f,
        0.0f,0.0f,-6.0f,
        -2.2f,2.0f,-4.0f,
        0.0f,0.5f,-6.0f,
        0.0f,2.0f,-6.0f,
        -2.2f,2.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        0.0f,0.5f,-6.0f,
        //side
        2.4f,2.0f,-4.0f,
        0.0f,2.0f,-6.0f,
        0.0f,2.0f,-6.2f,
        2.4f,2.0f,-4.0f,
        2.2f,2.0f,-4.0f,
        0.0f,2.0f,-6.0f,
        0.0f,2.0f,-6.2f,
        0.0f,0.5f,-6.0f,
        0.0f,0.0f,-6.0f,
        0.0f,2.0f,-6.2f,
        0.0f,2.0f,-6.0f,
        0.0f,0.5f,-6.0f,
        0.0f,0.0f,-6.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.0f,-4.0f,
        0.0f,0.0f,-6.0f,
        0.0f,0.5f,-6.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.2f,2.0f,-4.0f,
        2.4f,2.0f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.0f,0.5f,-4.0f,
        2.2f,2.0f,-4.0f,
        2.4f,2.0f,-4.0f,
        0.0f,0.0f,-6.0f,
        0.0f,2.0f,-6.2f,
        2.4f,2.0f,-4.0f,
        2.0f,0.0f,-4.0f,
        0.0f,0.0f,-6.0f,
        2.2f,2.0f,-4.0f,
        0.0f,0.5f,-6.0f,
        0.0f,2.0f,-6.0f,
        2.2f,2.0f,-4.0f,
        2.0f,0.5f,-4.0f,
        0.0f,0.5f,-6.0f,
        //side
        2.4f,2.0f,-4.0f,
        2.2f,2.0f,4.0f,
        2.4f,2.0f,4.0f,
        2.4f,2.0f,-4.0f,
        2.2f,2.0f,-4.0f,
        2.2f,2.0f,4.0f,
        2.4f,2.0f,4.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.0f,4.0f,
        2.4f,2.0f,4.0f,
        2.2f,2.0f,4.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.0f,4.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.0f,0.0f,4.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.2f,2.0f,-4.0f,
        2.4f,2.0f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.0f,0.5f,-4.0f,
        2.2f,2.0f,-4.0f,
        2.4f,2.0f,-4.0f,
        2.0f,0.0f,4.0f,
        2.4f,2.0f,4.0f,
        2.4f,2.0f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.0f,0.0f,4.0f,
        2.2f,2.0f,-4.0f,
        2.0f,0.5f,4.0f,
        2.2f,2.0f,4.0f,
        2.2f,2.0f,-4.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.5f,4.0f,
        //next side
        2.4f,2.0f,4.0f,
        0.0f,2.0f,6.0f,
        0.0f,2.0f,6.2f,
        2.4f,2.0f,4.0f,
        2.2f,2.0f,4.0f,
        0.0f,2.0f,6.0f,
        0.0f,2.0f,6.2f,
        0.0f,0.5f,6.0f,
        0.0f,0.0f,6.0f,
        0.0f,2.0f,6.2f,
        0.0f,2.0f,6.0f,
        0.0f,0.5f,6.0f,
        0.0f,0.0f,6.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.0f,4.0f,
        0.0f,0.0f,6.0f,
        0.0f,0.5f,6.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.0f,4.0f,
        2.2f,2.0f,4.0f,
        2.4f,2.0f,4.0f,
        2.0f,0.0f,4.0f,
        2.0f,0.5f,4.0f,
        2.2f,2.0f,4.0f,
        2.4f,2.0f,4.0f,
        0.0f,0.0f,6.0f,
        0.0f,2.0f,6.2f,
        2.4f,2.0f,4.0f,
        2.0f,0.0f,4.0f,
        0.0f,0.0f,6.0f,
        2.2f,2.0f,4.0f,
        0.0f,0.5f,6.0f,
        0.0f,2.0f,6.0f,
        2.2f,2.0f,4.0f,
        2.0f,0.5f,4.0f,
        0.0f,0.5f,6.0f, 
    };

    static const GLfloat base_data[] = {
        //bottom
        -2.0f,0.0f,-4.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.0f,4.0f,
        -2.0f,0.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.0f,4.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.0f,4.0f,
        -2.0f,0.0f,4.0f,
        -2.0f,0.5f,4.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.0f,4.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.0f,0.0f,4.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.0f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.0f,0.5f,-4.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.0f,-4.0f,
        2.0f,0.0f,4.0f,
        -2.0f,0.0f,4.0f,
        -2.0f,0.0f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.0f,0.0f,4.0f,
        -2.0f,0.5f,-4.0f,
        2.0f,0.5f,4.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.5f,-4.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.5f,4.0f,
        //front triangle
        -2.0f,0.0f,4.0f,
        0.0f,0.5f,6.0f,
        0.0f,0.0f,6.0f,
        -2.0f,0.0f,4.0f,
        -2.0f,0.5f,4.0f,
        0.0f,0.5f,6.0f,
        0.0f,0.0f,6.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.0f,4.0f,
        0.0f,0.0f,6.0f,
        0.0f,0.5f,6.0f,
        2.0f,0.5f,4.0f,
        2.0f,0.0f,4.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.0f,4.0f,
        2.0f,0.0f,4.0f,
        2.0f,0.5f,4.0f,
        -2.0f,0.5f,4.0f,
        -2.0f,0.0f,4.0f,
        0.0f,0.0f,6.0f,
        2.0f,0.0f,4.0f,
        -2.0f,0.5f,4.0f,
        0.0f,0.5f,6.0f,
        2.0f,0.5f,4.0f,
        //back triangle
        -2.0f,0.0f,-4.0f,
        0.0f,0.5f,-6.0f,
        0.0f,0.0f,-6.0f,
        -2.0f,0.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        0.0f,0.5f,-6.0f,
        0.0f,0.0f,-6.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.0f,-4.0f,
        0.0f,0.0f,-6.0f,
        0.0f,0.5f,-6.0f,
        2.0f,0.5f,-4.0f,
        2.0f,0.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.0f,-4.0f,
        2.0f,0.0f,-4.0f,
        2.0f,0.5f,-4.0f,
        -2.0f,0.5f,-4.0f,
        -2.0f,0.0f,-4.0f,
        0.0f,0.0f,-6.0f,
        2.0f,0.0f,-4.0f,
        -2.0f,0.5f,-4.0f,
        0.0f,0.5f,-6.0f,
        2.0f,0.5f,-4.0f,
    };

    this->base = create3DObject(GL_TRIANGLES, (12+8+8)*3, base_data, colorbase, GL_FILL);
    this->side = create3DObject(GL_TRIANGLES, (12*6)*3, side_data, colorside, GL_FILL);
    this->pole = create3DObject(GL_TRIANGLES, (24*2)*3, pole_data, colorpole, GL_FILL);
    this->sail = create3DObject(GL_TRIANGLES, (8)*3, sail_data, colorsail, GL_FILL);
    this->canon = create3DObject(GL_TRIANGLES, (8)*3, canon_data, colorcanon, GL_FILL);
    this->cover = create3DObject(GL_TRIANGLES, (3)*3, cover_data, colorcover, GL_FILL);
}


