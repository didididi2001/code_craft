#include "map.h"

std::unordered_map<std::string, int> directionToIndex{
    {"-10", 0},
    {"10", 1},
    {"0-1", 2},
    {"01", 3}};

std::unordered_map<int, std::string> IndexTodirection{
    {0, "-10"},
    {1, "10"},
    {2, "0-1"},
    {3, "01"}};
auto HasLandAround(const MapArray &map, int x, int y) -> bool
{
    // 检查指定点的上、下、左、右四个方向
    if (x - 1 >= 0 && map[x - 1][y].type == EMPTY_LAND) // 上
        return true;
    if (x + 1 < MAP_SIZE && map[x + 1][y].type == EMPTY_LAND) // 下
        return true;
    if (y - 1 >= 0 && map[x][y - 1].type == EMPTY_LAND) // 左
        return true;
    if (y + 1 < MAP_SIZE && map[x][y + 1].type == EMPTY_LAND) // 右
        return true;

    // 如果四个方向都是海洋，则返回 false
    return false;
}

auto Findcorner(const MapArray &map, const std::vector<int> &five_index, const std::vector<Point> &berth_vertex) -> std::vector<Point>
{
    std::vector<Point> corner;
    for (auto index : five_index)
    {
        int dx_[] = {3, 3, 0, 0};
        int dy_[] = {0, 3, 0, 3};
        int num = 0;
        Point t = berth_vertex[index];
        for (int i = 0; i < 4; i++)
        {
            if (HasLandAround(map, t.x + dx_[i], t.y + dy_[i]))
            {
                num++;
                corner.push_back({t.x + dx_[i], t.y + dy_[i]});
            }
            if (num == 2)
                break;
        }
    }
    return corner;
}

auto check_is_arrive_robot(const MapArray &map, int x, int y) -> bool
{
    if (map[x][y].type == EMPTY_LAND || map[x][y].type == LAND_ROAD || map[x][y].type == ROBOT_START || map[x][y].type == TRANSPORT_BLOCK || map[x][y].type == TRANSPORT_BLOCK_ROUTE || map[x][y].type == DOCK)
    {
        return true;
    }

    return false;
}

auto check_robot_coexistence(const MapArray &map, int x, int y) -> bool
{
    if (map[x][y].type == LAND_ROAD || map[x][y].type == TRANSPORT_BLOCK_ROUTE || map[x][y].type == ROBOT_START)
    {
        return true;
    }

    return false;
}

void ProcessMap(char grid[MAP_SIZE][MAP_SIZE], MapArray& map, std::vector<Point>&robot_purchase_point,  std::vector<Point>&boat_purchase_point,std::vector<Point>&delivery_point)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            map[i][j].type = grid[i][j];
            map[i][j].is_robot = false;
            if (grid[i][j] == ROBOT_START)
                robot_purchase_point.push_back({i, j});
            else if (grid[i][j] == SHIP_START)
                boat_purchase_point.push_back({i, j});
            else if (grid[i][j] == DELIVERY_POINT)
                delivery_point.push_back({i, j});
        }
    }
}

auto get_next_core(Point core, int pre_direction, int opration, int &new_direction) -> Point
{
    if (opration == 0)
    {
        // forward
        // direction: 0 right, 1 left, 2 up, 3 down
        switch (pre_direction)
        {
        case 0:
            new_direction = 0;
            return Point{core.x, core.y + 1};
            break;
        case 1:
            new_direction = 1;
            return Point{core.x, core.y - 1};
            break;
        case 2:
            new_direction = 2;
            return Point{core.x - 1, core.y};
            break;
        case 3:
            new_direction = 3;
            return Point{core.x + 1, core.y};
            break;
        default:
            std::cerr << "Error: direction is not valid" << std::endl;
            break;
        }
    }
    else if (opration == 1)
    {
        // turn right
        // direction: 0 right, 1 left, 2 up, 3 down
        switch (pre_direction)
        {
        case 0:
            new_direction = 3;
            return Point{core.x, core.y + 2};
            break;
        case 1:
            new_direction = 2;
            return Point{core.x, core.y - 2};
            break;
        case 2:
            new_direction = 0;
            return Point{core.x - 2, core.y};
            break;
        case 3:
            new_direction = 1;
            return Point{core.x + 2, core.y};
            break;
        default:
            std::cerr << "Error: direction is not valid" << std::endl;
            break;
        }
    }
    else if (opration == 2)
    {
        // turn left
        // direction: 0 right, 1 left, 2 up, 3 down
        switch (pre_direction)
        {
        case 0:
            new_direction = 2;
            return Point{core.x + 1, core.y + 1};
            break;
        case 1:
            new_direction = 3;
            return Point{core.x - 1, core.y - 1};
            break;
        case 2:
            new_direction = 1;
            return Point{core.x - 1, core.y + 1};
            break;
        case 3:
            new_direction = 0;
            return Point{core.x + 1, core.y - 1};
            break;
        default:
            std::cerr << "Error: direction is not valid" << std::endl;
            break;
        }
    }
    else
    {
        std::cerr << "Error: operation is not valid" << std::endl;
    }
    return {-1, -1};
}

