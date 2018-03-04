#include "rock.h"
#include "main.h"

void Rock::draw(glm::mat4 VP) {
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

void Rock::tick() {
}

Rock::Rock(float x, float y) {
    position = glm::vec3(0, 0, 0);
    position.x = x, position.z = y;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    GLfloat rock_data[] = {
        //FINAL
        -4.0f,0.0f,4.0f,
        -4.0f,10.0f,-4.0f,
        -4.0f,0.0f,-4.0f,
        -4.0f,0.0f,4.0f,
        -4.0f,10.0f,4.0f,
        -4.0f,10.0f,-4.0f,
        -4.0f,0.0f,-4.0f,
        4.0f,10.0f,-4.0f,
        4.0f,0.0f,-4.0f,
        -4.0f,0.0f,-4.0f,
        -4.0f,10.0f,-4.0f,
        4.0f,10.0f,-4.0f,
        4.0f,0.0f,-4.0f,
        4.0f,10.0f,4.0f,
        4.0f,0.0f,4.0f,
        4.0f,0.0f,-4.0f,
        4.0f,10.0f,-4.0f,
        4.0f,10.0f,4.0f,
        4.0f,0.0f,4.0f,
        -4.0f,10.0f,4.0f,
        -4.0f,0.0f,4.0f,
        4.0f,0.0f,4.0f,
        4.0f,10.0f,4.0f,
        -4.0f,10.0f,4.0f,
        -4.0f,0.0f,4.0f,
        4.0f,0.0f,-4.0f,
        -4.0f,0.0f,-4.0f,
        -4.0f,0.0f,4.0f,
        4.0f,0.0f,4.0f,
        4.0f,0.0f,-4.0f,
        -4.0f,10.0f,4.0f,
        4.0f,10.0f,-4.0f,
        -4.0f,10.0f,-4.0f,
        -4.0f,10.0f,4.0f,
        4.0f,10.0f,4.0f,
        4.0f,10.0f,-4.0f,
        //grass
        -4.0f,11.0f,4.0f,
        -4.0f,10.0f,-4.0f,
        -4.0f,11.0f,-4.0f,
        -4.0f,11.0f,4.0f,
        -4.0f,10.0f,4.0f,
        -4.0f,10.0f,-4.0f,
        -4.0f,11.0f,-4.0f,
        4.0f,10.0f,-4.0f,
        4.0f,11.0f,-4.0f,
        -4.0f,11.0f,-4.0f,
        -4.0f,10.0f,-4.0f,
        4.0f,10.0f,-4.0f,
        4.0f,11.0f,-4.0f,
        4.0f,10.0f,4.0f,
        4.0f,11.0f,4.0f,
        4.0f,11.0f,-4.0f,
        4.0f,10.0f,-4.0f,
        4.0f,10.0f,4.0f,
        4.0f,11.0f,4.0f,
        -4.0f,10.0f,4.0f,
        -4.0f,11.0f,4.0f,
        4.0f,11.0f,4.0f,
        4.0f,10.0f,4.0f,
        -4.0f,10.0f,4.0f,
        -4.0f,11.0f,4.0f,
        4.0f,11.0f,-4.0f,
        -4.0f,11.0f,-4.0f,
        -4.0f,11.0f,4.0f,
        4.0f,11.0f,4.0f,
        4.0f,11.0f,-4.0f,
        -4.0f,10.0f,4.0f,
        4.0f,10.0f,-4.0f,
        -4.0f,10.0f,-4.0f,
        -4.0f,10.0f,4.0f,
        4.0f,10.0f,4.0f,
        4.0f,10.0f,-4.0f,
    };

    GLfloat colorrock[] = {
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.372f, 0.388f, 0.396f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        0.631f, 0.643f, 0.650f,
        //green
        0.552,1,0,
        0.776,1,0.498,
        0.552,1,0,
        0.552,1,0,
        0.776,1,0.498,
        0.776,1,0.498,
        0.552,1,0,
        0.776,1,0.498,
        0.552,1,0,
        0.552,1,0,
        0.776,1,0.498,
        0.776,1,0.498,
        0.552,1,0,
        0.776,1,0.498,
        0.552,1,0,
        0.552,1,0,
        0.776,1,0.498,
        0.776,1,0.498,
        0.552,1,0,
        0.776,1,0.498,
        0.552,1,0,
        0.552,1,0,
        0.776,1,0.498,
        0.776,1,0.498,
        0.552,1,0,
        0.552,1,0,
        0.552,1,0,
        0.552,1,0,
        0.552,1,0,
        0.552,1,0,
        0.776,1,0.498,
        0.776,1,0.498,
        0.776,1,0.498,
        0.776,1,0.498,
        0.776,1,0.498,
        0.776,1,0.498,
    };

    this->rock = create3DObject(GL_TRIANGLES, (2*12)*3, rock_data, colorrock, GL_FILL);
}


