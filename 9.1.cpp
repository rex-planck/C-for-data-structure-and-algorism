#include <iostream>
#include <cstdio>

using namespace std;

const int MAXN = 100005;

// 定义二叉树节点 (静态链表形式)
// left 和 right 存储的是子节点在数组中的下标，-1 表示空
struct Node {
    int left;
    int right;
    int size;   // 子树节点数
    int height; // 子树高度
} tree[MAXN];

// ==========================================
// 手写简易栈 (用于非递归遍历)
// ==========================================
struct MyStack {
    int data[MAXN];
    int topIndex;

    MyStack() { topIndex = 0; }
    void push(int val) { data[topIndex++] = val; }
    int pop() { return data[--topIndex]; }
    bool empty() { return topIndex == 0; }
    int top() { return data[topIndex - 1]; }
    void clear() { topIndex = 0; }
};

// ==========================================
// 手写简易队列 (用于层次遍历)
// ==========================================
struct MyQueue {
    int data[MAXN];
    int head;
    int tail;

    MyQueue() { head = 0; tail = 0; }
    void push(int val) { data[tail++] = val; }
    int pop() { return data[head++]; }
    bool empty() { return head == tail; }
};

class BinaryTreeSolver {
public:
    int n;

    // 读取输入并构建树
    void build() {
        if (!(cin >> n)) return;
        for (int i = 1; i <= n; ++i) {
            cin >> tree[i].left >> tree[i].right;
        }
    }

    // 1. 前序遍历 (递归要求)
    void preOrderRecursive(int u, bool isRoot = false) {
        if (u == -1) return;
        
        // 格式控制：第一个数字前不加空格，后续加空格
        // 但为了简化逻辑，通常输出每个数后跟空格，或者手动控制
        // 这里为了严格匹配样例（通常行末空格是被允许的，但为了美观我们稍作控制）
        // 实际上简单的 cout << u << " "; 即可
        cout << u << " ";
        
        preOrderRecursive(tree[u].left);
        preOrderRecursive(tree[u].right);
    }

    // 2. 中序遍历 (非递归要求)
    void inOrderNonRecursive() {
        MyStack s;
        int curr = 1; // 根节点总是 1

        while (curr != -1 || !s.empty()) {
            // 一直向左走到底
            while (curr != -1) {
                s.push(curr);
                curr = tree[curr].left;
            }
            // 弹出栈顶
            curr = s.pop();
            cout << curr << " ";
            // 转向右子树
            curr = tree[curr].right;
        }
        cout << endl;
    }

    // 3. 后序遍历 (非递归要求)
    // 技巧：使用双栈法。
    // 栈1的遍历顺序是 根->左->右，改为 根->右->左 压入栈2
    // 栈2弹出顺序即为 左->右->根
    void postOrderNonRecursive() {
        MyStack s1, s2;
        s1.push(1); // 根节点

        while (!s1.empty()) {
            int u = s1.pop();
            s2.push(u);

            // 注意：先压左再压右，这样在s1弹出的顺序就是 先右后左
            // 放入s2的顺序就是 根->右->左
            if (tree[u].left != -1) s1.push(tree[u].left);
            if (tree[u].right != -1) s1.push(tree[u].right);
        }

        while (!s2.empty()) {
            cout << s2.pop() << " ";
        }
        cout << endl;
    }

    // 4. 层次遍历 (使用队列)
    void levelOrder() {
        MyQueue q;
        q.push(1); // 根节点

        while (!q.empty()) {
            int u = q.pop();
            cout << u << " ";

            if (tree[u].left != -1) q.push(tree[u].left);
            if (tree[u].right != -1) q.push(tree[u].right);
        }
        cout << endl;
    }

    // 计算子树大小和高度 (辅助函数，使用递归计算最简单)
    // 返回值无所谓，直接修改全局 tree 数组
    void calcMetrics(int u) {
        if (u == -1) return;

        int l = tree[u].left;
        int r = tree[u].right;

        // 后序遍历的思想：先算子节点，再算自己
        calcMetrics(l);
        calcMetrics(r);

        int l_size = (l == -1) ? 0 : tree[l].size;
        int r_size = (r == -1) ? 0 : tree[r].size;
        
        int l_height = (l == -1) ? 0 : tree[l].height;
        int r_height = (r == -1) ? 0 : tree[r].height;

        tree[u].size = l_size + r_size + 1;
        tree[u].height = (l_height > r_height ? l_height : r_height) + 1;
    }

    // 5 & 6. 输出统计数据
    void printMetrics() {
        // 先计算一遍所有节点的指标
        calcMetrics(1);

        // 输出节点数 (按节点编号 1~n 顺序)
        for (int i = 1; i <= n; ++i) {
            cout << tree[i].size << " ";
        }
        cout << endl;

        // 输出高度 (按节点编号 1~n 顺序)
        for (int i = 1; i <= n; ++i) {
            cout << tree[i].height << " ";
        }
        cout << endl;
    }
    
    // 封装的运行入口
    void solve() {
        build();
        
        // 1. 前序 (递归)
        preOrderRecursive(1);
        cout << endl;
        
        // 2. 中序 (非递归)
        inOrderNonRecursive();
        
        // 3. 后序 (非递归)
        postOrderNonRecursive();
        
        // 4. 层次
        levelOrder();
        
        // 5 & 6. 统计信息
        printMetrics();
    }
};

int main() {
    // 提高 IO 效率，量化/竞赛必备习惯
    ios::sync_with_stdio(false);
    cin.tie(0);

    BinaryTreeSolver solver;
    solver.solve();

    return 0;
}