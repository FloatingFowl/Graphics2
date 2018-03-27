#include "main.h"
#include "timer.h"
#include "ball.h"
#include "boat.h"
#include "sea.h"
#include "bomb.h"
#include "rock.h"
#include "spammon.h"
#include "barrel.h"
#include "monster.h"
#include <sstream>

template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
 * Customizable functions *
 **************************/

Boat boat;
Sea sea;
Bomb bomb;
vector<Rock> rocks;
vector<Spammon> spammons;
vector<Barrel> barrels;
Monster *monster = NULL;
bool orth = false;
vector<Bomb *> oppbombs(10, NULL);

const int nRocks = 10;
int nCamera = 0;
int killed = 0;
bool won = false;
bool wind = false;
glm::vec3 eye, target, up;
float screen_zoom = 1.0f, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
bool fsttime = true;

Timer t60(1.0 / 60);
Timer t04(1.0 / 10);
Timer t10sec(10.0);
Timer t05sec(4.0);
Timer *t10booster;

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    camera_func();

    // Eye - Location of camera. Don't change unless you are sure!!
    //eye = glm::vec3( boat.position.x - sin((15.0 + boat.rotation +camera_rotation_angle)*M_PI/180.0)*15, 6, boat.position.z - 15*cos((15.0+boat.rotation+camera_rotation_angle)*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    //target = glm::vec3(boat.position.x, 1, boat.position.z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    //up = glm::vec3(0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    if(monster != NULL) {
        monster->draw(VP);
    }
    sea.draw(VP);
    boat.draw(VP);
    bomb.draw(VP);
    for(int i = 0; i < 10; i++)
        rocks[i].draw(VP);
    for(int i = 0; i < spammons.size(); i++)
        spammons[i].draw(VP);
    for(int i = 0; i < barrels.size(); i++)
        barrels[i].draw(VP);
    for (int i = 0; i < oppbombs.size(); i++){
        if(oppbombs[i])
            oppbombs[i]->draw(VP);
        else break;
    }
    std::string fin = "Lifes left: " + NumberToString(boat.life) + "| Points: " + NumberToString(boat.points);
    glfwSetWindowTitle(window, fin.c_str());
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int f = glfwGetKey(window, GLFW_KEY_F);
    int v = glfwGetKey(window, GLFW_KEY_V);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int s = glfwGetKey(window, GLFW_KEY_S);
    int q = glfwGetKey(window, GLFW_KEY_E);
    if (up) {
        boat.motor_speed = 10.0f;
    }
    if (right) {
        boat.rotation -= 1.0f;
    }
    if (left) {
        boat.rotation += 1.0f;
    }
    if (f && !bomb.shot) {
        if(boat.booster > 1.0f)
            bomb.start(boat.position.x, boat.position.y, boat.position.z, boat.rotation + boat.crot, true);
        else
            bomb.start(boat.position.x, boat.position.y, boat.position.z, boat.rotation + boat.crot, false);
    }
    if (space && !boat.jump) {
        boat.jump = true;
        boat.yspeed = 5.0f;
    }
    if (t04.processTick() && v) {
        nCamera = (nCamera + 1) % 5;
    }
    if(a) {
        boat.crot = min(45.0f, boat.crot + 1.0f);
    }
    if(s) {
        boat.crot = max(-45.0f, boat.crot - 1.0f);
    }
    if(q) {
        changeOrthoPersp();
    }
    //
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera_look_x -= -360 + (xpos - prev_xpos) * 90 / width;
        camera_look_y -= -360 + (ypos - prev_ypos) * 90 / height;
        camera_look_x = camera_look_x > 360 ? camera_look_x - ((int) (camera_look_x / 360))*360 : camera_look_x;
        camera_look_y = camera_look_y > 360 ? camera_look_y - ((int) (camera_look_y / 360))*360 : camera_look_y;
        prev_xpos = xpos;
        prev_ypos = ypos;
    }

}

