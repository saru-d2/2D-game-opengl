#include "main.h"
#include "maze.h"
#include <set>

using namespace std;

int myrand(int i) { return rand() % i; }

void fun(vector<int> adj[], int &r, int &c, vector<vector<int>> &vis, int x, int y)
{
    vis[x][y] = true;
    pair<int, int> choices[] = {
        {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};

    // pick a random order if these
    vector<int> order = {0, 1, 2, 3};
    random_shuffle(order.begin(), order.end(), myrand);

    // for (auto i : order)
    // {
    //     cout << i << " ";
    // }
    // cout << endl;
    for (auto i : order)
    {
        int x1 = choices[i].first;
        int y1 = choices[i].second;
        if (x1 >= r || x1 < 0 || y1 < 0 || y1 >= c)
            continue;
        if (vis[x1][y1])
            continue;
        adj[c * x + y].push_back(c * x1 + y1);
        adj[c * x1 + y1].push_back(c * x + y);
        // cout << x << "," << y << " " << x1 << "," << y1 << endl;
        fun(adj, r, c, vis, x1, y1);
    }
}

void makeMaze(vector<int> adj[], int &r, int &c)
{
    // code for random maze generation
    // so uses prims... sorta :P
    // prims recur like this https://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm
    vector<vector<int>> vis(r, vector<int>(c, 0));
    fun(adj, r, c, vis, 0, 0);
}

bool checkEdge(std::vector<std::vector<int>> &adj, pair<int, int> a, pair<int, int> b, int r, int c, vector<vector<bool>> &map)
{
    int i1 = a.first;
    int j1 = a.second;
    int i2 = b.first;
    int j2 = b.second;

    if (i1 < 0 || i1 >= r ||
        i2 < 0 || i2 >= r ||
        j2 < 0 || j2 >= c ||
        j1 < 0 || j1 >= c)
        return false;

    // cout << "{ {" << i1 * c + j1 << "},{" << i2 * c + j2 << "} }" << endl;
    for (auto k : adj[i1 * c + j1])
    {
        if (k == c * i2 + j2)
            return false;
    }
    //redundant but whatever
    for (auto k : adj[i2 * c + j2])
    {
        if (k == c * i1 + j1)
            return false;
    }

    if (map[i1 * c + j1][i2 * c + j2] || map[i2 * c + j2][i1 * c + j1])
        return false;
    map[i1 * c + j1][i2 * c + j2] = true;
    map[i2 * c + j2][i1 * c + j1] = true;
    // cout << '\t' << "[ {" << i1 * c + j1 << "},{" << i2 * c + j2 << "} ]" << endl;
    return true;
}

void printAdj(vector<vector<int>> &adj)
{
    for (int i = 0; i < adj.size(); i++)
    {
        cout << i << ": ";
        for (auto j : adj[i])
        {
            cout << j << " ";
        }
        cout << endl;
    }
}

Maze::Maze(int r, int c)
{
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;
    this->r = r;
    this->c = c;
    cout << this->r << ' ' << this->c << endl;
    vector<int> adj[this->r * this->c];
    makeMaze(adj, r, c);
    // cout << adj[0][0] << endl;
    // this->adj = adj;
    this->adj = vector<vector<int>>(adj, adj + r * c);
    int outR = 0, outC = 0;
    // if (rand() & 1)
    //     outC = this->c;
    outR = (rand() % (this->r - 2)) + 1;
    if (rand() & 1)
        swap(outR, outC);
    this->outCoords = {outR, outC};

    //plotting maze vertices
    std::vector<std::pair<pair<float, float>, pair<float, float>>> vertex_buffer_vector;
    vertex_buffer_vector.clear();
    GLfloat edgeLength = 1.0f;
    // the lines on the inside

    printAdj(this->adj);

    vector<vector<bool>> map(this->r * this->c, vector<bool>(this->r * this->c, false));

    for (int i = 0; i < this->r; i++)
        for (int j = 0; j < this->c; j++)
        {
            // 4 cases
            if (checkEdge(this->adj, {i, j}, {i - 1, j}, r, c, map))
                vertex_buffer_vector.push_back({{i - edgeLength / 2.0, j - edgeLength / 2.0}, {i - edgeLength / 2.0, j + edgeLength / 2.0}});

            if (checkEdge(this->adj, {i, j}, {i + 1, j}, r, c, map))
                vertex_buffer_vector.push_back({{i + edgeLength / 2.0, j - edgeLength / 2.0}, {i + edgeLength / 2.0, j + edgeLength / 2.0}});

            if (checkEdge(this->adj, {i, j}, {i, j - 1}, r, c, map))
                vertex_buffer_vector.push_back({{i - edgeLength / 2.0, j - edgeLength / 2.0}, {i + edgeLength / 2.0, j - edgeLength / 2.0}});

            if (checkEdge(this->adj, {i, j}, {i, j + 1}, r, c, map))
                vertex_buffer_vector.push_back({{i - edgeLength / 2.0, j + edgeLength / 2.0}, {i + edgeLength / 2.0, j + edgeLength / 2.0}});
        }

    vertex_buffer_vector.push_back({{-0.5, -0.5}, {this->r - 0.5, -0.5}});
    vertex_buffer_vector.push_back({{-0.5, -0.5}, {-0.5, this->c - 0.5}});
    vertex_buffer_vector.push_back({{this->r - 0.5, -0.5}, {this->r - 0.5, this->c - 0.5}});
    vertex_buffer_vector.push_back({{-0.5, this->c - 0.5}, {this->r - 0.5, this->c - 0.5}});

    GLfloat vertex_buffer_data[vertex_buffer_vector.size() * 6];
    // cout << "length: " << vertex_buffer_vector.size() << endl;
    int ind = 0;
    for (auto iter : vertex_buffer_vector)
    {
        vertex_buffer_data[ind] = iter.first.first;
        vertex_buffer_data[ind + 1] = iter.first.second;
        vertex_buffer_data[ind + 2] = 0.0f;
        vertex_buffer_data[ind + 3] = iter.second.first;
        vertex_buffer_data[ind + 4] = iter.second.second;
        vertex_buffer_data[ind + 5] = 0.0f;

        // cout << vertex_buffer_data[ind] << " " << vertex_buffer_data[ind + 1] << " " << vertex_buffer_data[ind + 2] << " " << vertex_buffer_data[ind + 3] << " " << vertex_buffer_data[ind + 4] << " " << vertex_buffer_data[ind + 5] << endl;
        ind += 6;
    }

    // cout << "vertex data: " << endl;
    // for (int i = 0; i < vertex_buffer_vector.size() * 6; i++)
    // {
    //     cout << vertex_buffer_data[i] << " ";
    //     if (i % 3 == 2)
    //         cout << endl;
    // }
    // cout << endl;

    this->object = create3DObject(GL_LINES, vertex_buffer_vector.size() * 2, vertex_buffer_data, COLOR_BLACK, GL_FILL);
}

void Maze::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

vector<pair<int, int>> Maze::getBlocks(int x, int y)
{
    int depth = 2;
    vector<pair<int, int>> ret;

    queue<pair<int, int>> q;
    q.push({x * this->c + y, 0});
    set<int> s;
    vector<bool> vis(this->r * this->c, 0);

    while (!q.empty())
    {
        auto fr = q.front();
        vis[fr.first] = true;
        // cout<<  <<endl;
        q.pop();
        if (fr.second > depth)
            break;
        for (auto i : this->adj[fr.first])
        {
            if (!vis[i])
                q.push({i, fr.second + 1});
        }
        // ret.push_back({fr.first / this->c, fr.first % this->c});
        s.insert(fr.first);
    }

    for (int i = 0; i < this->r * this->c; i++)
    {
        if (s.count(i) == 0)
        {
            int tx = i / this->c, ty = i % this->c;
            if (abs(tx - x) < 3 && abs(ty - y) < 3)
                ret.push_back({tx, ty});
        }
    }

    return ret;
}
