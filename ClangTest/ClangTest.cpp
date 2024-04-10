#include <range/v3/view/enumerate.hpp>

#include <iostream>
#include <stack>
#include <vector>

class StockSpanner {
public:
    StockSpanner() {
        this->stk.emplace(-1, INT_MAX);
        this->idx = -1;
    }

    int next(int price) {
        idx++;
        while (price >= stk.top().second) {
            stk.pop();
        }
        const int ret = idx - stk.top().first;
        stk.emplace(idx, price);
        return ret;
    }

private:
    std::stack<std::pair<int, int>> stk;
    int                             idx;
};

/**
 * Your StockSpanner object will be instantiated and called as such:
 * StockSpanner* obj = new StockSpanner();
 * int param_1 = obj->next(price);
 */

int main() {
    std::vector<int> v = {100, 80, 60, 70, 60, 75, 85};
    std::vector<int> c = {1, 1, 1, 2, 1, 4, 6};

    auto *obj = new StockSpanner();

    for (auto [i, e] : v | ranges::views::enumerate) {
        if (const int ans = obj->next(e); c[i] != ans) {
            std::cout << "error at " << i << ", expected: " << c[i] << ", got: " << ans << std::endl;
        }
    }
}