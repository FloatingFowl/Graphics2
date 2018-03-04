#include "sea.h"
#include "main.h"

void Sea::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->sea);
}

Sea::Sea(color_t colorsea) {
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;
    speed = 0.01;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    static const GLfloat sea_data[] = {
        //cover
        500.0f,0.0f,500.0f,
        -500.0f,0.0f,500.0f,
        500.0f,0.0f,-500.0f,
        -500.0f,0.0f,500.0f,
        500.0f,0.0f,-500.0f,
        -500.0f,0.0f,-500.0f, 
    };

    GLfloat sea_color[] = {
        0,0.749,1,
        0.254,0.411,1,
        0.254,0.411,1,
        0.254,0.411,1,
        0.254,0.411,1,
        0,0.749,1,
    };

    //this->sea = create3DObject(GL_TRIANGLES, (2)*3, sea_data, colorsea, GL_FILL);
    this->sea = create3DObject(GL_TRIANGLES, (2)*3, sea_data, sea_color, GL_FILL);
}

void Sea::tick() {
    position.y += speed;
    if (position.y > 0.4 || position.y < 0.0) {
        speed = -speed;
        position.y += speed;
    }
}
