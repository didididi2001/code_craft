#pragma once
#include <iostream>
#include <vector>
#include <random>

#include "map.h"

constexpr int robot0_price = 2000;
constexpr int robot1_price = 5000;

struct Robot
{
    int x, y, goods, id;
    int status;
    int mbx, mby;
    int cur{-1};
    int corner_index{-1};
    std::vector<Point> pick_path;
    int type = 0;
    int cur_good_num = 0;
    Robot() {}
    Robot(int startX, int startY)
    {
        x = startX;
        y = startY;
    }
};

void purchase_robot(Robot robot[],int& money,int cur_robot_num, int boat_price, int max_robot_num,std::vector<Point>& robot_purchase_point , int boat_num);
