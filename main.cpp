#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <random>
#include <algorithm>

#include "map.h"

#include "boat.h"
#include "good.h"
#include "path.h"
#include "utils.h"
#include "robot.h"

using namespace std;

const int N = 200;

// Counters
int total_generate_ = 0;
int cur_max_path_length = 0;
int pull_money = 0;
int pull_num = 0;
int get_money = 0;

int target_berth_index = 0;
// Constants
constexpr int max_robot_num = 13;
constexpr int max_boat_num = 2;
constexpr int max_crash_time = 9;

// Containers
vector<int> existing_berth;
vector<int> berth_good_num;
vector<Point> robot_purchase_point;
vector<Point> boat_purchase_point;
vector<Point> delivery_point;
vector<Point> berth_vertex;
vector<int> berth_around_num;
std::unordered_map<int, int> berth_vertex_to_index;

// Counters
int robot_num = 0;
int boat_num = 0;
int berth_num = 0;
int goods_num = 0;
int frame_id = 0;

// Flags
bool largest_val = false;

int max_get_berth = 2;
// Maps and Arrays
int max_cur_bfs = 3;
MapArray map;
DirectionsArray direction;
DirectionsArrays direction_distance;
GoodPool good_pool(map);
array<array<int, MAP_SIZE>, MAP_SIZE> nearest_berth_index;
std::vector<std::vector<std::vector<Point>>> berth_to_berth_path;
array<array<array<int, 4>, MAP_SIZE>, MAP_SIZE> berth_to_nearst_T;
array<array<int, MAP_SIZE>, MAP_SIZE> is_reach;
vector<int> berth_nearest_T_index;
// Constants
int boat_capacity, id, money;

char grid[MAP_SIZE][MAP_SIZE];

Robot robot[max_robot_num + 10];
array<int, max_robot_num> robot_id;
array<int, max_boat_num> boat_id;
struct Berth
{
    int x, y;
    int loading_speed;
    Berth() {}
    Berth(int x, int y, int loading_speed)
    {
        this->x = x;
        this->y = y;
        this->loading_speed = loading_speed;
    }
} berth[10];

Boat boat[10];

void Init()
{
    cout << "Init\n";
    for (int i = 0; i < N; i++)
    {
        scanf("%s", grid[i]);
        // for (int j = 0; j< N; ++j) {
        //     grid[i][j] = getchar();
        // }
        // getchar();
    }
    ProcessMap(grid, map, robot_purchase_point, boat_purchase_point, delivery_point);
    // ProcessMap();
    cout << "ProcessMap\n";
    scanf("%d", &berth_num);
    for (int i = 0; i < berth_num; i++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d", &berth[id].x, &berth[id].y, &berth[id].loading_speed);
        berth_vertex.push_back({berth[id].x, berth[id].y});
        existing_berth.push_back(1);
        berth_good_num.push_back(0);
        berth_around_num.push_back(0);
        // berth_nearest_T_index.push_back(-1);
    }
    for (int i = 0; i < berth_vertex.size(); i++)
    {
        int key = berth_vertex[i].x * MAP_SIZE + berth_vertex[i].y;
        berth_vertex_to_index[key] = i;
    }
    scanf("%d", &boat_capacity);
    //
    char okk[100];
    // cout << "init direction_distance\n";
    scanf("%s", okk);
    // cout << "init direction_distance\n";
    direction_distance = InitWith10(map, berth_vertex, nearest_berth_index, is_reach);
    // cerr << " init direction\n ";
    // direction = InitWith1(map);
    // nearest_berth_index = FindNearestBerth_index(map, direction_distance, berth_vertex.size());
    // berth_to_berth_path = find_berth_to_berth_path(direction_distance, berth_vertex);
    // berth_to_nearst_T = BFS_berth_to_nearest_T(map);
    berth_nearest_T_index = init_berth_nearest_T_index(map, delivery_point, berth_vertex);
    for (auto temp : berth_nearest_T_index)
    {
        cerr << temp << " ";
    }
    cerr << "\n";
    for (int i = 0; i < max_robot_num; i++)
    {
        robot_id[i] = i;
    }
    for (int i = 0; i < max_boat_num; i++)
    {
        boat_id[i] = i;
    }
    printf("OK\n");
    fflush(stdout);
}

