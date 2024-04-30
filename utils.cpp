#include <algorithm>

#include "utils.h"

// 归一化函数
auto normalize(const std::vector<int> &data) -> std::vector<double>
{
    int min_ = data[0];
    int max_ = data[0];
    for (auto x : data)
    {
        min_ = std::min(x, min_);
        max_ = std::max(x, max_);
    }
    std::vector<double> normalized_data;
    for (int value : data)
    {
        double normalized_value = static_cast<double>(value - min_) / (max_ - min_);
        normalized_data.push_back(normalized_value);
    }
    return normalized_data;
}

// 计算指标函数
std::vector<double> calculate_indicator(const std::vector<double> &first_column, const std::vector<double> &second_column, double weight_first, double weight_second)
{
    std::vector<double> indicator;
    for (size_t i = 0; i < first_column.size(); ++i)
    {
        indicator.push_back(weight_first * first_column[i] - weight_second * second_column[i]);
    }
    return indicator;
}

auto findTopFiveIndices(const std::vector<double> &nums) -> std::vector<int>
{
    // 创建一个包含数组下标的数组
    std::vector<int> indices(nums.size());
    for (int i = 0; i < nums.size(); ++i)
    {
        indices[i] = i;
    }

    // 对下标数组进行排序，排序规则为根据 nums 中的值降序排列
    std::sort(indices.begin(), indices.end(), [&nums](int a, int b)
              { return nums[a] > nums[b]; });

    // 返回数组中最大的五个数所在的下标
    return std::vector<int>(indices.begin(), indices.begin() + 5);
}

auto findTopFiveIndices(const std::array<int, 10> transport_times, const std::array<int, 10> loading_speeds, int boat_capacity) -> std::array<int, 5>{
    std::array<int, 5> five_berth_index;
    std::array<int, 10> indices;
    for (int i = 0; i < 10; ++i)
    {
        indices[i] = i;
    }
    std::array<int, 10> indicator;
    for(int i = 0; i < 10; i++){
        indicator[i] =  2 * transport_times[i] + boat_capacity / loading_speeds[i] + 1;
    }

    // 对下标数组进行排序，排序规则为根据 nums 中的值降序排列
    std::sort(indices.begin(), indices.end(), [&indicator](int a, int b)
              { return indicator[a] < indicator[b]; });

    for(int i = 0; i < 5; i++){
        five_berth_index[i] = indices[i];
    }
    return five_berth_index;
}
auto GroupBerth(const std::array<int, 10> &berth_transort_time ,int ways) -> std::array<std::pair<int, int>, 5>
{
    // 创建一个包含数组下标的数组
    int n = berth_transort_time.size();
    std::array<int, 10> indices;
    for (int i = 0; i < n; ++i)
    {
        indices[i] = i;
    }
    // 对下标数组进行排序，排序规则为根据 nums 中的值降序排列
    std::sort(indices.begin(), indices.end(), [&berth_transort_time](int a, int b)
              { return berth_transort_time[a] < berth_transort_time[b]; });

    std::array<std::pair<int, int>, 5> berth_pair;
    if (ways == 0)
    {
        int index = 0;
        int i = 0, j = n - 1;
        while (i < j)
        {
            berth_pair[index++] = std::make_pair(indices[i++], indices[j--]);
        }
    }
    else
    {
        int index = 0;
        int i = 0;
        while (i < 10)
        {
            berth_pair[index++] = std::make_pair(indices[i++], indices[i++]);
        }
    }

    return berth_pair;
}

// 匈牙利算法的实现
auto hungarianAlgorithm(const std::vector<std::vector<int>> &costMatrix, std::vector<int> &assignment) -> int
{
    int n = costMatrix.size();
    int m = costMatrix[0].size();

    std::vector<int> u(n + 1), v(m + 1), p(m + 1), way(m + 1);
    for (int i = 1; i <= n; ++i)
    {
        p[0] = i;
        int j0 = 0;
        std::vector<int> minv(m + 1, INF);
        std::vector<bool> used(m + 1, false);
        do
        {
            used[j0] = true;
            int i0 = p[j0], delta = INF, j1;
            for (int j = 1; j <= m; ++j)
            {
                if (!used[j])
                {
                    int cur = costMatrix[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j])
                    {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta)
                    {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= m; ++j)
            {
                if (used[j])
                {
                    u[p[j]] += delta;
                    v[j] -= delta;
                }
                else
                {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);

        do
        {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

    assignment.resize(n);
    for (int j = 1; j <= m; ++j)
    {
        if (p[j] != 0)
            assignment[p[j] - 1] = j - 1;
    }

    return -v[0]; // 返回最小总成本
}

auto CountofTrue(bool arr[], int size) -> int
{
    int ans = 0;
    for (int i = 0; i < size; i++)
    {
        if (arr[i])
            ans++;
    }
    return ans;
}

int get_max_num_berth_id(std::vector<int> & berth_good_num){
    int max_index = -1;
    int max_good_num = -1;
    int n = berth_good_num.size();
    for(int i = 0; i < n; i++){
        if(max_index == -1 || max_good_num < berth_good_num[i]){
            max_index = i;
            max_good_num = berth_good_num[i];
        }
    }
    return max_index;
}