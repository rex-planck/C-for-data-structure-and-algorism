#include <iostream>

using namespace std;

// 针对数据结构学习，手动管理内存并实现散列表
class HashTable {
private:
    int* table;      // 存储键值
    bool* occupied;   // 标记位置是否被占用
    int D;            // 除数
    int size;         // 当前元素个数

    // 计算循环数组中从 from 到 to 的步数（距离）
    int get_dist(int from, int to) const {
        return (to - from + D) % D;
    }

public:
    HashTable(int divisor) : D(divisor), size(0) {
        table = new int[D];
        occupied = new bool[D];
        for (int i = 0; i < D; ++i) occupied[i] = false;
    }

    ~HashTable() {
        delete[] table;
        delete[] occupied;
    }

    // 查询：在线性探测中查找 x
    int find(int x) const {
        int start = (x % D + D) % D; // 处理负数情况
        int i = start;
        for (int step = 0; step < D; ++step) {
            if (!occupied[i]) return -1;  // 遇到空位，说明不存在
            if (table[i] == x) return i;  // 找到目标
            i = (i + 1) % D;
        }
        return -1;
    }

    // 插入
    void insert(int x) {
        if (find(x) != -1) {
            cout << "Existed" << "\n";
            return;
        }
        if (size >= D) return; // 容错：表满则无法插入

        int i = (x % D + D) % D;
        while (occupied[i]) {
            i = (i + 1) % D;
        }
        table[i] = x;
        occupied[i] = true;
        size++;
        cout << i << "\n";
    }

    // 删除：重排删除算法
    void remove(int x) {
        int i = find(x);
        if (i == -1) {
            cout << "Not Found" << "\n";
            return;
        }

        int moveCount = 0;
        occupied[i] = false; // 制造初始空洞
        size--;
        int hole = i;
        int j = (i + 1) % D;

        // 检查后续连续的元素块
        while (occupied[j]) {
            int k = (table[j] % D + D) % D; // 该元素的理想位置
            
            // 如果空洞 hole 位于理想位置 k 和当前位置 j 之间
            if (get_dist(k, hole) < get_dist(k, j)) {
                table[hole] = table[j];
                occupied[hole] = true;
                occupied[j] = false;
                hole = j; // 新的空洞出现在 j
                moveCount++;
            }
            j = (j + 1) % D;
        }
        cout << moveCount << "\n";
    }
};

int main() {
    // 性能优化：在量化开发或 OJ 题目中，关闭同步流可大幅提升速度，解决 TLE
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int D, m;
    if (!(cin >> D >> m)) return 0;

    HashTable ht(D);
    for (int i = 0; i < m; ++i) {
        int opt, x;
        cin >> opt >> x;
        if (opt == 0) ht.insert(x);
        else if (opt == 1) cout << ht.find(x) << "\n";
        else if (opt == 2) ht.remove(x);
    }

    return 0;
}