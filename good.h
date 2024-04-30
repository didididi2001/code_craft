#pragma once
#include <unordered_map>
#include <vector>
#include <memory>

#include "map.h"
class Good
{
    int x_, y_;
    int val_;
    bool status_{false};
    int birth_time_{0};
public:
    Good() = default;
    Good(int X, int Y, int val,int birth_time) :x_(X), y_(Y),val_(val),birth_time_(birth_time) {}
    auto GetVal() -> int { return val_; }
    auto GetHash() -> int { return x_ * 200 + y_;}
    auto GetPoint() -> Point { return { x_, y_ }; }
    auto GetExistenceTime(int id) const -> int { return id - birth_time_; }
    auto GetStatus() -> bool { return status_; }
    void SetStatus(bool status) { status_ = status; }
    

};

class GoodPool
{
    std::unordered_map<int,std::shared_ptr<Good>> goods_;
    int time_{ 20 };
    MapArray &map_;
public:
    GoodPool(MapArray &map):map_(map){
    }
    void AddGoods(std::shared_ptr<Good> good) {
        goods_[good->GetHash()] = good;
    }
    auto Search(int x, int y) -> std::shared_ptr<Good>;
    auto Size() { return goods_.size(); }
    void CleanInvalidCargo(int id);
    auto IsVlaid(Point point, int id) ->bool;
    // Point Find(Point start_point);

};