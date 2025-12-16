#include <iostream>

using namespace std;

// --- 0. 辅助函数：不使用 STL 的排序 ---

// 手写交换函数，替代 std::swap
void manual_swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// 快速排序的分区函数
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // 选择最后一个元素作为基准
    int i = (low - 1);     // i 是较小元素的索引

    for (int j = low; j <= high - 1; j++) {
        // 如果当前元素小于基准
        if (arr[j] < pivot) {
            i++;
            manual_swap(arr[i], arr[j]);
        }
    }
    manual_swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// 手写快速排序，替代 std::sort
void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        // pi 是分区索引，arr[pi] 现在在正确的位置
        int pi = partition(arr, low, high);

        // 分别排序分区前后的元素
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

// --- 1. Node, Iterator, and LinkedList Class Definitions ---
// (链表类的定义保持不变，因为这部分本来就是手写的，没有用 STL 容器)

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int _size;

public:
    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        int& operator*() const { return current->data; }
        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    LinkedList() : head(nullptr), _size(0) {}

    ~LinkedList() { clear(); }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        _size = 0;
    }

    int size() const { return _size; }

    // 在尾部插入元素
    void push_back(int val) {
        Node* newNode = new Node(val);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        _size++;
    }

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};

// --- 2. Core Functions ---

/**
 * 合并两个有序链表
 */
LinkedList merge_sorted_lists(const LinkedList& L1, const LinkedList& L2) {
    LinkedList mergedList;
    LinkedList::Iterator it1 = L1.begin();
    LinkedList::Iterator it2 = L2.begin();

    while (it1 != L1.end() && it2 != L2.end()) {
        if (*it1 <= *it2) {
            mergedList.push_back(*it1);
            ++it1;
        } else {
            mergedList.push_back(*it2);
            ++it2;
        }
    }

    while (it1 != L1.end()) {
        mergedList.push_back(*it1);
        ++it1;
    }

    while (it2 != L2.end()) {
        mergedList.push_back(*it2);
        ++it2;
    }

    return mergedList;
}

/**
 * 计算链表的异或和
 */
long long calculate_xor_sum(const LinkedList& chain) {
    long long xor_sum = 0;
    int index = 0;

    for (LinkedList::Iterator it = chain.begin(); it != chain.end(); ++it) {
        xor_sum += (long long)index ^ (*it);
        index++;
    }

    return xor_sum;
}

// --- 3. Main Logic ---

int main() {
    // 优化 I/O，这属于 iostream 库，通常不被视为 STL 算法部分，可以保留
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    // **处理序列 1**
    // 替换 vector 为动态数组
    int* seq1 = new int[N];
    for (int i = 0; i < N; ++i) {
        cin >> seq1[i];
    }
    
    // 替换 std::sort 为手写 quick_sort
    quick_sort(seq1, 0, N - 1);
    
    LinkedList L1;
    for (int i = 0; i < N; ++i) {
        L1.push_back(seq1[i]);
    }
    // 释放动态数组内存
    delete[] seq1;
    
    // **处理序列 2**
    int* seq2 = new int[M];
    for (int i = 0; i < M; ++i) {
        cin >> seq2[i];
    }
    
    // 替换 std::sort 为手写 quick_sort
    quick_sort(seq2, 0, M - 1);

    LinkedList L2;
    for (int i = 0; i < M; ++i) {
        L2.push_back(seq2[i]);
    }
    delete[] seq2;

    // **合并链表**
    LinkedList L_merged = merge_sorted_lists(L1, L2);

    // **计算并输出异或和**
    cout << calculate_xor_sum(L1) << "\n";
    cout << calculate_xor_sum(L2) << "\n";
    cout << calculate_xor_sum(L_merged) << "\n";

    return 0;
}