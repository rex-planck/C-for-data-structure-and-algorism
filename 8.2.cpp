#include <iostream>

using namespace std;

// 1. 定义链表节点结构体
struct Node {
    int val;
    Node* next;
    
    Node(int v) : val(v), next(nullptr) {}
};

// 2. 哈希表类
class LinkedHashTable {
private:
    // 桶结构：包含头指针和链表当前长度
    // 维护长度是为了满足题目要求 O(1) 时间输出长度，避免每次遍历计数
    struct Bucket {
        Node* head;
        int length;
    };

    Bucket* buckets; // 桶数组
    int D;           // 除数

    // 哈希函数：处理负数情况
    int hash(int key) const {
        return (key % D + D) % D;
    }

public:
    LinkedHashTable(int divisor) : D(divisor) {
        buckets = new Bucket[D];
        for (int i = 0; i < D; ++i) {
            buckets[i].head = nullptr;
            buckets[i].length = 0;
        }
    }

    // 析构函数：释放所有节点和桶数组
    ~LinkedHashTable() {
        for (int i = 0; i < D; ++i) {
            Node* curr = buckets[i].head;
            while (curr != nullptr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        delete[] buckets;
    }

    // 插入操作 (opt 0)
    void insert(int x) {
        int idx = hash(x);
        
        // 1. 检查是否存在
        Node* curr = buckets[idx].head;
        while (curr != nullptr) {
            if (curr->val == x) {
                cout << "Existed" << endl;
                return;
            }
            curr = curr->next;
        }

        // 2. 头插法插入新节点 (效率 O(1))
        Node* newNode = new Node(x);
        newNode->next = buckets[idx].head;
        buckets[idx].head = newNode;
        buckets[idx].length++;
        // 题目未要求插入成功后输出，保持静默
    }

    // 查询操作 (opt 1)
    void query(int x) {
        int idx = hash(x);
        
        Node* curr = buckets[idx].head;
        while (curr != nullptr) {
            if (curr->val == x) {
                // 题目要求：输出 x 所在链表的长度
                cout << buckets[idx].length << endl;
                return;
            }
            curr = curr->next;
        }
        cout << "Not Found" << endl;
    }

    // 删除操作 (opt 2)
    void remove(int x) {
        int idx = hash(x);
        
        Node* curr = buckets[idx].head;
        Node* prev = nullptr;

        while (curr != nullptr) {
            if (curr->val == x) {
                // 找到目标，开始删除
                if (prev == nullptr) {
                    // 删除的是头节点
                    buckets[idx].head = curr->next;
                } else {
                    // 删除的是中间或尾部节点
                    prev->next = curr->next;
                }
                
                delete curr;
                buckets[idx].length--;
                
                // 题目要求：输出删除后链表的长度
                cout << buckets[idx].length << endl;
                return;
            }
            // 继续遍历
            prev = curr;
            curr = curr->next;
        }
        cout << "Delete Failed" << endl;
    }
};

int main() {
    // 优化 I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int D, m;
    if (cin >> D >> m) {
        LinkedHashTable ht(D);
        for (int i = 0; i < m; ++i) {
            int opt, x;
            cin >> opt >> x;
            if (opt == 0) {
                ht.insert(x);
            } else if (opt == 1) {
                ht.query(x);
            } else if (opt == 2) {
                ht.remove(x);
            }
        }
    }
    return 0;
}