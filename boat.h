#pragma once
#include <iostream>
#include <vector>
#include <random>

#include "map.h"

constexpr int boat_price = 8000;


struct Boat
{
    int id, x, y, direction;
    int cur{-1};
    int to_T_cur{-1};
    int goods{0};
    int target_berth_index{-1};
    int target_T_index{-1};
    int stay_time{0};
    int get_berth_num{0};
    int crash_time{0};
    std::vector<int> path;
    std::vector<int> to_T_path;
    
    // status正常行驶状态（状态 0）   恢复状态（状态 1）   装载状态（状态 2）
    int goods_num, status;
    Boat() {}
    Boat(int startX, int startY)
    {
        x = startX;
        y = startY;
    }
};


void purchase_boat(int& money,int cur_boat_num, int max_boat_num,std::vector<Point>& boat_purchase_point , int robot_num , int max_robot_num);

void boat_move_dirction(int boat_index, int opration);