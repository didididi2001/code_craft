#include <queue>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_map>

#include "path.h"

auto CalculateDistance(Point a, Point b) -> int
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// 计算从起始位置到最近的泊位的最短路径方向
auto InitWith1(const MapArray &map) -> DirectionsArray
{
    DirectionsArray directions;
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            directions[i][j] = {-1, -1, 1000};
        }
    }
    // 定义四个方向
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    // BFS搜索
    std::queue<Point> q;
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            if (map[i][j].type == DOCK)
            {
                q.push({i, j});
                directions[i][j] = {0, 0, 0};
            }
        }
    }
    int deepth = 1;
    while (!q.empty())
    {

        for (int i = 0; i < q.size(); i++)
        {
            Point curr = q.front();
            q.pop();

            for (int k = 0; k < 4; ++k)
            {
                int nx = curr.x + dx[k];
                int ny = curr.y + dy[k];

                if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && check_is_arrive_robot(map, nx, ny) &&
                    directions[nx][ny].x == -1 && directions[nx][ny].y == -1 && directions[nx][ny].distance == 1000)
                {
                    q.push({nx, ny});
                    directions[nx][ny] = {-dx[k], -dy[k], deepth};
                }
            }
        }
        deepth++;
    }

    return directions;
}

auto InitWith10(const MapArray &map, const std::vector<Point> berth_vertex, std::array<std::array<int, MAP_SIZE>, MAP_SIZE> &nearest_berth_index, std::array<std::array<int, MAP_SIZE>, MAP_SIZE> &is_reach) -> DirectionsArrays
{

    std::cout << "dadsd" << std::endl;
    std::array<std::array<int, MAP_SIZE>, MAP_SIZE> nearest_berth_distance;
    DirectionsArrays directions_distance;
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            nearest_berth_index[i][j] = -1;
            nearest_berth_distance[i][j] = 1000;
            is_reach[i][j] = 0;
            for (int k = 0; k < berth_vertex.size(); ++k)
            {
                directions_distance[i][j][k] = {-1, -1, 1000};
            }
        }
    }
    // 定义四个方向
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    for (int l = 0; l < berth_vertex.size(); l++)
    {
        std::queue<Point> q;
        // 遍历每一个corner

        auto vertex = berth_vertex[l];
        for (int i = -4; i < 5; i++)
        {
            for (int j = -4; j < 5; j++)
            {
                int x = vertex.x + i;
                int y = vertex.y + j;

                if (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE && map[x][y].type == DOCK && HasLandAround(map, x, y))
                {
                    q.push({x, y});
                    directions_distance[x][y][l] = {0, 0, 0};
                    nearest_berth_distance[x][y] = l;
                    is_reach[x][y] = 1;
                }
            }
        }
        std::cerr << q.size() << std::endl;
        int deepth = 1;
        while (!q.empty())
        {
            int layer_size = q.size();
            // std::cerr << " layer_size" << layer_size << std::endl;
            // std::cerr << " deepth" << deepth << std::endl;
            for (int j = 0; j < layer_size; j++)
            {
                Point curr = q.front();
                q.pop();
                for (int k = 0; k < 4; ++k)
                {
                    int nx = curr.x + dx[k];
                    int ny = curr.y + dy[k];

                    if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && check_is_arrive_robot(map, nx, ny) &&
                        directions_distance[nx][ny][l].x == -1 && directions_distance[nx][ny][l].y == -1 && directions_distance[nx][ny][l].distance == 1000)
                    {
                        q.push({nx, ny});
                        directions_distance[nx][ny][l] = {-dx[k], -dy[k], deepth};
                        is_reach[nx][ny] = 1;
                        if (nearest_berth_index[nx][ny] == -1 || nearest_berth_distance[nx][ny] > deepth)
                        {
                            nearest_berth_index[nx][ny] = l;
                            nearest_berth_distance[nx][ny] = deepth;
                        }
                    }
                }
            }
            deepth++;
        }
    }
    // std::cerr << "adas\n";
    return directions_distance;
}
// auto AStar(const MapArray &map, Point start, Point goal) -> std::vector<Point>
// {
//     std::vector<Point> path;

//     // Define possible movement directions (up, down, left, right)
//     const int dx[] = {0, 0, -1, 1};
//     const int dy[] = {-1, 1, 0, 0};

