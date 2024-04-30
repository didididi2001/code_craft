#include <algorithm>

#include "good.h"


// void GoodPool::CleanInvalidCargo() {
//     for (auto it = goods_.begin(); it != goods_.end();) {
//         if (it->GetExistenceTime() < 0) {
//             auto point = it->GetPoint();
//             map_[point.x][point.y].is_good = false;
//             it = goods_.erase(it);
//         } else {
//             ++it;
//         }
//     }
// }

void GoodPool::CleanInvalidCargo(int id)
{
    std::vector<int> keysToRemove; // 保存要删除的键
    for (auto it = goods_.begin(); it != goods_.end(); ++it)
    {
        if (it->second->GetExistenceTime(id) > 1000)
        {
            auto t = it->second->GetPoint();
            map_[t.x][t.y].is_good = false;
            keysToRemove.push_back(it->first); // 将要删除的键添加到向量中
        }
    }
    // 循环结束后删除元素
    for (int key : keysToRemove)
    {
        goods_.erase(key);
    }
}


auto GoodPool::IsVlaid(Point point,int id) -> bool
{
    if (goods_.find(point.x * 200 + point.y) == goods_.end())
        return false;
    return goods_[point.x * 200 + point.y]->GetExistenceTime(id) <= 1000;
}

auto GoodPool::Search(int x, int y) -> std::shared_ptr<Good>
{
    return goods_[x * 200 + y];
}
// Point GoodPool::Find(Point start_point) {
//     // 计算哈夫曼距离
//     auto huffmanDistance = [](const Point& p1, const Point& p2) {
//         return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
//     };

//     // 对 goods_ 按照哈夫曼距离 + 20 < Existtime 排序
//     std::sort(goods_.begin(), goods_.end(), [&]( Good& a, Good& b) {
//         return (huffmanDistance(start_point, a.GetPoint())) < (huffmanDistance(start_point, b.GetPoint()));
//     });

//     // 找到第一个满足条件的 Good 对象
//     for (Good& good : goods_) {
//         if ((huffmanDistance(start_point, good.GetPoint()) + time_ < good.GetExistenceTime()) && map_[good.GetPoint().x][good.GetPoint().y].is_good == true ) {
//             map_[good.GetPoint().x][good.GetPoint().y].is_good == false;
//             return good.GetPoint();
//         }
//     }
//     // 如果没有满足条件的，则返回第一个对象
//     return {-1,-1};
// }
