#pragma once
#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <array>
constexpr int max_berth_num = 10;
const int MAP_SIZE = 200;

// 定义地图字符常量
const char EMPTY_LAND = '.';            // 空地
const char LAND_ROAD = '>';             // 陆地主干道
const char SEA = '*';                   // 海洋
const char SEA_ROUTE = '~';             // 海洋主航道
const char OBSTACLE = '#';              // 障碍
const char ROBOT_START = 'R';           // 机器人购买地块，同时该地块也是主干道
const char SHIP_START = 'S';            // 船舶购买地块，同时该地块也是主航道
const char DOCK = 'B';                  // 泊位
const char DOCK_AREA = 'K';             // 靠泊区
const char TRANSPORT_BLOCK = 'C';       // 海陆立体交通地块
const char TRANSPORT_BLOCK_ROUTE = 'c'; // 海陆立体交通地块，同时为主干道和主航道
const char DELIVERY_POINT = 'T';        // 交货点


// Other

struct Point
{
    int x, y;
    // 重载 == 运算符
    bool operator==(const Point &other) const
    {
        return (x == other.x) && (y == other.y);
    }
};

struct PointDistance
{
    int x, y, distance;
};
struct MapNode
{
    char type;
    bool is_good;
    bool is_robot;
    bool is_boat;
};

// 定义二维的 std::array
using MapArray = std::array<std::array<MapNode, MAP_SIZE>, MAP_SIZE>;

using DirectionsArray = std::array<std::array<PointDistance, MAP_SIZE>, MAP_SIZE>;

using DirectionsArrays = std::array<std::array<std::array<PointDistance, max_berth_num>, MAP_SIZE>, MAP_SIZE>;

extern std::unordered_map<std::string, int> directionToIndex;

extern std::unordered_map<int, std::string> IndexTodirection;
auto HasLandAround(const MapArray &map, int x, int y) -> bool;

auto Findcorner(const MapArray &map, const std::vector<int> &five_index, const std::vector<Point> &berth_vertex) -> std::vector<Point>;

auto check_is_arrive_robot(const MapArray &map, int x, int y) -> bool;

auto check_robot_coexistence(const MapArray &map, int x, int y) -> bool;

void ProcessMap(char grid[MAP_SIZE][MAP_SIZE], MapArray& map, std::vector<Point>&robot_purchase_point,  std::vector<Point>&boat_purchase_point,std::vector<Point>&delivery_point);

bool check_boat_coexistence(const MapArray &map, Point pt, int dir);

auto get_next_core(Point pre_core, int pre_direction, int opration, int& new_direction) -> Point;
Point get_previous_core(Point next_core, int next_direction, int operation, int& pre_direction);
auto get_occupied_point(Point pre_core, int pre_direction) -> std::array<Point, 6>;
auto check_boat_arrive(MapArray& map,Point core, int direction) -> bool;
auto check_map_type_boat(MapArray& map,int x, int y) -> bool;

bool check_maptype_boat_coexistence(const MapArray &map, Point p);

void boad_set_map_false(MapArray &map,Point pt, int dir);
void boad_set_map_true(MapArray &map,Point pt, int dir);
bool check_boat_coexistence(const MapArray &map, Point pt, int dir);