//     // Initialize open and closed lists
//     std::vector<std::vector<bool>> closed(map.size(), std::vector<bool>(map[0].size(), false));
//     std::priority_queue<Node *, std::vector<Node *>, decltype(&Node::Compare)> open(&Node::Compare);

//     // Create start node
//     Node *startNode = new Node{start, 0, CalculateDistance(start, goal), 0, nullptr};
//     open.push(startNode);

//     while (!open.empty())
//     {
//         // Get the node with the lowest f value from the open list
//         Node *current = open.top();
//         open.pop();

//         // Check if the current node is the goal
//         if (current->position.x == goal.x && current->position.y == goal.y)
//         {
//             // Construct path by tracing back from goal to start
//             while (current != nullptr)
//             {
//                 /*path.push_back(current->position);*/
//                 auto p = current->parent;
//                 if (p != nullptr)
//                 {
//                     path.push_back({current->position.x - p->position.x, current->position.y - p->position.y});
//                 }
//                 current = p;
//             }
//             // Reverse the path to get the correct order
//             reverse(path.begin(), path.end());
//             break;
//         }

//         // Mark the current node as closed
//         closed[current->position.x][current->position.y] = true;

//         // Expand the current node's neighbors
//         for (int i = 0; i < 4; ++i)
//         {
//             int next_x = current->position.x + dx[i];
//             int next_y = current->position.y + dy[i];

//             // Check if the neighbor is within the grid and not an obstacle
//             if (next_x >= 0 && next_x < map.size() && next_y >= 0 && next_y < map[0].size() && map[next_x][next_y].type != OBSTACLE && map[next_x][next_y].type != SEA && !closed[next_x][next_y])
//             {
//                 int newG = current->g + 1; // Assuming each step has a cost of 1
//                 int newH = CalculateDistance({next_x, next_y}, goal);
//                 int newF = newG + newH;

//                 Node *successor = new Node{{next_x, next_y}, newG, newH, newF, current};
//                 open.push(successor);
//             }
//         }
//     }

//     // Free memory allocated for nodes
//     while (!open.empty())
//     {
//         delete open.top();
//         open.pop();
//     }

//     return path;
// }

auto Move(Point direction) -> int
{
    if (direction.x == 0 && direction.y == 1)
    {
        // std::cerr << "move:" << 0 << std::endl;
        return 0;
    }
    if (direction.x == 0 && direction.y == -1)
    {
        // std::cerr << "move:" << 1 << std::endl;
        return 1;
    }
    if (direction.x == -1 && direction.y == 0)
    {
        // std::cerr << "move:" << 2 << std::endl;
        return 2;
    }
    if (direction.x == 1 && direction.y == 0)
    {
        // std::cerr << "move:" << 3 << std::endl;
        return 3;
    }
    // std::cerr << "move:" << -1 << std::endl;
    return -1;
}

auto bfs(MapArray &map, Point pstart, Point &ptarget) -> std::vector<Point>
{
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    std::vector<Point> path;
    std::vector<std::vector<bool>> visited(200, std::vector<bool>(200, false));
    std::queue<Node *> q;
    Node *start = new Node{pstart, nullptr};
    q.push(start);
    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();

        // 如果当前位置是目标位置，返回路径
        if (map[curr->position.x][curr->position.y].is_good)
        {
            ptarget.x = curr->position.x;
            ptarget.y = curr->position.y;
            while (curr != nullptr)
            {
                /*path.push_back(current->position);*/
                auto p = curr->parent;
                if (p != nullptr)
                {
                    path.push_back({curr->position.x - p->position.x, curr->position.y - p->position.y});
                }
                curr = p;
            }
            // Reverse the path to get the correct order
            reverse(path.begin(), path.end());
            break;
        }

        // 检查当前位置的相邻位置
        for (int i = 0; i < 4; ++i)
        {
            int newX = curr->position.x + dx[i];
            int newY = curr->position.y + dy[i];

            // 检查相邻位置是否在边界内，并且未被访问过，并且不是障碍物
            if (newX >= 0 && newX < 200 && newY >= 0 && newY < 200 && !visited[newX][newY] && map[newX][newY].type != OBSTACLE && map[newX][newY].type != SEA)
            {
                // 将相邻位置加入队列，并更新路径
                visited[newX][newY] = true;
                Point nNode{newX, newY};
                Node *next = new Node{nNode, curr};
                q.push(next);
            }
        }
    }
    while (!q.empty())
    {
        delete q.front();
        q.pop();
    }
    // 如果没有找到路径，返回空路径
    return path;
}

