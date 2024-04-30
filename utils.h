#pragma once
#include <vector>
#include <limits>
#include <array>

const int INF = std::numeric_limits<int>::max();
auto normalize(const std::vector<int> &data) -> std::vector<double>;

std::vector<double> calculate_indicator(const std::vector<double> &first_column, const std::vector<double> &second_column, double weight_first = 0.5, double weight_second = 0.5);

auto findTopFiveIndices(const std::vector<double> &nums) -> std::vector<int>;

auto findTopFiveIndices(const std::array<int, 10> transport_times, const std::array<int, 10> loading_speeds, int boat_capacity) -> std::array<int, 5>;
auto hungarianAlgorithm(const std::vector<std::vector<int>> &costMatrix, std::vector<int> &assignment) ->int;

auto GroupBerth(const std::array<int, 10>& berth_transort_time, int ways) -> std::array<std::pair<int, int>, 5>; 

auto CountofTrue(bool arr[], int size) ->int;

int get_max_num_berth_id(std::vector<int> & berth_good_num);