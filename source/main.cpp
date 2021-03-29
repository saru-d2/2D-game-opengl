#include "main.h"
#include "mazeBg.h"
#include "timer.h"
#include "ball.h"
#include "maze.h"
#include "agent.h"
#include "button.h"
#include "hud.h"
#include "lightBlocker.h"
#include "exitEdge.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

void gameLogic();
void endGame(int);
/**************************
* Customizable functions *
**************************/
int lightheight;
bool gameOver = false;
bool isLighted = false;
float radius;
int lives = 5;
double gameTime = 0.0;
Button killButton;
Button powerButton;
Ball ball1;
Maze maze;
Agent crewmate;
Agent imposter;
HUD hud;
MazeBg mazeBg;
vector<LightBlocker> lightblockers;
ExitEdge exitEdge;
double score = 0.0;
float timeInDark = 0.0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw()
{
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye(maze.r / 2, maze.c / 2, 5);

    // glm::vec3 eye(5 * cos(camera_rotation_angle * M_PI / 180.0f), maze.c / 2, 5 * sin(camera_rotation_angle * M_PI / 180.0f));
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target = eye + glm::vec3(0, 0, -3);

    // glm::vec3 target(maze.r / 2, maze.c / 2, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up(0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // feeding position of the crewmate to the shader
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP; // MVP = Projection * View * Model
    glm::vec3 agentPos;
    // if (isLighted)
    //     lightheight = -4;
    // else
    lightheight = -1;

    // // Scene render
    if (!gameOver)
    {
        agentPos = glm::vec3(
            crewmate.x + 0.5, crewmate.y + 0.5, lightheight);
        glUniform3fv(glGetUniformLocation(programID, "agentPos"), 1, &agentPos[0]);
        mazeBg.draw(VP);
        maze.draw(VP);

        agentPos = glm::vec3(
            crewmate.x - killButton.x, crewmate.y - killButton.y, lightheight);
        glUniform3fv(glGetUniformLocation(programID, "agentPos"), 1, &agentPos[0]);

        killButton.draw(VP);

        agentPos = glm::vec3(
            crewmate.x - powerButton.x, crewmate.y - powerButton.y, lightheight);
        glUniform3fv(glGetUniformLocation(programID, "agentPos"), 1, &agentPos[0]);

        powerButton.draw(VP);

        agentPos = glm::vec3(
            0, 0, -1);
        glUniform3fv(glGetUniformLocation(programID, "agentPos"), 1, &agentPos[0]);

        crewmate.draw(VP);

        agentPos = glm::vec3(
            crewmate.x - imposter.x, crewmate.y - imposter.y, lightheight);
        glUniform3fv(glGetUniformLocation(programID, "agentPos"), 1, &agentPos[0]);

        imposter.draw(VP);

        lightblockers.clear();
        vector<pair<int, int>> blocks = maze.getBlocks(crewmate.x, crewmate.y);

        if (!isLighted)
        {
            for (auto block : blocks)
            {
                lightblockers.push_back(LightBlocker(block.first, block.second));
            }

            for (auto blocker : lightblockers)
            {
                blocker.draw(VP);
            }
        }
        // lives, score, time

        agentPos = glm::vec3(
            crewmate.x - exitEdge.x, crewmate.y - exitEdge.y, lightheight);
        glUniform3fv(glGetUniformLocation(programID, "agentPos"), 1, &agentPos[0]);

        exitEdge.draw(VP);

        hud.draw(lives, timeInDark, score, gameTime, isLighted);
        glUseProgram(programID);
    }
    else
    {
        //render text for game over;
    }
}

int imposterCtr = 0;