auto BfsFindNNode(MapArray &map, GoodPool &goodpool, DirectionsArray &direction, int id, int target_nums, Point pstart, Point &ptarget, bool largest_val, int max_path_lenth) -> std::vector<Point>
{
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    std::vector<Point> path;
    std::vector<Node *> targetnodes;
    std::vector<int> lenth;
    int cur_num = 0;
    std::vector<std::vector<bool>> visited(200, std::vector<bool>(200, false));
    std::queue<Node *> q;
    Node *start = new Node{pstart, nullptr};
    q.push(start);
    int deepth = 1;
    // std::cerr << "target_points: " << std::endl;
    while (!q.empty())
    {
        int layer_size = q.size();
        for (int i = 0; i < layer_size; i++)
        {
            auto curr = q.front();
            q.pop();
            int x = curr->position.x;
            int y = curr->position.y;

            if (map[x][y].is_good && cur_num < target_nums && goodpool.Search(x, y) != nullptr && 1000 - goodpool.Search(x, y)->GetExistenceTime(id) > deepth)
            {

                targetnodes.push_back(curr);
                lenth.push_back(deepth);

                // std::cerr << x << " " << y << std::endl;
                cur_num++;
                // std::cerr << "cur_num:" << cur_num << std::endl;
                if (cur_num == target_nums)
                {
                    break;
                }
            }
            // 检查当前位置的相邻位置
            for (int i = 0; i < 4; ++i)
            {
                int newX = curr->position.x + dx[i];
                int newY = curr->position.y + dy[i];

                // 检查相邻位置是否在边界内，并且未被访问过，并且不是障碍物
                if (newX >= 0 && newX < 200 && newY >= 0 && newY < 200 && !visited[newX][newY] && check_is_arrive_robot(map, newX, newY))
                {
                    // 将相邻位置加入队列，并更新路径
                    visited[newX][newY] = true;
                    Point nNode{newX, newY};
                    Node *next = new Node{nNode, curr};
                    q.push(next);
                }
            }
        }
        deepth++;
        // std::cerr << "deepth:" << deepth << std::endl;
        if (deepth > max_path_lenth)
        {
            break;
        }
    }
    int max_index = -1;
    double max_rate = -1;
    for (int i = 0; i < targetnodes.size(); i++)
    {
        int x = targetnodes[i]->position.x;
        int y = targetnodes[i]->position.y;
        double rate = 0.0;
        // double rate = 1.0 * goodpool.Search(x, y)->GetVal() / (lenth[i] + direction[x][y].distance);
        if (!largest_val)
        {
            rate = 1.0 * goodpool.Search(x, y)->GetVal() / (lenth[i]);
        }
        else
        {
            rate = 1.0 * goodpool.Search(x, y)->GetVal();
        }

        if (rate > max_rate || max_index == -1)
        {
            max_rate = rate;
            max_index = i;
        }
    }
    if (max_index != -1)
    {
        ptarget = targetnodes[max_index]->position;
        // std::cerr << "最大比率的货物：" << ptarget.x << " " << ptarget.y << std::endl;
        auto cur = targetnodes[max_index];
        while (cur != nullptr)
        {
            /*path.push_back(current->position);*/
            auto p = cur->parent;
            if (p != nullptr)
            {
                path.push_back({cur->position.x - p->position.x, cur->position.y - p->position.y});
            }
            cur = p;
        }
        // Reverse the path to get the correct order
        reverse(path.begin(), path.end());
        targetnodes.clear();
        while (!q.empty())
        {
            delete q.front();
            q.pop();
        }
    }
    else
    {
        // std::cerr << "没找到货物" << std::endl;
    }

    // 如果没有找到路径，返回空路径
    return path;
}

