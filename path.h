#pragma once
#include <vector>

#include "good.h"
#include "map.h"
struct Node {
    Point position;
    // int g; // Cost from start to current node
    // int h; // Heuristic: Estimated cost from current node to goal
    // int f; // f = g + h
    Node* parent; // Pointer to parent node for constructing path

    // static bool Compare(Node* a, Node* b) {
    //     return a->f > b->f;
    // }
};

struct path_node
{
    int x, y;
    int direction;
};
struct path_node_with_parent
{
    int x, y;
    int pre_to_cur_opration;
    int current_direction;
    path_node_with_parent* parent;

};

// 节点表示地图上的一个点
struct Astar_Node {
    int x, y;
    float g, h, f;
    Astar_Node* parent;
    int pre_to_cur_opration;
    int current_direction;
    Astar_Node(int x, int y, Astar_Node* p = nullptr) : x(x), y(y), g(0), h(0), f(0), parent(p) {}

    bool operator==(const Astar_Node& o) const {
        return x == o.x && y == o.y;
    }
    int hash() {
        return x * MAP_SIZE * 4 + y * 4 + current_direction;
    }
};

// 自定义优先队列比较运算符
struct NodeCompare {
    bool operator()(const Astar_Node* lhs, const Astar_Node* rhs) const {
        return lhs->f > rhs->f;
    }
};
int hash(Point cur, int curr_direction) ;
// 计算曼哈顿距离作为启发函数
float heuristic(const Astar_Node& a, const Astar_Node& b);
float heuristic(const Point a, const Point b);

float get_map_speed(MapArray& map, Point p,int cur);
auto InitWith1(const MapArray& map) -> DirectionsArray;

auto InitWith10(const MapArray &map, const std::vector<Point> berth_vertex, std::array<std::array<int, MAP_SIZE>, MAP_SIZE>& nearest_berth_index, std::array<std::array<int, MAP_SIZE>, MAP_SIZE>& is_reach) -> DirectionsArrays;
auto AStar(const MapArray& map, Point start, Point goal) -> std::vector<Point>;
auto Move(Point direction) -> int;

auto CalculateDistance(Point a, Point b) -> int;

auto bfs(MapArray& map, Point pstart, Point& ptarget) -> std::vector<Point>; 

auto BfsFindNNode(MapArray &map, GoodPool &goodpool,DirectionsArray& direction,int id, int target_nums, Point pstart, Point &ptarget,bool largest_val, int max_path_lenth = 100) -> std::vector<Point>;

auto FindNextDirection(const MapArray &map, const Point cur_point, const Point origin_direction) -> Point;

auto BFSWithTarget(MapArray& map, Point pstart, const Point& ptarget) -> std::vector<Point>;


auto FindNearestBerth(std::array<PointDistance, max_berth_num>& distance, std::vector<int>& existing_berth) -> int;

auto FindNearestBerth_index(MapArray& map,DirectionsArrays& distances, int berth_num) ->std::array<std::array<int, MAP_SIZE>, MAP_SIZE>;

auto find_berth_to_berth_path(DirectionsArrays &distances,std::vector<Point>& berth_vertex) -> std::vector<std::vector<std::vector<Point>>>;


// direction: 0 right, 1 left, 2 up, 3 down    
auto get_next_core(Point pre_core, int pre_direction, int opration, int& new_direction) -> Point;
Point get_previous_core(Point next_core, int next_direction, int operation, int& pre_direction);
auto get_occupied_point(Point pre_core, int pre_direction) -> std::array<Point, 6>;
auto check_boat_arrive(MapArray& map,Point core, int direction) -> bool;
auto check_map_type_boat(MapArray& map,int x, int y) -> bool;

auto BFS_berth_to_nearest_T(MapArray& map) -> std::array<std::array<std::array<int, 4>, MAP_SIZE>, MAP_SIZE>;

auto BFS_T_to_berth(MapArray& map, Point pstart, int start_direction, Point ptarget) -> std::vector<int>;


auto Astar_T_to_berth(MapArray& map, Point pstart, int start_direction, Point ptarget) -> std::vector<int>;

int FindNextDirectionBoat(const MapArray &map, const Point cur_core, int cur_direction, int cur_opration);

std::vector<int> init_berth_nearest_T_index(MapArray& map, std::vector<Point>& T,const std::vector<Point>& berth_vertex);