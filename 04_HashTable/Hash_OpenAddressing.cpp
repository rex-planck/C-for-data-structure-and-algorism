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
    // 仅仅用于删除板块
    int get_dist(int from, int to) const {
        return (to - from + D) % D;
        // 环形缓冲区
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
        // 负数取模可能为负，调整为非负索引
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
            /*
            存在哈希冲突的现象，哈希值相同但被迫后移元素
            用于解决冲突哈希值被迫后移，但是删除前一个与之相同的哈希值后进行前移
            但是只能前移到理想位置 k，不能前移到 hole 位置，否则会破坏其他元素的查找路径
            并且只能解决冲突导致的后移，不能无限制前移
            */ 
            // 如果空洞 hole 位于理想位置 k 和当前位置 j 之间
            if (get_dist(k, hole) < get_dist(k, j)) {
                /*
                
                */ 
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
// 全部函数的时空复杂度分析：
/*
1. 构造函数和析构函数
   - 时间复杂度：O(D) 初始化数组
   - 空间复杂度：O(D) 存储数组
2. 查询函数 (find)
   - 时间复杂度：O(D) 最坏情况下需要遍历整个表
    - 空间复杂度：O(1) 使用固定数量的变量
3. 插入函数 (insert)
    - 时间复杂度：O(D) 最坏情况下需要遍历整个表以找到插入位置
    - 空间复杂度：O(1) 使用固定数量的变量       
4. 删除函数 (remove)
    - 时间复杂度：O(D) 最坏情况下需要遍历整个表以重     
    排删除
    - 空间复杂度：O(1) 使用固定数量的变量   
总体来看，哈希表的各项操作在平均情况下时间复杂度接近 O(1)，但在最坏情况下可能退化到 O(D)。空间复杂度为 O(D)，主要用于存储哈希表的数据。
*/