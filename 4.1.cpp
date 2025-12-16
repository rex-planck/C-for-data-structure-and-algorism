#include <iostream>

using namespace std;

// 定义链表节点结构体
struct Node {
    int val;
    Node* next;
    
    Node(int v) : val(v), next(nullptr) {}
};

class LinkedList {
private:
    Node* dummyHead; // 虚拟头节点，方便操作

public:
    LinkedList() {
        dummyHead = new Node(0); // 初始化虚拟头节点
    }

    // 尾部添加元素（用于初始化）
    void append(int val) {
        Node* cur = dummyHead;
        while (cur->next != nullptr) {
            cur = cur->next;
        }
        cur->next = new Node(val);
    }

    // 操作1: 在 idx 位置插入 val
    void insert(int idx, int val) {
        Node* cur = dummyHead;
        // 找到 idx 的前一个位置 (即移动 idx 次)
        for (int i = 0; i < idx && cur != nullptr; ++i) {
            cur = cur->next;
        }
        
        if (cur != nullptr) {
            Node* newNode = new Node(val);
            newNode->next = cur->next;
            cur->next = newNode;
        }
    }

 
    void remove(int val) {
        Node* cur = dummyHead;
        // 寻找 val 的前驱节点
        while (cur->next != nullptr) {
            if (cur->next->val == val) {
                Node* toDelete = cur->next;
                cur->next = toDelete->next;
                delete toDelete; // 释放内存
                return; // 只删除第一个，直接返回
            }
            cur = cur->next;
        }
        // 如果没找到
        cout << -1 << endl;
    }

    // 操作3: 原地逆置链表
    void reverse() {
        if (dummyHead->next == nullptr || dummyHead->next->next == nullptr) {
            return; // 空链表或只有一个节点，无需逆置
        }

        Node* prev = nullptr;
        Node* cur = dummyHead->next;
        Node* next = nullptr;

        while (cur != nullptr) {
            next = cur->next; // 保存下一个节点
            cur->next = prev; // 反转指针
            prev = cur;       // 步进 prev
            cur = next;       // 步进 cur
        }
        
        // 重新连接虚拟头节点到新的首节点（原尾节点）
        dummyHead->next = prev;
    }

    // 操作4: 查询 val 的索引
    void query(int val) {
        Node* cur = dummyHead->next;
        int idx = 0;
        while (cur != nullptr) {
            if (cur->val == val) {
                cout << idx << endl;
                return;
            }
            cur = cur->next;
            idx++;
        }
        cout << -1 << endl;
    }

    // 操作5: 计算异或和
    void calcXorSum() {
        Node* cur = dummyHead->next;
        long long sum = 0; // 使用 long long 防止溢出
        int idx = 0;
        
        while (cur != nullptr) {
            // 注意优先级：加法优先级高于异或，所以必须加括号 (idx ^ cur->val)
            sum += (idx ^ cur->val);
            cur = cur->next;
            idx++;
        }
        cout << sum << endl;
    }
    
    // 析构函数：释放所有内存
    ~LinkedList() {
        Node* cur = dummyHead;
        while (cur != nullptr) {
            Node* temp = cur;
            cur = cur->next;
            delete temp;
        }
    }
};

int main() {
    // 优化 I/O 速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, Q;
    if (!(cin >> N >> Q)) return 0;

    LinkedList list;
    
    // 初始化链表
    for (int i = 0; i < N; ++i) {
        int val;
        cin >> val;
        list.append(val);
    }

    // 处理 Q 个操作
    for (int i = 0; i < Q; ++i) {
        int op;
        cin >> op;
        
        if (op == 1) {
            int idx, val;
            cin >> idx >> val;
            list.insert(idx, val);
        } else if (op == 2) {
            int val;
            cin >> val;
            list.remove(val);
        } else if (op == 3) {
            list.reverse();
        } else if (op == 4) {
            int val;
            cin >> val;
            list.query(val);
        } else if (op == 5) {
            list.calcXorSum();
        }
    }

    return 0;
}