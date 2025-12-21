#include <iostream>
#include <cstdio>

class Chain {
private:
    struct Node {
        int data;
        Node* next;
        Node() : data(0), next(nullptr) {}
        Node(int x) : data(x), next(nullptr) {}
    };

    Node* head;
    Node* tail;  // 尾指针，优化pushBack

    static Node* mergeSortRec(Node* head);   // 归并排序递归主过程
    static Node* merge(Node* a, Node* b);    // 合并两个有序链表
    static void split(Node* source, Node*& front, Node*& back); // 链表拆分

public:
    Chain() : head(nullptr), tail(nullptr) {}

    void pushBack(int x) {
        Node* node = new Node(x);
    
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    void mergeSort() {
        head = mergeSortRec(head);
    }

    void print() const {
        Node* cur = head;
        bool first = true;
        while (cur) {
            if (!first) printf(" ");
            printf("%d", cur->data);
            first = false;
            cur = cur->next;
        }
        printf("\n");
    }
};

// ====================== 主函数 ======================
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
  
    int n;
    scanf("%d", &n);

    Chain chain;
    for (int i = 0; i < n; ++i) {
        int x;
        scanf("%d", &x);
        chain.pushBack(x);
    }

    chain.mergeSort();
    chain.print();

    return 0;
}

// ======= 需补全的部分，提交时只给出下面的代码 ========

Chain::Node* Chain::mergeSortRec(Node* head) {
    // 递归基：如果链表为空或只有一个节点，直接返回
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    Node* front = nullptr;
    Node* back = nullptr;

    // 1. 分割链表
    split(head, front, back);

    // 2. 递归排序两部分
    front = mergeSortRec(front);
    back = mergeSortRec(back);

    // 3. 合并有序链表
    return merge(front, back);
}

Chain::Node* Chain::merge(Node* a, Node* b) {
    // 边界情况处理
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    // 使用 dummy 节点简化头插逻辑 (避免处理 head 是否为空的特判)
    Node dummy; 
    Node* tail = &dummy;

    // 迭代比较两个链表的节点，将其挂在 tail 后面
    while (a != nullptr && b != nullptr) {
        if (a->data <= b->data) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next; // 尾指针后移
    }

    // 将剩余的链表直接接在后面
    if (a != nullptr) {
        tail->next = a;
    } else {
        tail->next = b;
    }

    return dummy.next;
}

void Chain::split(Node* source, Node*& front, Node*& back) {
    if (source == nullptr || source->next == nullptr) {
        front = source;
        back = nullptr;
        return;
    }

    // 快慢指针法找到中点
    Node* slow = source;
    Node* fast = source->next; // fast 先走一步，保证偶数个节点时 slow 在前半部分的最后一个

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // slow 指向前半部分的最后一个节点
    front = source;
    back = slow->next;
    
    // 断开链表
    slow->next = nullptr;
}
