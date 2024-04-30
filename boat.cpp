#include "boat.h"

void purchase_boat(int& money, int cur_boat_num, int max_boat_num, std::vector<Point> &boat_purchase_point , int robot_num , int max_robot_num)
{
    while (//cur_boat_num == 0 || (robot_num >= max_robot_num && 
    money > boat_price && cur_boat_num < max_boat_num)
    {
        // 使用随机设备生成种子
        std::random_device rd;

        // 使用 Mersenne Twister 引擎来生成随机数
        std::mt19937 gen(rd());

        // 定义随机数分布，范围在 0 到 10 之间
        std::uniform_int_distribution<> dis(0, boat_purchase_point.size() - 1);
        // 生成随机数
        int random_number = dis(gen);
        printf("lboat %d %d\n", boat_purchase_point[random_number].x, boat_purchase_point[random_number].y);
        cur_boat_num++;
        money -= boat_price;
        std::cerr << "purchase one boat\n";
    }
}

void boat_move_dirction(int boat_index, int opration)
{
    if (opration == 0)
    {
        printf("ship %d\n", boat_index);
    }
    else if (opration == 1)
    {
        printf("rot %d %d\n", boat_index, 0);
    }
    else if (opration == 2)
    {
        printf("rot %d %d\n", boat_index, 1);
    }
    else
    {
        std::cerr << "invalid boat_move_dirction|"
                  << "boat_index :" << boat_index << "    opration:" << opration << std::endl;
    }
}