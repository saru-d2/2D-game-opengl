#include "agent.h"
#include "main.h"

using namespace std;

Agent::Agent(float x, float y, bool imposter)
{
    this -> alive = true;
    int numTriangles = 8;
    // this->position = glm::vec3(x, y, 0);
    this->x = x;
    this->y = y;
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    GLfloat vertices[][3] = {
        {0.0f, 0.5f, 0.0f},    //0
        {-0.3f, 0.4f, 0.0f},   //1
        {0.3f, 0.4f, 0.0f},    //2
        {-0.3f, 0.3f, 0.0f},   //3
        {-0.4f, 0.3f, 0.0f},   //4
        {-0.3f, -0.3f, 0.0f},  //5
        {0.3f, -0.3f, 0.0f},   //6
        {0.0f, -0.3f, 0.0f},   //7
        {0.3f, -0.5f, 0.0f},   //8
        {-0.3f, -0.5f, 0.0f},  //9
        {-0.1f, 0.2f, 0.0f},  //10
        {0.35f, 0.2f, 0.0f},  //11
        {-0.1f, -0.0f, 0.0f}, //12
        {0.35f, -0.0f, 0.0f}, //13
    };

    int triangleList[][3] = {
        {0, 1, 2},
        {1, 2, 5},
        {2, 5, 6},
        {3, 4, 5},
        {5, 7, 9},
        {6, 7, 8},
        {10, 11, 12},
        {12, 13, 11},
    };

    GLfloat vertex_buffer_data[9 * numTriangles];

    for (int i = 0; i < numTriangles; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // for each pt in each triangle
            for (int k = 0; k < 3; k++)
            {
                vertex_buffer_data[9 * i + 3 * j + k] = vertices[triangleList[i][j]][k];
            }
        }
    }

    GLfloat colorBuffer[numTriangles * 3 * 3];
    GLfloat colors[][3] = {{0.5f, 0.7f, 0.9f},
                           {0.3f, 0.6f, 0.5f}};
    if (imposter)
    {
        colors[1][0] = 0.9f;
        colors[1][1] = 0.2f;
        colors[1][2] = 0.2f;
    }
    int colorMap[] = {1, 1, 1, 1, 1, 1, 0, 0};

    for (int i = 0; i < numTriangles; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            //each point
            colorBuffer[9 * i + 3 * j + 0] = colors[colorMap[i]][0];
            colorBuffer[9 * i + 3 * j + 1] = colors[colorMap[i]][1];
            colorBuffer[9 * i + 3 * j + 2] = colors[colorMap[i]][2];
        }
    }

    this->object = create3DObject(GL_TRIANGLES, numTriangles * 3, vertex_buffer_data, colorBuffer, GL_FILL);
}

void Agent::draw(glm::mat4 VP)
{
    if (this->alive == false) return;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(glm::vec3(this->x, this->y, 0)); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Agent::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Agent::tick()
{
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Agent::move(int toX, int toY, vector<vector<int>> adj, int r, int c)
{
    //check if edge
    if (this->alive == false) return;

    toX += this->x;
    toY += this->y;
    if (toX < 0 || toX >= r || toY >= c || toY < 0)
        return;
    bool moveFlag = false;
    for (auto i : adj[this->x * c + this->y])
    {
        if (i == toX * c + toY)
            moveFlag = true;
    }
    for (auto i : adj[toX * c + toY])
    {
        if (i == this->x * c + this->y)
            moveFlag = true;
    }

    if (moveFlag)
    {
        this->x = toX;
        this->y = toY;
    }

    return;
}

void Agent::seek(int seekX, int seekY, vector<vector<int>> adj, int r, int c)
{
    // here we will dijkstra!!
    // screw it bfs for now
    if (this->alive == false) return;

    list<int> q;
    vector<int> dis(c * r, INT32_MAX);
    vector<int> path;
    vector<bool> vis(r * c, false);
    vector<int> pred(c * r, -1);

    dis[this->x * c + this->y] = 0;
    vis[this->x * c + this->y] = true;
    q.push_back(this->x * c + this->y);

    while (!q.empty())
    {
        int fr = q.front();
        q.pop_front();
        bool found = false;
        for (auto i : adj[fr])
        {
            if (!vis[i])
            {
                vis[i] = true;
                dis[i] = dis[fr] + 1;
                pred[i] = fr;
                q.push_back(i);

                if (i == seekX * c + seekY)
                {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }

    int crawl = seekX * c + seekY;
    path.push_back(crawl);

    while(pred[crawl]!= -1){
        path.push_back(pred[crawl]);
        crawl = pred[crawl];
    }

    // for (auto i: path){
    //     cout<<i<<" ";
    // }
    // cout<<endl;

    if (path.size() > 1)
    {
        this->x = path[path.size() - 2] / c;
        this->y = path[path.size() - 2] % c;
    }
}

void Agent::kill() {
    this->x = -1;
    this->y = -1;
    this->alive = false;
}