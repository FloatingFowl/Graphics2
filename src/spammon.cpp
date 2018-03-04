#include "spammon.h"
#include "main.h"

void Spammon::draw(glm::mat4 VP) {
    if (dead) return;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->rock);
}

Spammon::Spammon(float x, float y, color_t color) {
    dead = false;
    position = glm::vec3(x, 0, y);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    GLfloat rock_data[] = {
        //
        -7.0f,0.0f,-7.0f,
        -7.0f,3.0f,7.0f,
        -7.0f,0.0f,7.0f,
        -7.0f,0.0f,-7.0f,
        -7.0f,3.0f,-7.0f,
        -7.0f,3.0f,7.0f,
        -7.0f,0.0f,7.0f,
        7.0f,3.0f,7.0f,
        7.0f,0.0f,7.0f,
        -7.0f,0.0f,7.0f,
        -7.0f,3.0f,7.0f,
        7.0f,3.0f,7.0f,
        7.0f,0.0f,7.0f,
        7.0f,3.0f,-7.0f,
        7.0f,0.0f,-7.0f,
        7.0f,0.0f,7.0f,
        7.0f,3.0f,7.0f,
        7.0f,3.0f,-7.0f,
        7.0f,0.0f,-7.0f,
        -7.0f,3.0f,-7.0f,
        -7.0f,0.0f,-7.0f,
        7.0f,0.0f,-7.0f,
        7.0f,3.0f,-7.0f,
        -7.0f,3.0f,-7.0f,
        -7.0f,0.0f,-7.0f,
        7.0f,0.0f,7.0f,
        -7.0f,0.0f,7.0f,
        -7.0f,0.0f,-7.0f,
        7.0f,0.0f,-7.0f,
        7.0f,0.0f,7.0f,
        -7.0f,3.0f,-7.0f,
        7.0f,3.0f,7.0f,
        -7.0f,3.0f,7.0f,
        -7.0f,3.0f,-7.0f,
        7.0f,3.0f,-7.0f,
        7.0f,3.0f,7.0f,
        //next
        -5.0f,3.0f,2.0f,
        -5.0f,13.0f,-2.0f,
        -5.0f,3.0f,-2.0f,
        -5.0f,3.0f,2.0f,
        -5.0f,13.0f,2.0f,
        -5.0f,13.0f,-2.0f,
        -5.0f,3.0f,-2.0f,
        -3.0f,13.0f,-2.0f,
        -3.0f,3.0f,-2.0f,
        -5.0f,3.0f,-2.0f,
        -5.0f,13.0f,-2.0f,
        -3.0f,13.0f,-2.0f,
        -3.0f,3.0f,-2.0f,
        -3.0f,13.0f,2.0f,
        -3.0f,3.0f,2.0f,
        -3.0f,3.0f,-2.0f,
        -3.0f,13.0f,-2.0f,
        -3.0f,13.0f,2.0f,
        -3.0f,3.0f,2.0f,
        -5.0f,13.0f,2.0f,
        -5.0f,3.0f,2.0f,
        -3.0f,3.0f,2.0f,
        -3.0f,13.0f,2.0f,
        -5.0f,13.0f,2.0f,
        -5.0f,3.0f,2.0f,
        -3.0f,3.0f,-2.0f,
        -5.0f,3.0f,-2.0f,
        -5.0f,3.0f,2.0f,
        -3.0f,3.0f,2.0f,
        -3.0f,3.0f,-2.0f,
        -5.0f,13.0f,2.0f,
        -3.0f,13.0f,-2.0f,
        -5.0f,13.0f,-2.0f,
        -5.0f,13.0f,2.0f,
        -3.0f,13.0f,2.0f,
        -3.0f,13.0f,-2.0f,
        //
        5.0f,3.0f,2.0f,
        5.0f,13.0f,-2.0f,
        5.0f,3.0f,-2.0f,
        5.0f,3.0f,2.0f,
        5.0f,13.0f,2.0f,
        5.0f,13.0f,-2.0f,
        5.0f,3.0f,-2.0f,
        3.0f,13.0f,-2.0f,
        3.0f,3.0f,-2.0f,
        5.0f,3.0f,-2.0f,
        5.0f,13.0f,-2.0f,
        3.0f,13.0f,-2.0f,
        3.0f,3.0f,-2.0f,
        3.0f,13.0f,2.0f,
        3.0f,3.0f,2.0f,
        3.0f,3.0f,-2.0f,
        3.0f,13.0f,-2.0f,
        3.0f,13.0f,2.0f,
        3.0f,3.0f,2.0f,
        5.0f,13.0f,2.0f,
        5.0f,3.0f,2.0f,
        3.0f,3.0f,2.0f,
        3.0f,13.0f,2.0f,
        5.0f,13.0f,2.0f,
        5.0f,3.0f,2.0f,
        3.0f,3.0f,-2.0f,
        5.0f,3.0f,-2.0f,
        5.0f,3.0f,2.0f,
        3.0f,3.0f,2.0f,
        3.0f,3.0f,-2.0f,
        5.0f,13.0f,2.0f,
        3.0f,13.0f,-2.0f,
        5.0f,13.0f,-2.0f,
        5.0f,13.0f,2.0f,
        3.0f,13.0f,2.0f,
        3.0f,13.0f,-2.0f,
    };

    GLfloat colorrock[] = {
        5.0f, 5.0f, 5.0f,
        5.0f, 5.0f, 5.0f,
    };

    this->rock = create3DObject(GL_TRIANGLES, (12*3)*3, rock_data, colorrock, GL_FILL);
}