void tick_elements() {
    boat.tick();
    sea.tick();
    bomb.tick();
    for (int i = 0; i < oppbombs.size(); i++){
        if(oppbombs[i] == NULL) break;
        oppbombs[i]->tick();
    }
    //camera_rotation_angle += 1;
    if(bomb.shot)
        for(int i = 0; i < nRocks; i++){
            bool hit = ccBoxBox(bomb.position.x, bomb.position.y, bomb.position.z, 0.25f, 0.25f, 0.25f,
                    rocks[i].position.x, rocks[i].position.y + 5.5f, rocks[i].position.z, 4.0f, 5.5f, 4.0f
                    );
            if(hit == true){
                bomb.shot = false;
            }
        }
    std::vector<std::pair<float,float> > a;
    std::vector<std::pair<float,float> > b;
    float m1 = sin(boat.rotation * M_PI / 180.0);
    float m2 = sin(boat.rotation * M_PI / 180.0);
    a.push_back(make_pair(boat.position.x - 2.0f * m1, boat.position.z - 4.0f * m2));
    a.push_back(make_pair(boat.position.x, boat.position.z - 6.0f * m2));
    a.push_back(make_pair(boat.position.x + 2.0f * m1, boat.position.z - 4.0f * m2));
    a.push_back(make_pair(boat.position.x + 2.0f * m1, boat.position.z + 4.0f * m2));
    a.push_back(make_pair(boat.position.x, boat.position.z + 6.0f * m2));
    a.push_back(make_pair(boat.position.x - 2.0f * m1, boat.position.z + 4.0f * m2));
    for(int i = 0; i < nRocks; i++){
        b.clear();
        b.push_back(make_pair(rocks[i].position.x - 4.0f, rocks[i].position.z - 4.0f));
        b.push_back(make_pair(rocks[i].position.x + 4.0f, rocks[i].position.z - 4.0f));
        b.push_back(make_pair(rocks[i].position.x + 4.0f, rocks[i].position.z + 4.0f));
        b.push_back(make_pair(rocks[i].position.x - 4.0f, rocks[i].position.z + 4.0f));
        bool hit = ccPolyPoly(a,b);
        if(hit){
            boat.reactionspeed = 25.0f;
            boat.points -= 10;
        };
    }
    if(bomb.shot)
        for(int i = 0; i < spammons.size(); i++){
            bool hit = ccBoxBox(bomb.position.x, bomb.position.y, bomb.position.z, 0.25f, 0.25f, 0.25f,
                spammons[i].position.x-4.0f,spammons[i].position.y+8.0f,spammons[i].position.z+0.0f,1.0f,5.0f,2.0f
                    );
            hit |= ccBoxBox(bomb.position.x, bomb.position.y, bomb.position.z, 0.25f, 0.25f, 0.25f,
                spammons[i].position.x+4.0f,spammons[i].position.y+8.0f,spammons[i].position.z+0.0f,1.0f,5.0f,2.0f
                    );
            if(hit){
                barrels.push_back(Barrel(spammons[i].position.x, spammons[i].position.z));
                spammons.erase(spammons.begin() + i);
                oppbombs.erase(oppbombs.begin() + i);
                oppbombs.push_back(NULL);
                boat.points += 10;
                killed++;
                if(killed == 3)
                    monster = new Monster(0,0);
                break;
            };
        }
    for(int i = 0; i < spammons.size(); i++){
        b.clear();
        b.push_back(make_pair(spammons[i].position.x-7.0f,spammons[i].position.z+7.0f));
        b.push_back(make_pair(spammons[i].position.x-7.0f,spammons[i].position.z-7.0f));
        b.push_back(make_pair(spammons[i].position.x+7.0f,spammons[i].position.z-7.0f));
        b.push_back(make_pair(spammons[i].position.x+7.0f,spammons[i].position.z+7.0f));
        bool hit = ccPolyPoly(a,b);
        if(hit){
            boat.reactionspeed = 25.0f;
            boat.life -= 1;
            boat.points -= 5;
        };
    }
    for(int i = 0; i < barrels.size(); i++){
        b.clear();
        b.push_back(make_pair(barrels[i].position.x-1.0f,barrels[i].position.z-1.0f));
        b.push_back(make_pair(barrels[i].position.x-1.0f,barrels[i].position.z+1.0f));
        b.push_back(make_pair(barrels[i].position.x+1.0f,barrels[i].position.z+1.0f));
        b.push_back(make_pair(barrels[i].position.x+1.0f,barrels[i].position.z-1.0f));
        bool hit = ccPolyPoly(a,b);
        hit &= boat.jump;
        if(hit){
            if(barrels[i].type == 0){
                boat.booster = 4.0f;
                t10booster = new Timer(10.0);
            }
            else if(barrels[i].type == 1){
                boat.life = min(3, boat.life + 1);
            }
            else{
                boat.points += 5;
            }
            barrels.erase(barrels.begin() + i);
        }
    }
    if(bomb.shot && monster){
        bool hit = ccBoxBox(bomb.position.x, bomb.position.y, bomb.position.z, 0.25f, 0.25f, 0.25f,
            monster->position.x, 6.5, monster->position.z, 2.0f, 6.5f, 2.0f
                );
        if(hit){
            bomb.shot = false;
        }
        else{
            hit = ccBoxBox(bomb.position.x, bomb.position.y, bomb.position.z, 0.25f, 0.25f, 0.25f,
                monster->position.x, 13.5, monster->position.z, 2.0f, 0.5f, 2.0f
                    );
            if(hit){
                cout<<"YOU WON!\n";
                cout<<"You got "<<boat.points<<" points!\n";
                exit(0);
            }
        }
    }
    for(int i = 0; i < a.size(); i++){
        pair<float,float> &temp = a[i];
        if(std::abs(temp.first)>200.0 || std::abs(temp.second)>200.0)
        {
            cout<<"YOU LOST!\n";
            cout<<"Total points "<<boat.points<<'\n';
            exit(0);
        }
    }
    for(int i = 0; i < spammons.size(); i++){
        if(!oppbombs[i]) break;
        if(!oppbombs[i]->shot) continue;
        b.clear();
        b.push_back(make_pair(oppbombs[i]->position.x+0.25f,oppbombs[i]->position.z+0.25f));
        b.push_back(make_pair(oppbombs[i]->position.x+0.25f,oppbombs[i]->position.z-0.25f));
        b.push_back(make_pair(oppbombs[i]->position.x-0.25f,oppbombs[i]->position.z-0.25f));
        b.push_back(make_pair(oppbombs[i]->position.x-0.25f,oppbombs[i]->position.z+0.25f));
        bool hit = ccPolyPoly(a,b);
        if (hit && oppbombs[i]->position.y - 0.25f <= boat.position.y + 1.5f)
        {
            boat.life -= 1;
            oppbombs[i]->shot = false;
            break;
        }
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat = Boat(COLOR_RED, COLOR_BROWN, COLOR_SILVER, COLOR_WHITE, COLOR_ARMYGREEN, COLOR_BLACK);
    sea = Sea(COLOR_BROWN);
    bomb = Bomb(COLOR_FIRE1, COLOR_FIRE2, COLOR_FIRE3);
    for (int i = 0; i < 10; i++){
        float x = ((400.0) * ((float)rand() / RAND_MAX)) - 200.0;
        float y = ((400.0) * ((float)rand() / RAND_MAX)) - 200.0;
        rocks.push_back(Rock(x, y));
    }

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);
    t10booster = NULL;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        
        if(boat.life == 0){
            cout<<"GAME OVER\n";
            cout<<"YOU GOT "<<boat.points<<" POINTS\n";
            break;
        }

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        if (t10sec.processTick()) {
            wind = (rand() % 2 == 1);
            if (wind) {
                if(rand()%2)
                    boat.windspeedz = (rand() % 2 == 1) * (-1.0f) + 0.5f;
                else
                    boat.windspeedx = (rand() % 2 == 1) * (-1.0f) + 0.5f;
            }
            else boat.windspeedx = boat.windspeedz = 0;

            if(spammons.size() < 10){
                float x = ((400.0) * ((float)rand() / RAND_MAX)) - 200.0;
                float y = ((400.0) * ((float)rand() / RAND_MAX)) - 200.0;
                spammons.push_back(Spammon(x,y,COLOR_FIRE1));
                oppbombs[spammons.size() - 1] = new Bomb(COLOR_FIRE1, COLOR_FIRE2, COLOR_FIRE3);
            }
        }

        if (t10booster && t10booster->processTick()) {
            if(fsttime){
                fsttime = false;
                continue;
            }
            fsttime = true;
            boat.booster = 1.0f;
            delete t10booster;
            t10booster = NULL;
        }

        if (t05sec.processTick()) {
            for(int i = 0; i < oppbombs.size(); i++){
                if(!oppbombs[i]) break;
                //oppbombs[i]->start(boat.position.x, boat.position.y, boat.position.z, boat.rotation + boat.crot, false);
                oppbombs[i]->start(spammons[i].position.x,spammons[i].position.y, spammons[i].position.z, rand(), false);
            }
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
        (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 22 / screen_zoom;
    float bottom = screen_center_y - 22 / screen_zoom;
    float left   = screen_center_x - 22 / screen_zoom;
    float right  = screen_center_x + 22 / screen_zoom;
    //Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 200.0f);
    float l = right - left, r = top - bottom;
    Matrices.projection = glm::perspective((GLfloat)M_PI/2, (GLfloat)l/r, 0.1f, 500.0f);
}

void camera_func() {
    //case for follow-cam view 
    if (nCamera == 0){
        eye = glm::vec3(boat.position.x - sin((15.0 + boat.rotation)*M_PI/180.0)*15, 6, boat.position.z - 15*cos((15.0+boat.rotation)*M_PI/180.0f) );
        target = glm::vec3(boat.position.x, 1, boat.position.z);
        up = glm::vec3(0, 1, 0);
    }
    //case for boat view
    else if (nCamera == 1){
        eye = glm::vec3(boat.position.x + sin(boat.rotation * M_PI/180.0)*6.0, boat.position.y + 2.5f, boat.position.z + cos(boat.rotation*M_PI/180.0)*(6));
        target = glm::vec3(boat.position.x + sin(boat.rotation * M_PI/180.0)*(6.5), boat.position.y + 2.5f, boat.position.z + cos(boat.rotation*M_PI/180.0)*(6.5));
        up = glm::vec3(0, 1, 0);
    }
    //case for top view
    else if (nCamera == 2){
        eye = glm::vec3(boat.position.x, 40.0, boat.position.z);
        target = glm::vec3(boat.position.x, boat.position.y, boat.position.z);
        up = glm::vec3(sin(boat.rotation*M_PI/180.0), 0, cos(boat.rotation*M_PI/180.0));
        //up = glm::vec3(1, 0, 0);
    }
    //case for tower view
    else if (nCamera == 3){
        if(boat.position.x <= 0.0){
            if (boat.position.z <= 0.0)
                eye = glm::vec3(-200.0, 20.0, -200.0);
            else
                eye = glm::vec3(-200.0, 20.0, 200.0);
        }
        else{
            if (boat.position.z <= 0.0)
                eye = glm::vec3(200.0, 20.0, -200.0);
            else
                eye = glm::vec3(200.0, 20.0, 200.0);
        }
        target = glm::vec3(boat.position.x, boat.position.y, boat.position.z);
        up = glm::vec3(0, 1, 0);
    }
    //case for helicopter view
    else if (nCamera == 4){
        float b = (20-camera_zoom/5)*cos(camera_look_y*M_PI/180.0f);
        float c = 10 + boat.position.z + (20-camera_zoom/5)*sin(camera_look_x*M_PI/180.0f)*sin(camera_look_y*M_PI/180.0f);
        float a = 10 + boat.position.x + (20-camera_zoom/5)*cos(camera_look_x*M_PI/180.0f)*sin(camera_look_y*M_PI/180.0f);
        eye = glm::vec3(a,b,c);
        target = glm::vec3(boat.position.x, boat.position.y + 1.5f, boat.position.z);
        up = glm::vec3(0, 1, 0);
    }
}

bool ccBoxBox(float ax, float ay, float az, float ahx, float ahy, float ahz, 
        float bx, float by, float bz, float bhx, float bhy, float bhz
        )
{
    if(abs(ax-bx) < ahx + bhx &&
            abs(ay - by) < ahy + bhy &&
            abs(az - bz) < ahz + bhz)
        return true;
    return false;
}

float ccPolyPoly(std::vector<std::pair<float,float> >& a, std::vector<std::pair<float,float> >& b){
    float minA, maxA, projected, i, i1, j, minB, maxB;

    for(int polyi = 0; polyi < 2; ++polyi)
    {
        const vector<pair<float,float> >& polygon = polyi == 0 ? a : b;

        for(int i1 = 0; i1 < polygon.size(); ++i1)
        {
            const int i2 = (i1 + 1) % polygon.size();

            const double normalx = polygon[i2].second - polygon[i1].second;
            const double normaly = polygon[i2].first - polygon[i1].first;

            double minA = std::numeric_limits<double>::max();
            double maxA = std::numeric_limits<double>::min();
            for(int ai = 0; ai < a.size(); ++ai)
            {
                const double projected = normalx * a[ai].first + normaly * a[ai].second;
                if( projected < minA ) minA = projected;
                if( projected > maxA ) maxA = projected;
            }

            double minB = std::numeric_limits<double>::max();
            double maxB = std::numeric_limits<double>::min();
            for(int bi = 0; bi < b.size(); ++bi)
            {
                const double projected = normalx * b[bi].first + normaly * b[bi].second;
                if( projected < minB ) minB = projected;
                if( projected > maxB ) maxB = projected;
            }

            if( maxA < minB || maxB < minA )
                return false;
        }
    }
    return true;
}

void changeOrthoPersp(){
    float top    = screen_center_y + 22 / screen_zoom;
    float bottom = screen_center_y - 22 / screen_zoom;
    float left   = screen_center_x - 22 / screen_zoom;
    float right  = screen_center_x + 22 / screen_zoom;
    if (orth){
        orth = false;
        Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 200.0f);
    }
    else{
        float l = right - left, r = top - bottom;
        Matrices.projection = glm::perspective((GLfloat)M_PI/2, (GLfloat)l/r, 0.1f, 500.0f);
        orth = true;
    }
}