void tick_input(GLFWwindow *window)
{
    // int left = glfwGetKey(window, GLFW_KEY_LEFT);
    // int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    // int up = glfwGetKey(window, GLFW_KEY_UP);
    // int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int s = glfwGetKey(window, GLFW_KEY_S);
    if (kbdLEFT)
    {
        // Do something
        crewmate.move(-1, 0, maze.adj, maze.r, maze.c);
        kbdLEFT = false;
    }
    if (kbdRIGHT)
    {
        crewmate.move(1, 0, maze.adj, maze.r, maze.c);
        kbdRIGHT = false;
    }
    if (kbdUP)
    {
        crewmate.move(0, 1, maze.adj, maze.r, maze.c);
        kbdUP = false;
    }
    if (kbdDOWN)
    {
        crewmate.move(0, -1, maze.adj, maze.r, maze.c);
        kbdDOWN = false;
    }
    imposterCtr += 1;
    if (kbdSEEK || imposterCtr == 20)
    {
        imposter.seek(crewmate.x, crewmate.y, maze.adj, maze.r, maze.c);
        kbdSEEK = false;
        imposterCtr = 0;
        if (imposter.x == crewmate.x && imposter.y == crewmate.y)
            lives -= 1;
    }
    if (kbdL)
    {
        isLighted = !isLighted;
        if (isLighted)
        {
            glUniform1f(glGetUniformLocation(programID, "lighted"), 1.0);
        }
        else
        {
            glUniform1f(glGetUniformLocation(programID, "lighted"), 0.0);
        }
        cout << "islighted" << isLighted << endl;
        kbdL = false;
    }
}

void tick_elements()
{
    // ball1.tick();
    camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    killButton = Button(2, 4, 0);
    powerButton = Button(2, 3, 1);
    ball1 = Ball(0, 0, COLOR_RED);
    crewmate = Agent(0, 0, false);
    // -----maze
    maze = Maze(9, 9);
    mazeBg = MazeBg(9, 9);
    imposter = Agent(maze.r - 1, maze.c - 1, true);
    exitEdge = ExitEdge(maze.outCoords, maze.r, maze.c);
    cout << "maze cols: " << maze.c << endl;
    cout << "maze rows: " << maze.r << endl;

    hud = HUD(0.0, 0.0, width, height);
    // auto adj = maze.adj;
    // for (int i = 0; i < maze.r; i++)
    //     for (int j = 0; j < maze.c; j++)
    //         cout << adj[i * maze.c + j].size() << endl;

    //-----------

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");

    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    // Background color of the scene
    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv)
{
    srand(time(0));
    int width = 900;
    int height = 900;

    window = initGLFW(width, height);

    initGL(window, width, height);
    // maze = Maze(4, 5);

    // cout << "maze cols: " << maze.c << endl;
    // cout << "maze rows: " << maze.r << endl;

    // // auto adj = maze.adj;
    // // for (int i = 0; i < maze.r; i++)
    // //     for (int j = 0; j < maze.c; j++)
    // //         cout << adj[i * maze.c + j].size() << endl;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers

        if (t60.processTick())
        {
            // 60 fps
            // OpenGL Draw commands

            gameLogic();

            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b)
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen()
{
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

void gameLogic()
{
    if (crewmate.x == killButton.x && crewmate.y == killButton.y && imposter.alive)
    {
        //vaporize
        imposter.kill();
        score += 40.0;
        // endGame(1);
    }

    if (crewmate.x == imposter.x && crewmate.y == imposter.y)
    {
        // crewmate.kill();
        // endGame(0);
        // lives -= 1;
    }
    if (!isLighted)
        timeInDark += 1.0 / 60.0;
    gameTime += 1.0 / 60.0;
}

void endGame(int state)
{
    // if (state == 0)
    // {
    //     glClearColor(COLOR_RED.r / 256.0, COLOR_RED.g / 256.0, COLOR_RED.b / 256.0, 0.0f); // R, G, B, A
    // }
    // else
    // {
    //     glClearColor(COLOR_BLACK.r / 256.0, COLOR_BLACK.g / 256.0, COLOR_BLACK.b / 256.0, 0.0f); // R, G, B, A
    // }
    // gameOver = true;
}