#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"

bool   cannon_keyboard_input = true;
bool   drag_pan = false, old_cki;
double drag_oldx = -1, drag_oldy = -1;
float camera_zoom = 0, camera_look_x = -170, camera_look_y = 60;

using namespace std;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        // switch (key) {
        // case GLFW_KEY_C:
        // rectangle_rot_status = !rectangle_rot_status;
        // break;
        // case GLFW_KEY_P:
        // triangle_rot_status = !triangle_rot_status;
        // break;
        // case GLFW_KEY_X:
        //// do something ..
        // break;
        // default:
        // break;
        // }
    } else if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            quit(window);
            break;
        default:
            break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar(GLFWwindow *window, unsigned int key) {
    switch (key) {
    case 'Q':
    case 'q':
        quit(window);
        break;
    default:
        break;
    }
}

int prev_xpos = 0, prev_ypos = 0;

/* Executed when a mouse button is pressed/released */
void mouseButton(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        prev_xpos = xpos;
        prev_ypos = ypos;
    }
}

/* Process continuous input */
/*void tick_input(GLFWwindow* window) {*/

    //int width, height;
    //glfwGetWindowSize(window, &width, &height);
    //double xpos, ypos;
    //glfwGetCursorPos(window, &xpos, &ypos);
    //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        //camera_look_x -= -360 + (xpos - prev_xpos) * 90 / width;
        //camera_look_y -= -360 + (ypos - prev_ypos) * 90 / height;
        //camera_look_x = camera_look_x > 360 ? camera_look_x - ((int) (camera_look_x / 360))*360 : camera_look_x;
        //camera_look_y = camera_look_y > 360 ? camera_look_y - ((int) (camera_look_y / 360))*360 : camera_look_y;
        //prev_xpos = xpos;
        //prev_ypos = ypos;
    //}
/*}*/


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera_zoom -= yoffset;
    // Do something
}