auto FindNextDirection(const MapArray &map, const Point cur_point, const Point origin_direction) -> Point
{
    // 定义方向数组
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    // 初始化新方向为原始方向
    Point new_direction = origin_direction;

    // 计算下一个点的坐标
    int next_x = cur_point.x + new_direction.x;
    int next_y = cur_point.y + new_direction.y;

    // 找到原始方向在方向数组中的索引
    int origin_index = directionToIndex[std::to_string(new_direction.x) + std::to_string(new_direction.y)];
    std::array<int, 4> index_array = {0, 1, 2, 3};
    // 初始化索引为原始方向的索引
    std::random_device rd;
    std::mt19937 g(rd());

    // 使用shuffle函数打乱数组
    std::shuffle(index_array.begin(), index_array.end(), g);
    int index = -1;
    for (int i = 0; i < 4; i++)
    {
        if (index_array[i] == origin_index)
        {
            index = i;
            break;
        }
    }
    origin_index = index;
    // 循环直到满足条件
    while (map[next_x][next_y].is_robot || !check_is_arrive_robot(map, next_x, next_y))
    {
        // 选择下一个方向
        index = (index + 1) % 4;
        // 如果回到了原始方向，则停止循环
        if (index == origin_index)
        {

            std::cerr << "x: " << dx[origin_index] << "  y: " << dy[origin_index] << " 四个方向都不行" << std::endl;
            return Point{0, 0};
            break;
        }

        // 更新新方向
        new_direction.x = dx[index_array[index]];
        new_direction.y = dy[index_array[index]];

        // 更新下一个点的坐标
        next_x = cur_point.x + new_direction.x;
        next_y = cur_point.y + new_direction.y;
        // std::cerr << next_x << " " << next_y << " " << map[next_x][next_y].is_robot << " " << map[next_x][next_y].type << std::endl;
    }

    // 返回新方向
    return {new_direction.x, new_direction.y};
}

auto BFSWithTarget(MapArray &map, Point pstart, const Point &ptarget) -> std::vector<Point>
{
    std::vector<Point> path;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    std::vector<std::vector<bool>> visited(200, std::vector<bool>(200, false));
    std::queue<Node *> q;
    Node *start = new Node{pstart, nullptr};
    q.push(start);
    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();

        // 如果当前位置是目标位置，返回路径
        if (ptarget == curr->position)
        {

            while (curr != nullptr)
            {
                /*path.push_back(current->position);*/
                auto p = curr->parent;
                if (p != nullptr)
                {
                    path.push_back({curr->position.x - p->position.x, curr->position.y - p->position.y});
                }
                curr = p;
            }
            // Reverse the path to get the correct order
            reverse(path.begin(), path.end());
            break;
        }

        // 检查当前位置的相邻位置
        for (int i = 0; i < 4; ++i)
        {
            int newX = curr->position.x + dx[i];
            int newY = curr->position.y + dy[i];

            // 检查相邻位置是否在边界内，并且未被访问过，并且不是障碍物
            if (newX >= 0 && newX < 200 && newY >= 0 && newY < 200 && !visited[newX][newY] && map[newX][newY].type != OBSTACLE && map[newX][newY].type != SEA)
            {
                // 将相邻位置加入队列，并更新路径
                visited[newX][newY] = true;
                Point nNode{newX, newY};
                Node *next = new Node{nNode, curr};
                q.push(next);
            }
        }
    }
    while (!q.empty())
    {
        delete q.front();
        q.pop();
    }
    // 如果没有找到路径，返回空路径
    return path;
}
auto FindNearestBerth(std::array<PointDistance, max_berth_num> &distance, std::vector<int> &existing_berth) -> int
{
    int min_index = -1;
    int min_distance = -1;
    for (int i = 0; i < existing_berth.size(); i++)
    {
        if ((distance[i].distance < min_distance || min_index == -1) && existing_berth[i])
        {
            min_distance = distance[i].distance;
            min_index = i;
        }
    }
    return min_index;
}

auto FindNearestBerth_index(MapArray &map, DirectionsArrays &distances, int berth_num) -> std::array<std::array<int, MAP_SIZE>, MAP_SIZE>
{
    std::array<std::array<int, MAP_SIZE>, MAP_SIZE> nearest_berth_index;
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (map[i][j].type == EMPTY_LAND)
            {
                int min_index = -1;
                int min_ = 100000;
                for (int k = 0; k < berth_num; k++)
                {
                    if (k == 0 || distances[i][j][k].distance < min_)
                    {
                        min_ = distances[i][j][k].distance;
                        min_index = k;
                    }
                }
                nearest_berth_index[i][j] = min_index;
            }
            else
            {
                nearest_berth_index[i][j] = -1;
            }
        }
    }
    return nearest_berth_index;
}

