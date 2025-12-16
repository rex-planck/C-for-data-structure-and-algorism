#include <iostream>
#include <queue>
using namespace std;

int main() {
    int n;
    cin >> n;

    queue<int> cards;
    // 初始化牌堆
    for (int i = 1; i <= n; ++i) {
        cards.push(i);
    }

    // 模拟游戏
    while (cards.size() > 1) {
        // 扔掉第一张牌
        cards.pop();
        // 将下一张牌放到最后
        int temp = cards.front();
        cards.pop();
        cards.push(temp);
    }

    // 输出最后剩下的牌
    cout << cards.front() << endl;

    return 0;
}