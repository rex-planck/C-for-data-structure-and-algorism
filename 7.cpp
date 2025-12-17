#include <iostream>

// 自定义循环队列类
class CardQueue {
private:
    int* array;      // 动态数组存储数据
    int capacity;    // 数组总容量
    int head;        // 队头索引
    int tail;        // 队尾索引（指向下一个空位）
    int count;       // 当前队列中的元素个数

public:
    // 构造函数：初始化队列
    // 为了防止溢出，容量设为 n + 1 (虽然循环队列 n 就够了，但多留一个空间便于判空/满逻辑，这里用 count 变量更直观)
    CardQueue(int n) {
        capacity = n + 1;
        array = new int[capacity];
        head = 0;
        tail = 0;
        count = 0;
    }

    // 析构函数：释放内存
    ~CardQueue() {
        delete[] array;
    }

    // 入队操作 (放到这叠牌的最后)
    void push(int val) {
        array[tail] = val;
        tail = (tail + 1) % capacity; // 循环移动 tail
        count++;
    }

    // 出队操作 (扔掉第一张牌)
    void pop() {
        if (count > 0) {
            head = (head + 1) % capacity; // 循环移动 head
            count--;
        }
    }

    // 获取队头元素 (查看第一张牌)
    int front() {
        return array[head];
    }

    // 获取当前队列大小
    int size() {
        return count;
    }
};

int main() {
    int n;
    // 输入 n
    if (!(std::cin >> n)) {
        return 0; 
    }

    // 1. 创建队列类
    CardQueue queue(n);

    // 初始化：将 1 到 n 依次放入队列
    for (int i = 1; i <= n; ++i) {
        queue.push(i);
    }

    // 2. 实现卡片游戏逻辑
    // 当至少还有两张牌的时候进行操作
    while (queue.size() >= 2) {
        // 操作 1: 把第一张牌扔掉
        queue.pop();

        // 操作 2: 把新的第一张牌（原第二张）放到整叠牌的最后
        int cardToMove = queue.front(); // 先把这张牌拿出来
        queue.pop();                    // 从队头移除
        queue.push(cardToMove);         // 放到队尾
    }

    // 输出最后剩下的一张牌
    std::cout << queue.front() << std::endl;

    return 0;
}