auto find_berth_to_berth_path(DirectionsArrays &distances, std::vector<Point> &berth_vertex) -> std::vector<std::vector<std::vector<Point>>>
{
    int n = berth_vertex.size();
    // paths[i][j]代表从i港口到j港口的路径
    std::vector<std::vector<std::vector<Point>>> paths(n, std::vector<std::vector<Point>>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            std::vector<Point> path;
            std::vector<Point> repath;
            auto begin = berth_vertex[i];
            auto end = berth_vertex[j];
            while (!(begin == end))
            {
                auto dirction_x = distances[begin.x][begin.y][j].x;
                auto dirction_y = distances[begin.x][begin.y][j].y;
                path.push_back({dirction_x, dirction_y});
                repath.push_back({-dirction_x, -dirction_y});
                begin.x += dirction_x;
                begin.y += dirction_y;
            }
            paths[i][j] = path;
            reverse(repath.begin(), repath.end());
            paths[j][i] = repath;
        }
    }
    return paths;
}

std::vector<int> init_berth_nearest_T_index(MapArray &map, std::vector<Point> &T, const std::vector<Point> &berth_vertex)
{
    
    int berth_size = berth_vertex.size();
    std::vector<int> berth_to_nearest_T_index(berth_size, -1);
    std::vector<int> berth_to_nearest_T_distance(berth_size, 1000);
    std::unordered_map<int, int> berth_vertex_to_index;
    for(int i = 0; i < berth_size;i++){
        int key = berth_vertex[i].x * MAP_SIZE + berth_vertex[i].y;
        berth_vertex_to_index[key] = i;
    }
    std::cerr << "T.size()" << T.size() << "\n";
    for (int t = 0; t < T.size(); t++)
    {
        std::cerr << "t:" << t << "\n"; 
        int T_x = T[t].x;
        int T_y = T[t].y;
        std::array<std::array<std::array<int, 4>, MAP_SIZE>, MAP_SIZE> visited;
        for (int i = 0; i < MAP_SIZE; ++i)
        {
            for (int j = 0; j < MAP_SIZE; ++j)
            {
                for (int k = 0; k < 4; ++k)
                {
                    visited[i][j][k] = 0;
                }
            }
        }
        std::queue<path_node> q;
        for (int i = 0; i < 4; i++)
        {
            visited[T_x][T_y][i] = 1;
            q.push({T_x, T_y, i});
        }
        int depth = 1;
        while (!q.empty())
        {
            int size = q.size();
            for (int i = 0; i < size; ++i)
            {
                path_node cur = q.front();
                q.pop();
                int x = cur.x;
                int y = cur.y;
                int direction = cur.direction;
                // direction: 0 right, 1 left, 2 up, 3 down
                int pre_direction = -1;
                for (int j = 0; j < 3; ++j)
                {
                    Point pre_core = get_previous_core({x, y}, direction, j, pre_direction);
                    if (pre_core.x >= 0 && pre_core.x < MAP_SIZE && pre_core.y >= 0 && pre_core.y < MAP_SIZE && visited[pre_core.x][pre_core.y][pre_direction] == 0)
                    {
                        if (check_boat_arrive(map, pre_core, pre_direction))
                        {
                            q.push(path_node{pre_core.x, pre_core.y, pre_direction});
                            int hash = pre_core.x * MAP_SIZE + pre_core.y;
                            if(berth_vertex_to_index.find(hash) != berth_vertex_to_index.end()){
                                int index = berth_vertex_to_index[hash];
                                if(berth_to_nearest_T_index[index] == -1 || berth_to_nearest_T_distance[index] > depth){
                                    std::cerr << "depth" << depth << "\n";
                                    std::cerr << "berth_to_nearest_T_index[index]:" << berth_to_nearest_T_index[index] << "\n";
                                    berth_to_nearest_T_index[index] = t;
                                    berth_to_nearest_T_distance[index] = depth;
                                }
                            }
                            visited[pre_core.x][pre_core.y][pre_direction] = 1;
                        }
                    }
                }
            }
            depth++;
        }
    }
    return berth_to_nearest_T_index;
}
auto BFS_berth_to_nearest_T(MapArray &map) -> std::array<std::array<std::array<int, 4>, MAP_SIZE>, MAP_SIZE>
{

    // std::cout << "dasda" << std::endl;
    std::array<std::array<std::array<int, 4>, MAP_SIZE>, MAP_SIZE> berth_to_nearst_T;
    // 初始化所有元素为-1
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                berth_to_nearst_T[i][j][k] = -1;
            }
        }
    }
    // 4代表到达
    std::queue<path_node> q;
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            if (map[i][j].type == DELIVERY_POINT)
            {
                q.push(path_node{i, j, 0});
                q.push(path_node{i, j, 1});
                q.push(path_node{i, j, 2});
                q.push(path_node{i, j, 3});
                berth_to_nearst_T[i][j][0] = 4;
                berth_to_nearst_T[i][j][1] = 4;
                berth_to_nearst_T[i][j][2] = 4;
                berth_to_nearst_T[i][j][3] = 4;
            }
        }
    }
    int depth = 1;
    while (!q.empty())
    {
        int size = q.size();
        for (int i = 0; i < size; ++i)
        {
            path_node cur = q.front();
            q.pop();
            int x = cur.x;
            int y = cur.y;
            int direction = cur.direction;
            // direction: 0 right, 1 left, 2 up, 3 down
            int pre_direction = -1;
            for (int j = 0; j < 3; ++j)
            {
                Point pre_core = get_previous_core({x, y}, direction, j, pre_direction);
                if (pre_core.x >= 0 && pre_core.x < MAP_SIZE && pre_core.y >= 0 && pre_core.y < MAP_SIZE && berth_to_nearst_T[pre_core.x][pre_core.y][pre_direction] == -1)
                {
                    if (check_boat_arrive(map, pre_core, pre_direction))
                    {
                        q.push(path_node{pre_core.x, pre_core.y, pre_direction});
                        berth_to_nearst_T[pre_core.x][pre_core.y][pre_direction] = j;
                    }
                }
            }
        }
    }
    return berth_to_nearst_T;
}
float heuristic(const Point a, const Point b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

float heuristic(const Astar_Node &a, const Astar_Node &b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
int hash(Point cur, int curr_direction)
{
    return cur.x * MAP_SIZE * 4 + cur.y * 4 + curr_direction;
}

auto BFS_T_to_berth(MapArray &map, Point pstart, int start_direction, Point ptarget) -> std::vector<int>
{

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (map[i][j].is_boat)
            {
                std::cerr << "x: " << i << "y:" << j << "\n";
            }
        }
    }
    std::array<std::array<std::array<int, 4>, MAP_SIZE>, MAP_SIZE> visited;
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        for (int j = 0; j < MAP_SIZE; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                visited[i][j][k] = 0;
            }
        }
    }
    std::queue<path_node_with_parent *> q;
    path_node_with_parent *start = new path_node_with_parent{pstart.x, pstart.y, 0, start_direction, nullptr};
    visited[start->x][start->y][start_direction] = 1;
    q.push(start);
    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();
        if (curr->x == ptarget.x && curr->y == ptarget.y)
        {
            std::cerr << "target found" << std::endl;
            auto next = curr;
            std::vector<int> result;
            while (next->parent != nullptr)
            {
                result.push_back(next->pre_to_cur_opration);
                next = next->parent;
            }
            reverse(result.begin(), result.end());
            while (!q.empty())
            {
                delete q.front();
                q.pop();
            }
            return result;
        }
        // visited[curr->x][curr->y][curr->current_direction] = 1;
        for (int i = 0; i < 3; i++)
        {
            int next_direction = -1;
            auto next_core = get_next_core({curr->x, curr->y}, curr->current_direction, i, next_direction);
            if (next_core.x == ptarget.x && next_core.y == ptarget.y)
            {
                std::cerr << "target found" << check_boat_coexistence(map, next_core, next_direction) << "  " << check_boat_arrive(map, next_core, next_direction) << std::endl;
            }
            if (next_core.x >= 0 && next_core.x < MAP_SIZE && next_core.y >= 0 && next_core.y < MAP_SIZE && visited[next_core.x][next_core.y][next_direction] == 0 && check_boat_coexistence(map, next_core, next_direction) && check_boat_arrive(map, next_core, next_direction))
            {
                visited[next_core.x][next_core.y][next_direction] = 1;
                path_node_with_parent *next = new path_node_with_parent{next_core.x, next_core.y, i, next_direction, curr};
                q.push(next);
            }
        }
    }
    while (!q.empty())
    {
        delete q.front();
        q.pop();
    }
    return {};
}

