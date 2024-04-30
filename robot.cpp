#include "robot.h"
void purchase_robot(Robot robot[],int& money,int cur_robot_num, int boat_price, int max_robot_num,std::vector<Point>& robot_purchase_point , int boat_num){
    while (money - boat_price > robot0_price && cur_robot_num < max_robot_num)
        {
            // 使用随机设备生成种子
            std::random_device rd;

            // 使用 Mersenne Twister 引擎来生成随机数
            std::mt19937 gen(rd());

            // 定义随机数分布，范围在 0 到 10 之间
            std::uniform_int_distribution<> dis(0, robot_purchase_point.size() - 1);
            // 生成随机数
            int random_number = dis(gen);
            printf("lbot %d %d %d\n", robot_purchase_point[random_number].x, robot_purchase_point[random_number].y , 0);
            robot[cur_robot_num].type = 0;
            cur_robot_num++;
            money -= robot0_price;
            
            std::cerr << "purchase one robot\n";
        }
}