int Input()
{
    // 读入帧数和钱
    scanf("%d", &money);
    // cerr << "money: " << money << endl;
    scanf("%d", &goods_num);
    for (int i = 0; i < goods_num; i++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        if (direction[x][y].x == -1 && direction[x][y].y == -1)
        {
            continue;
        }
        if (val != 0)
        {
            map[x][y].is_good = true;
            std::shared_ptr<Good> good = std::make_shared<Good>(x, y, val, frame_id);
            good_pool.AddGoods(good);
            total_generate_ += val;
            auto berth_index = nearest_berth_index[x][y];
            // cerr << "berth_index:" <<berth_index << "\n";
            if (berth_index != -1)
            {
                berth_around_num[berth_index]++;
            }
        }
        else
        {
            auto berth_index = nearest_berth_index[x][y];
            // cerr << "berth_index:" << berth_index << "\n";
            if (berth_index != -1)
            {
                berth_around_num[berth_index]--;
            }
        }
    }

    scanf("%d", &robot_num);
    for (int i = 0; i < robot_num; i++)
    {
        scanf("%d%d%d%d", &robot[i].id, &robot[i].goods, &robot[i].x, &robot[i].y);
    }

    scanf("%d", &boat_num);
    for (int i = 0; i < boat_num; i++)
    {
        scanf("%d%d%d%d%d%d\n", &boat[i].id, &boat[i].goods_num, &boat[i].x, &boat[i].y, &boat[i].direction, &boat[i].status);
    }

    char okk[100];
    scanf("%s", okk);
    return id;
}