float get_map_speed(MapArray &map, Point pt, int dir)
{

    auto boats = get_occupied_point(pt, dir);
    for (auto &p : boats)
    {
        if (map[p.x][p.y].type == TRANSPORT_BLOCK_ROUTE || map[p.x][p.y].type == SEA_ROUTE || map[p.x][p.y].type == SHIP_START || map[p.x][p.y].type == DOCK || map[p.x][p.y].type == DOCK_AREA)
        {
            return 1.0;
        }
    }

    return 2.0;
}

auto Astar_T_to_berth(MapArray &map, Point pstart, int start_direction, Point ptarget) -> std::vector<int>
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (map[i][j].is_boat)
            {
                std::cerr << "x: " << i << "y:" << j << "\n";
            }
        }
    }
    std::priority_queue<Astar_Node *, std::vector<Astar_Node *>, NodeCompare> openSet;
    std::unordered_map<int, std::nullptr_t> closed;
    std::unordered_map<int, float> valuef;

    Astar_Node *start = new Astar_Node(pstart.x, pstart.y);
    Astar_Node *target = new Astar_Node(ptarget.x, ptarget.y);
    start->g = 0;
    start->h = heuristic(*start, *target);
    start->f = start->h;
    start->current_direction = start_direction;
    start->pre_to_cur_opration = 0;
    valuef[start->hash()] = start->f;
    openSet.push(start);
    closed[start->hash()] = nullptr;
    while (!openSet.empty())
    {
        Astar_Node *current = openSet.top(); // 取出f值最小的节点
        openSet.pop();
        if (current->x == ptarget.x && current->y == ptarget.y)
        {
            auto next = current;
            std::cerr << "target found" << std::endl;
            std::vector<int> result;
            while (next->parent != nullptr)
            {
                result.push_back(next->pre_to_cur_opration);
                next = next->parent;
            }
            reverse(result.begin(), result.end());
            delete target;
            while (!openSet.empty())
            {
                delete openSet.top();
                openSet.pop();
            }
            return result;
        }
        for (int i = 0; i < 3; i++)
        {
            int next_direction = -1;
            auto next_core = get_next_core({current->x, current->y}, current->current_direction, i, next_direction);
            if (next_core.x >= 0 && next_core.x < MAP_SIZE && next_core.y >= 0 && next_core.y < MAP_SIZE && closed.find(hash(next_core, next_direction)) == closed.end())
            {
                closed[hash(next_core, next_direction)] = nullptr;
                if (check_boat_arrive(map, next_core, next_direction) && check_boat_coexistence(map, next_core, next_direction))
                {
                    Astar_Node *next = new Astar_Node(next_core.x, next_core.y);
                    next->g = current->g + 10.0f / get_map_speed(map, {current->x, current->y}, current->current_direction);
                    next->h = heuristic(*next, *target);
                    next->f = next->g + next->h;
                    next->pre_to_cur_opration = i;
                    next->current_direction = next_direction;
                    next->parent = current;
                    if (valuef.find(next->hash()) == valuef.end() || valuef[next->hash()] > next->f)
                    {
                        valuef[next->hash()] = next->f;
                        openSet.push(next);
                    }
                }
            }
        }
    }
    delete target; // 如果没有找到路径
    return {};     // 返回空路径
}

int FindNextDirectionBoat(const MapArray &map, const Point cur_core, int cur_direction, int cur_opration)
{
    int next_direction = -1;
    Point next_core = get_next_core(cur_core, cur_direction, cur_opration, next_direction);

    std::array<int, 3> index_array = {0, 1, 2};
    std::random_device rd;
    std::mt19937 g(rd());

    // 使用shuffle函数打乱数组
    std::shuffle(index_array.begin(), index_array.end(), g);
    int index = -1;
    for (int i = 0; i < 3; i++)
    {
        if (index_array[i] == cur_opration)
        {
            index = i;
            break;
        }
    }
    int start_index = index;
    while (!check_boat_coexistence(map, next_core, next_direction))
    {
        index = (index + 1) % 3;
        if (index == start_index)
        {
            std::cerr << "该船没找到新的方向" << std::endl;
            return -2;
        }
        next_core = get_next_core(cur_core, cur_direction, index_array[index], next_direction);
    }
    return index_array[index];
}