Point get_previous_core(Point next_core, int next_direction, int operation, int &pre_direction)
{
    if (operation == 0)
    {
        // forward
        switch (next_direction)
        {
        case 0:
            pre_direction = 0;
            return Point{next_core.x, next_core.y - 1};
        case 1:
            pre_direction = 1;
            return Point{next_core.x, next_core.y + 1};
        case 2:
            pre_direction = 2;
            return Point{next_core.x + 1, next_core.y};
        case 3:
            pre_direction = 3;
            return Point{next_core.x - 1, next_core.y};
        default:
            std::cerr << "Error: next_direction is not valid" << std::endl;
            break;
        }
    }
    else if (operation == 1)
    {
        // turn right
        switch (next_direction)
        {
        case 0:
            pre_direction = 2;
            return Point{next_core.x + 2, next_core.y};
        case 1:
            pre_direction = 3;
            return Point{next_core.x - 2, next_core.y};
        case 2:
            pre_direction = 1;
            return Point{next_core.x, next_core.y + 2};
        case 3:
            pre_direction = 0;
            return Point{next_core.x, next_core.y - 2};
        default:
            std::cerr << "Error: next_direction is not valid" << std::endl;
            break;
        }
    }
    else if (operation == 2)
    {
        // turn left
        switch (next_direction)
        {
        case 0:
            pre_direction = 3;
            return Point{next_core.x - 1, next_core.y + 1};
        case 1:
            pre_direction = 2;
            return Point{next_core.x + 1, next_core.y - 1};
        case 2:
            pre_direction = 0;
            return Point{next_core.x - 1, next_core.y - 1};
        case 3:
            pre_direction = 1;
            return Point{next_core.x + 1, next_core.y + 1};
        default:
            std::cerr << "Error: next_direction is not valid" << std::endl;
            break;
        }
    }
    else
    {
        std::cerr << "Error: operation is not valid" << std::endl;
    }
    return {-1, -1};
}
auto get_occupied_point(Point core, int direction) -> std::array<Point, 6>
{
    // direction: 0 right, 1 left, 2 up, 3 down
    std::array<Point, 6> occupied_point;
    switch (direction)
    {
    case 0:
        occupied_point[0] = Point{core.x, core.y};
        occupied_point[1] = Point{core.x, core.y + 1};
        occupied_point[2] = Point{core.x, core.y + 2};
        occupied_point[3] = Point{core.x + 1, core.y};
        occupied_point[4] = Point{core.x + 1, core.y + 1};
        occupied_point[5] = Point{core.x + 1, core.y + 2};
        break;
    case 1:
        occupied_point[0] = Point{core.x, core.y};
        occupied_point[1] = Point{core.x, core.y - 1};
        occupied_point[2] = Point{core.x, core.y - 2};
        occupied_point[3] = Point{core.x - 1, core.y};
        occupied_point[4] = Point{core.x - 1, core.y - 1};
        occupied_point[5] = Point{core.x - 1, core.y - 2};
        break;
    case 2:
        occupied_point[0] = Point{core.x, core.y};
        occupied_point[1] = Point{core.x - 1, core.y};
        occupied_point[2] = Point{core.x - 2, core.y};
        occupied_point[3] = Point{core.x, core.y + 1};
        occupied_point[4] = Point{core.x - 1, core.y + 1};
        occupied_point[5] = Point{core.x - 2, core.y + 1};
        break;
    case 3:
        occupied_point[0] = Point{core.x, core.y};
        occupied_point[1] = Point{core.x + 1, core.y};
        occupied_point[2] = Point{core.x + 2, core.y};
        occupied_point[3] = Point{core.x, core.y - 1};
        occupied_point[4] = Point{core.x + 1, core.y - 1};
        occupied_point[5] = Point{core.x + 2, core.y - 1};
        break;
    default:
        std::cerr << "Error: direction is not valid" << std::endl;
        break;
    }
    return occupied_point;
}

auto check_map_type_boat(MapArray &map, int x, int y) -> bool
{
    if (map[x][y].type == DOCK_AREA || map[x][y].type == SEA_ROUTE || map[x][y].type == SEA || map[x][y].type == TRANSPORT_BLOCK || map[x][y].type == DOCK || map[x][y].type == TRANSPORT_BLOCK_ROUTE || map[x][y].type == DELIVERY_POINT || map[x][y].type == SHIP_START)
    {
        return true;
    }
    else
    {
        return false;
    }
}

auto check_boat_arrive(MapArray &map, Point core, int direction) -> bool
{
    // direction: 0 right, 1 left, 2 up, 3 down
    std::array<Point, 6> occupied_point = get_occupied_point(core, direction);
    for (auto &point : occupied_point)
    {
        if (check_map_type_boat(map, point.x, point.y))
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool check_maptype_boat_coexistence(const MapArray &map, Point p){
    if (map[p.x][p.y].type == TRANSPORT_BLOCK_ROUTE || map[p.x][p.y].type == SEA_ROUTE || map[p.x][p.y].type == SHIP_START || map[p.x][p.y].type == DOCK || map[p.x][p.y].type == DOCK_AREA || map[p.x][p.y].type == DELIVERY_POINT)
    {
        return true;
    } else {
        return false;
    }
}
bool check_boat_coexistence(const MapArray &map, Point pt, int dir)
{
    auto boat = get_occupied_point(pt, dir);
    for(auto& p : boat){
        if(!check_maptype_boat_coexistence(map, p) && map[p.x][p.y].is_boat){
            return false;
        }
    }
    return true;
}

void boad_set_map_true(MapArray &map,Point pt, int dir){
    auto boat = get_occupied_point(pt, dir);
    for(auto& p : boat){
        map[p.x][p.y].is_boat = true;
    }
}
void boad_set_map_false(MapArray &map,Point pt, int dir){
    auto boat = get_occupied_point(pt, dir);
    for(auto& p : boat){
        map[p.x][p.y].is_boat = false;
    }
}