// -----------------------------------------------start----------------------------------------
int main()
{
    ofstream file("error.log"); // 打开文件
    cerr.rdbuf(file.rdbuf());   // 重定向 // cerr 到文件
    Init();
    while (scanf("%d", &frame_id) != EOF)
    {
        int cur_bfs = 0;
        int id = Input();
        // purchase robot

        // cerr << "robot_num: " << robot_num << endl;
        // cerr << "money: " << money << endl;
        purchase_robot(robot, money, robot_num, boat_price, max_robot_num, robot_purchase_point, boat_num);
        purchase_boat(money, boat_num, max_boat_num, boat_purchase_point, robot_num, max_robot_num);

        if (frame_id == 1)
        {
            for (int i = 0; i < robot_num; i++)
            {
                int x = robot[i].x;
                int y = robot[i].y;
                if (map[x][y].type == '.' || map[x][y].type == 'B' || map[x][y].type == 'C')
                {
                    map[x][y].is_robot = true;
                }
                else
                {
                    map[x][y].is_robot = false;
                }
            }
        }
        if (frame_id <= 50)
        {
            puts("OK");
            fflush(stdout);
            continue;
        }
        // for(int i = 0; i < MAP_SIZE; i++){
        //     for(int j = 0; j < MAP_SIZE;j++){
        //         if(map[i][j].is_boat){
        //             cerr << "x: " << i << "y:" << j << "\n";
        //         }
        //     }
        // }
        int left = 0; // 左指针，指向当前已处理的0序列的末尾
        int right = 0;
        while (right < boat_num)
        {
            int x = boat[boat_id[right]].x;
            int y = boat[boat_id[right]].y;
            int dir = boat[boat_id[right]].direction;
            auto ps = get_occupied_point({x, y}, dir);
            bool flag = false;
            for (auto &p : ps)
            {
                if (check_map_type_boat(map, p.x, p.y))
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                std::swap(boat_id[left], boat_id[right]); // 交换非零元素到左侧
                left++;
            }
            right++;
        }
        for (int t = 0; t < boat_num; t++)
        {
            int i = boat_id[t];
            auto &cur_boat = boat[i];
            Point cur_point{cur_boat.x, cur_boat.y};
            cerr << "boat" << i << " 坐标" << cur_boat.x << " " << cur_boat.y << "  goods:"
             << cur_boat.goods << " cur:" << cur_boat.cur << " dirction:" << cur_boat.direction << " status:" << cur_boat.status << " stay_time: " << cur_boat.stay_time << " crash_time:" << cur_boat.crash_time <<" to_T_cur:" << cur_boat.to_T_cur <<"\n";
            if (cur_boat.goods == 0 && cur_boat.cur == cur_boat.path.size())
            {
                // 到达泊位
                // cerr << " 输出berth \n";
                if (cur_boat.status == 0)
                {
                    boad_set_map_false(map, cur_point, cur_boat.direction);
                    printf("berth %d\n", cur_boat.id);
                    // cur_boat.target_berth_index = -1;
                    // cur_boat.target_T_index = -1;
                    cur_boat.goods = 1;
                }
            }
            if (cur_boat.goods == 0 && cur_boat.cur == -1)
            {
                Point target;
                // 代表无货 没有路径 设置路径  以及停靠时间
                if (cur_boat.target_berth_index == -1)
                {
                    cur_boat.target_berth_index = get_max_num_berth_id(berth_good_num);
                    target = berth_vertex[cur_boat.target_berth_index];
                    // cerr << "target:" << target.x << "  " << target.y << "\n";
                    cerr << "减\n";
                    berth_good_num[cur_boat.target_berth_index] -= min(boat_capacity, berth_good_num[cur_boat.target_berth_index]);
                    cur_boat.path.clear();
                }
                else
                {
                    target = berth_vertex[cur_boat.target_berth_index];
                }
                cerr << "target: " << target.x << " "  <<  target.y<< std::endl;
                cerr << "cur_point: " << cur_point.x << " "  <<  cur_point.y<< std::endl;
                boad_set_map_false(map, cur_point, cur_boat.direction);
                // cur_boat.path = BFS_T_to_berth(map, cur_point,cur_boat.direction, target);
                cur_boat.path = Astar_T_to_berth(map, cur_point, cur_boat.direction, target);
                boad_set_map_true(map, cur_point, cur_boat.direction);
                // cur_boat.path = Astar_T_to_berth(map, cur_point, cur_boat.direction, target);
                cerr << "cur_boat.path.size: " << cur_boat.path.size() << std::endl;
                if (!cur_boat.path.empty())
                {
                    cur_boat.stay_time = boat_capacity / berth[cur_boat.target_berth_index].loading_speed + 1;
                    cur_boat.cur = 0;
                }
                else
                {
                    boad_set_map_false(map, cur_point, cur_boat.direction);
                    printf("dept %d\n", cur_boat.id);
                    cur_boat.cur = -1;
                }
            }
            if (cur_boat.goods == 0 && cur_boat.cur != -1 && !cur_boat.path.empty())
            {
                // 代表无货，但是已经设置路径
                if (cur_boat.status == 1)
                {
                    // 船只处于恢复状态，不能行走
                    boad_set_map_true(map, cur_point, cur_boat.direction);
                }
                else if (cur_boat.status == 0 || cur_boat.status == 2)
                {
                    if (cur_boat.crash_time > 0)
                    {
                        cur_boat.crash_time--;
                    }
                    else
                    {
                        // 船只处于行走状态，可以行走
                        // get next core
                        int new_dirction = -1;
                        boad_set_map_false(map, cur_point, cur_boat.direction);
                        auto next_core = get_next_core(cur_point, cur_boat.direction, cur_boat.path[cur_boat.cur], new_dirction);
                        // TODO 添加检测其他船只位置
                        // cerr << "new_direction: " << new_dirction << "\n";
                        if (check_boat_coexistence(map, next_core, new_dirction))
                        {
                            // 说明下一个位置能走
                            boat_move_dirction(cur_boat.id, cur_boat.path[cur_boat.cur]);
                            cur_boat.cur++;
                            boad_set_map_true(map, next_core, new_dirction);
                        }
                        else
                        {
                            boad_set_map_true(map, cur_point, cur_boat.direction);
                            // 前面有机器人
                            std::random_device rd;
                            std::mt19937 gen(rd());

                            // 定义随机数分布范围
                            std::uniform_int_distribution<> dis(1, 100); // 生成1到100之间的随机整数

                            // 生成随机数
                            int random_number = dis(gen);
                            cur_boat.crash_time = random_number % max_crash_time;
                            cur_boat.cur = -1;
                        }
                    }
                }
            }
            if (cur_boat.goods == 1 && cur_boat.stay_time > 0)
            {
                // 准备靠岸
                if (cur_boat.status == 1)
                {
                    // 入泊位中
                    boad_set_map_true(map, cur_point, cur_boat.direction);
                }
                else if (cur_boat.status == 2 || cur_boat.status == 0)
                {
                    //
                    boad_set_map_true(map, cur_point, cur_boat.direction);
                    cur_boat.stay_time--;
                }
                if (cur_boat.stay_time == 0)
                {

                    if (cur_boat.get_berth_num < max_get_berth)
                    {
                        cur_boat.get_berth_num++;
                        cur_boat.cur = -1;
                        cur_boat.goods = 0;
                        cur_boat.target_berth_index = (cur_boat.target_berth_index + 1) % berth_vertex.size();
                        // berth_good_num[cur_boat.target_berth_index] -= min(boat_capacity, berth_good_num[cur_boat.target_berth_index]);
                        cur_boat.path.clear();
                    }
                    else
                    {
                        cur_boat.get_berth_num = 0;
                    }
                    // cerr << "减\n";
                    // cerr << cur_boat.status <<  " dept\n";
                    boad_set_map_false(map, cur_point, cur_boat.direction);
                    // printf("dept %d\n", cur_boat.id);
                    cur_boat.to_T_cur = -1;
                    cur_boat.to_T_path.clear();
                }
            }
            if (cur_boat.goods == 1 && cur_boat.stay_time == 0 && cur_boat.to_T_cur == cur_boat.to_T_path.size())
            {
                boad_set_map_true(map, cur_point, cur_boat.direction);
                cur_boat.goods = 0;
                cur_boat.cur = -1;
                cur_boat.target_berth_index = -1;
                cur_boat.target_T_index = -1;
                cur_boat.path.clear();
            }
            if (cur_boat.goods == 1 && cur_boat.stay_time == 0 && cur_boat.to_T_cur == -1)
            {
                Point target;
                cerr << "cur_boat.target_T_index:" << cur_boat.target_T_index << "\n";
                if (cur_boat.target_T_index == -1)
                {
                    // 设置目标
                    int hash = cur_boat.x * MAP_SIZE + cur_boat.y;
                    cur_boat.target_T_index = berth_nearest_T_index[berth_vertex_to_index[hash]];
                    cerr << "set cur_boat.target_T_index:" << cur_boat.target_T_index << "\n";

                    target = delivery_point[cur_boat.target_T_index];
                }
                else
                {
                    target = delivery_point[cur_boat.target_T_index];
                    ;
                }
                cerr << "target: " << target.x << " " << target.y << std::endl;
                cerr << "cur_point: " << cur_point.x << " " << cur_point.y << std::endl;
                boad_set_map_false(map, cur_point, cur_boat.direction);
                // cur_boat.path = BFS_T_to_berth(map, cur_point,cur_boat.direction, target);
                cur_boat.to_T_path = Astar_T_to_berth(map, cur_point, cur_boat.direction, target);
                boad_set_map_true(map, cur_point, cur_boat.direction);
                cerr << "cur_boat.path.size: " << cur_boat.to_T_path.size() << std::endl;
                if (!cur_boat.to_T_path.empty())
                {
                    cur_boat.to_T_cur = 0;
                }
                else
                {
                    boad_set_map_false(map, cur_point, cur_boat.direction);
                    printf("dept %d\n", cur_boat.id);
                    cur_boat.to_T_cur = -1;
                }
            }
            if (cur_boat.goods == 1 && cur_boat.stay_time == 0 && cur_boat.to_T_cur != -1 && !cur_boat.to_T_path.empty())
            {
                if (cur_boat.status == 1)
                {
                    // 船只处于恢复状态，不能行走
                    boad_set_map_true(map, cur_point, cur_boat.direction);
                }
                else if (cur_boat.status == 0 || cur_boat.status == 2)
                {
                    if (cur_boat.crash_time > 0)
                    {
                        cur_boat.crash_time--;
                    }
                    else
                    {
                        // 船只处于行走状态，可以行走
                        // get next core
                        int new_dirction = -1;
                        boad_set_map_false(map, cur_point, cur_boat.direction);
                        auto next_core = get_next_core(cur_point, cur_boat.direction, cur_boat.to_T_path[cur_boat.to_T_cur], new_dirction);
                        // TODO 添加检测其他船只位置
                        // cerr << "new_direction: " << new_dirction << "\n";
                        if (check_boat_coexistence(map, next_core, new_dirction))
                        {
                            // 说明下一个位置能走
                            boat_move_dirction(cur_boat.id, cur_boat.to_T_path[cur_boat.to_T_cur]);
                            cur_boat.to_T_cur++;
                            boad_set_map_true(map, next_core, new_dirction);
                        }
                        else
                        {
                            boad_set_map_true(map, cur_point, cur_boat.direction);
                            // 前面有机器人
                            std::random_device rd;
                            std::mt19937 gen(rd());

                            // 定义随机数分布范围
                            std::uniform_int_distribution<> dis(1, 100); // 生成1到100之间的随机整数

                            // 生成随机数
                            int random_number = dis(gen);
                            cur_boat.crash_time = random_number % max_crash_time;
                            cur_boat.to_T_cur = -1;
                        }
                    }
                }
            }
        }
        left = 0;  // 左指针，指向当前已处理的0序列的末尾
        right = 0; // 右指针，用于遍历整个数组
        // 遍历数组，让共享机器人先走
        while (right < robot_num)
        {
            int x = robot[robot_id[right]].x;
            int y = robot[robot_id[right]].y;
            if (check_robot_coexistence(map, x, y))
            {
                std::swap(robot_id[left], robot_id[right]); // 交换非零元素到左侧
                left++;                                     // 更新左指针
            }
            right++; // 更新右指针
        }
        for (int t = 0; t < robot_num; t++)
        {
            int i = robot_id[t];
            // int cur_corner_index = robot[i].corner_index;
            auto robot_x = robot[i].x;
            auto robot_y = robot[i].y;
            // 取货
            // cerr << "机器人坐标：" << i << " " << robot_x << " " << robot_y << "机器人状态" << robot[i].goods << " " << robot[i].pick_path.size() << " " << robot[i].cur << " " << robot[i].corner_index << " "
                //  << "机器人目标：" << i << " " << robot[i].mbx << " " << robot[i].mby << endl;
            // if (robot[i].goods == 0 && robot[i].pick_path.size() == robot[i].cur && map[robot_x][robot_y].is_good)
            if (robot[i].goods == 0 && robot[i].pick_path.size() == robot[i].cur)
            {
                cerr << "robot[i].cur_good_num" << robot[i].cur_good_num << "\n";
                if (robot[i].mbx != robot_x || robot[i].mby != robot_y)
                {
                    robot[i].goods = 0;
                }
                else
                {
                    robot[i].goods = 1;
                    get_money += good_pool.Search(robot_x, robot_y)->GetVal();
                    printf("get %d\n", i);
                }
                cerr << "robot[i].type:" << robot[i].type << endl;
                if (robot[i].type == 0)
                {
                    robot[i].cur = -1;
                    robot[i].pick_path.clear();
                }
                if (robot[i].type == 1)
                {
                    if (robot[i].cur_good_num == 0)
                    {
                        robot[i].goods = 0;
                        robot[i].cur_good_num++;
                        robot[i].cur = -1;
                        robot[i].pick_path.clear();
                    }
                    else
                    {
                        robot[i].goods = 1;
                        robot[i].cur = -1;
                        robot[i].pick_path.clear();
                        robot[i].cur_good_num = 0;
                    }
                    cerr << "robot[i].goods:" << robot[i].goods << '\n';
                }

                // map[robot_x][robot_y].is_good = false;
            }
            if (robot[i].goods == 0 && robot[i].cur == -1)
            {

                // // cerr << "dasdsafa" << endl;
                if (is_reach[robot_x][robot_y] == 0)
                {
                    // cerr << " 机器人出不去" << endl;
                    continue;
                }
                // Point target_point = good_pool.Find({robot_x, robot_y});
                // // cerr << "机器人设置目标" << target_point.x << " " << target_point.y << endl;
                // robot[i].pick_path = AStar(map, {robot_x, robot_y}, target_point);
                // // cerr << "path_lenth:" << robot[i].pick_path.size() << endl;
                // robot[i].cur = 0;
                // // cerr << "机器人设置目标完成" << endl;

                // Point target_point{-1, -1};
                // Point target_points{-1,-1};
                // vector<Point> paths = BfsFindNNode(map, good_pool,2, {robot_x, robot_y}, target_points);
                // robot[i].pick_path.clear();
                // robot[i].pick_path = bfs(map, {robot_x, robot_y}, target_point);
                // // cerr << "机器人设置目标" << target_point.x << " " << target_point.y << endl;
                // // cerr << "path_lenth:" << robot[i].pick_path.size() << endl;
                // robot[i].cur = 0;

                if (cur_bfs < max_cur_bfs)
                {
                    // cerr << "搜目标robot[i].cur_good_num" << robot[i].cur_good_num << "\n";
                    // cerr << "robot[i].goods:" << robot[i].goods << '\n';
                    Point target_point{-1, -1};
                    robot[i].pick_path.clear();
                    robot[i].pick_path = BfsFindNNode(map, good_pool, direction, frame_id, 30, {robot_x, robot_y}, target_point, largest_val, 200);
                    // // cerr << "pick_path:" << robot[i].pick_path.size() << endl;
                    cur_bfs++;
                    if (target_point.x != -1 && target_point.y != -1)
                    {
                        // cerr << "path_lenth:" << robot[i].pick_path.size() << endl;
                        cur_max_path_length = max(cur_max_path_length, (int)robot[i].pick_path.size());
                        robot[i].cur = 0;
                        // for(auto t : robot[i].pick_path){
                        //     // cerr << "path" << t.x  << " " << t.y << endl;
                        // }
                        // cerr << "机器人设置目标完成" << endl;
                        robot[i].mbx = target_point.x;
                        robot[i].mby = target_point.y;
                        // cerr << "机器人设置目标" << robot[i].mbx << " " << robot[i].mby << endl;
                        map[target_point.x][target_point.y].is_good = false;
                        robot[i].corner_index = -1;
                    }
                    else
                    {

                        int max_num_berth_index = -1;
                        int max_num = -1;
                        for (int i = 0; i < berth_around_num.size(); i++)
                        {
                            if (i == 0 || max_num > berth_around_num[i])
                            {
                                max_num = berth_around_num[i];
                                max_num_berth_index = i;
                            }
                        }
                        // cerr << " max_num_berth_index: " << max_num_berth_index << endl;
                        if (max_num_berth_index >= 0 && max_num_berth_index < berth_around_num.size() && max_num_berth_index != robot[i].corner_index)
                        {
                            cerr << "转移" << endl;
                            robot[i].corner_index = max_num_berth_index;
                            robot[i].goods = 1;
                        }
                        else
                        {
                            cerr << "随机走俩步" << max_num_berth_index << robot[i].corner_index << endl;
                            map[robot_x][robot_y].is_robot = false;
                            auto new_direction = FindNextDirection(map, {robot_x, robot_y}, {-1, 0});
                            auto temp_x = robot_x + new_direction.x;
                            auto temp_y = robot_y + new_direction.y;
                            if (!check_robot_coexistence(map, temp_x, temp_y))
                            {
                                // cerr << "set_true"  << temp_x << " " << temp_y << " \n";
                                map[temp_x][temp_y].is_robot = true;
                            }

                            if (new_direction.x == 0 && new_direction.y == 0)
                            {
                            }
                            else
                            {
                                printf("move %d %d\n", i, Move(new_direction));
                            }
                        }
                    }
                }
                else
                {
                    // 当前机器人跳过该帧的搜索
                }
                // cerr << "robot[i].goods:" << robot[i].goods << '\n';
            }

            // 机器人没货移动
            int next_x, next_y;
            if (robot[i].goods == 0 && !robot[i].pick_path.empty() && robot[i].cur != -1)
            {
                // cerr << "机器人没货移动\n";
                map[robot_x][robot_y].is_robot = false;
                auto origin_direction = robot[i].pick_path[robot[i].cur];
                // // cerr << "origin_direction" << origin_direction.x << " " << origin_direction.y << endl;
                auto new_direction = FindNextDirection(map, {robot_x, robot_y}, origin_direction);
                // // cerr << "new_direction" << new_direction.x << " " << new_direction.y << endl;
                next_x = robot_x + new_direction.x;
                next_y = robot_y + new_direction.y;
                if (!check_robot_coexistence(map, next_x, next_y))
                {
                    // cerr << "set_true"  << next_x << " " << next_y << " \n";
                    map[next_x][next_y].is_robot = true;
                }
                if (new_direction == origin_direction)
                {
                    printf("move %d %d\n", i, Move(new_direction));
                    robot[i].cur++;
                }
                else if (new_direction == Point{0, 0})
                {
                }
                else
                {
                    map[robot[i].mbx][robot[i].mby].is_good = true;
                    printf("move %d %d\n", i, Move(new_direction));
                    robot[i].cur = -1;
                }

                // // cerr << robot[i].cur << "机器人没货移动" << robot[i].pick_path[robot[i].cur].x << " " << robot[i].pick_path[robot[i].cur].y << endl;
            }

            // 机器人有货移动
            if (robot[i].goods == 1)
            {
                // cerr << "机器人有货移动\n";
                map[robot_x][robot_y].is_robot = false;
                auto distance_vector = direction_distance[robot_x][robot_y];
                if (robot[i].corner_index == -1)
                {
                    robot[i].corner_index = FindNearestBerth(distance_vector, existing_berth);
                    // cerr << "机器人设置目标泊位" << robot[i].corner_index << endl;
                }
                auto index = robot[i].corner_index;

                auto origin_direction = Point{direction_distance[robot_x][robot_y][index].x, direction_distance[robot_x][robot_y][index].y};
                if (origin_direction.x == -1 && origin_direction.y == -1)
                {
                    // cerr << "泊位不可达" << robot[i].corner_index << endl;
                    next_x = robot_x;
                    next_y = robot_y;
                    if (!check_robot_coexistence(map, next_x, next_y))
                    {
                        // cerr << "set_true"  << next_x << " " << next_y << " \n";
                        map[next_x][next_y].is_robot = true;
                    }
                }
                else
                {
                    // cerr << "origin_direction" << origin_direction.x << " " << origin_direction.y << endl;
                    auto new_direction = FindNextDirection(map, {robot_x, robot_y}, origin_direction);
                    // cerr << "new_direction" << new_direction.x << " " << new_direction.y << endl;
                    next_x = robot_x + new_direction.x;
                    next_y = robot_y + new_direction.y;
                    if (!check_robot_coexistence(map, next_x, next_y))
                    {
                        // cerr << "set_true"  << next_x << " " << next_y << " \n";
                        map[next_x][next_y].is_robot = true;
                    }
                    if (new_direction == origin_direction)
                    {
                        printf("move %d %d\n", i, Move(new_direction));
                    }
                    else if (new_direction == Point{0, 0})
                    {
                        // cerr << "机器人不动" << endl;
                    }
                    else
                    {
                        printf("move %d %d\n", i, Move(new_direction));
                    }
                }
            }
            // if(robot[i].corner_index != -1){
            //         // cerr << "robot[i].corner_index:" << robot[i].corner_index << endl;
            //         // cerr << " 地图方向" << direction_distance[next_x][next_y][robot[i].corner_index].x << " " << direction_distance[next_x][next_y][robot[i].corner_index].y << endl;
            //     }
            // 卸货
            if (robot[i].goods == 1 && direction_distance[next_x][next_y][robot[i].corner_index].x == 0 && direction_distance[next_x][next_y][robot[i].corner_index].y == 0)
            {
                // cerr << "机器人放货" << endl;
                printf("pull %d\n", i);
                robot[i].goods = 0;
                robot[i].cur = -1;
                pull_num++;
                robot[i].pick_path.clear();
                berth_good_num[robot[i].corner_index]++;
                // robot[i].corner_index = -1;
            }
        }
        good_pool.CleanInvalidCargo(frame_id);
        // cerr << "cur_bfs:" << cur_bfs << endl;
        // cerr << " max_path_lenth:" << max_path_lenth << endl;
        for (int j = 0; j < berth_good_num.size(); j++)
        {
            cerr << berth_good_num[j] << " ";
        }
        cerr << "\n";
        puts("OK");
        fflush(stdout);
    }
    cerr << "pull_money:" << pull_money << endl;
    cerr << "pull_num:" << pull_num << endl;
    cerr << "get_money:" << get_money << endl;
    cerr << "total_generate_:" << total_generate_ << endl;
    return 0;
}