#include <iostream>
#include <cstdio> // 使用 scanf/printf，处理大数据量时比 cin/cout 快很多

using namespace std;

// -------------------- 数据结构定义 --------------------

// 定义树节点结构体
struct Node {
    int val;        // 节点存储的数值
    int size;       // 以该节点为根的子树的总节点数（用于计算排名 Rank）
    Node *left;     // 左孩子指针
    Node *right;    // 右孩子指针
};

// -------------------- 内存池 (Memory Pool) --------------------
// 题目数据量 m <= 1,000,000，频繁使用 new/delete 会非常慢且容易产生内存碎片导致超时。
// 这里使用静态数组模拟内存分配，称为“内存池”。

const int MAXN = 1000005; // 最大节点数，稍微开大一点防止溢出
Node pool[MAXN];          // 预先分配好的节点数组
int pool_ptr = 0;         // 当前已使用的节点下标指针

// 辅助函数：从内存池中分配一个新节点
// 相当于替代了 new Node(v)
Node* newNode(int v) {
    pool_ptr++; // 指针后移，占用一个新的位置
    pool[pool_ptr].val = v;
    pool[pool_ptr].size = 1;       // 新节点初始大小为 1（它自己）
    pool[pool_ptr].left = nullptr; // 初始无左右孩子
    pool[pool_ptr].right = nullptr;
    return &pool[pool_ptr];        // 返回该节点的地址
}

// -------------------- 全局状态变量 --------------------

// 用于记录单次操作中，路径上经过的节点值的异或和
// 题目要求：输出执行操作过程中依次比较的元素值的异或值。
int xor_path = 0;

// 标记操作是否成功
// 例如：插入重复值算失败，查找不存在的值算失败
bool success_flag = false;

// -------------------- 核心辅助函数 --------------------

// 向上更新节点的大小信息 (Maintain Size)
// 当子树发生插入或删除后，必须调用此函数更新当前节点的 size
void pushUp(Node* p) {
    if (!p) return; // 空节点不需要更新
    int ls = (p->left) ? p->left->size : 0;   // 左子树大小
    int rs = (p->right) ? p->right->size : 0; // 右子树大小
    p->size = ls + rs + 1; // 当前大小 = 左 + 右 + 自身(1)
}

// -------------------- 业务逻辑函数 --------------------

// 【操作 0】插入数值 (Insert)
// 参数 p: 当前节点指针的引用（引用允许我们修改指针指向）
// 参数 val: 要插入的值
void insert(Node* &p, int val) {
    // 1. 如果当前位置为空，说明找到了插入位置
    if (p == nullptr) {
        p = newNode(val);   // 创建新节点
        success_flag = true; // 标记插入成功
        // 注意：插入新节点本身不涉及“与该位置原有值的比较”（因为原位置是空），
        // 且题目样例暗示插入第一个数输出0，所以这里不更新 xor_path
        return;
    }

    // 2. 记录路径异或值
    // 只要进入这个节点进行比较，就需要异或它的值
    xor_path ^= p->val;

    // 3. 递归查找插入位置
    if (val == p->val) {
        // 题目隐含规则：BST通常不存重复值
        // 如果已存在，标记失败，后续主函数会输出 0
        success_flag = false; 
        return;
    } else if (val < p->val) {
        insert(p->left, val); // 比当前小，往左插
    } else {
        insert(p->right, val); // 比当前大，往右插
    }

    // 4. 回溯更新
    // 如果插入成功了，路径上的节点 size 都要 +1
    if (success_flag) pushUp(p);
}

// 【操作 1】查找数值 (Search)
void search(Node* p, int val) {
    // 1. 走到空节点，说明没找到
    if (p == nullptr) {
        success_flag = false;
        return;
    }

    // 2. 题目要求：待查询的元素也需要异或入答案中
    // 只要访问该节点，就计入异或路径
    xor_path ^= p->val;

    // 3. 比较逻辑
    if (val == p->val) {
        success_flag = true; // 找到了
        return;
    } else if (val < p->val) {
        search(p->left, val);
    } else {
        search(p->right, val);
    }
}

// -------------------- 删除相关的复杂逻辑 --------------------

// 辅助函数：找到并移除子树中的最小值节点
// 作用：用于双子节点删除时的“后继替换”策略
// 注意：题目特别说明“删除操作的替换过程中所有比较操作不计入答案”
// 所以这个函数里没有 xor_path 的操作
int removeMin(Node* &p) {
    // 一直往左走，直到没有左孩子，这就是最小值
    if (p->left == nullptr) {
        int minVal = p->val; // 记录下这个最小值
        p = p->right;        // 用它的右孩子顶替它的位置（断开连接）
        // 这里不需要 pushUp，因为 p 指针已经变了，上一层回溯时会更新 size
        return minVal;       // 返回最小值给上层使用
    }
    // 递归向左找
    int val = removeMin(p->left);
    pushUp(p); // 回溯时更新 size
    return val;
}

// 辅助函数：执行节点的物理删除
// 当我们在 remove 函数中找到了要删除的节点 p 时，调用此函数进行拓扑结构调整
void deleteNodeDetails(Node* &p) {
    // 情况1：叶子节点（无孩子）
    if (!p->left && !p->right) {
        p = nullptr; // 直接变为空
    }
    // 情况2：只有左孩子
    else if (!p->left) {
        p = p->right; // 右孩子上位
    }
    // 情况3：只有右孩子
    else if (!p->right) {
        p = p->left; // 左孩子上位
    }
    // 情况4：有两个孩子 (最复杂的情况)
    else {
        // 题目明确要求：如果有两个孩子，替换为“右子树中最小的”
        // removeMin 负责把右子树最小的节点挖出来，并返回其值
        int minVal = removeMin(p->right);
        p->val = minVal; // 将当前节点的值替换为后继值
        // 树结构变了（右子树少了一个节点），更新当前节点大小
        pushUp(p);
    }
}

// 【操作 2】删除数值 (Delete by Value)
void remove(Node* &p, int val) {
    if (p == nullptr) {
        success_flag = false; // 没找到要删的数
        return;
    }

    xor_path ^= p->val; // 计入路径

    if (val == p->val) {
        success_flag = true;
        deleteNodeDetails(p); // 找到目标，执行物理删除
        return;
    } else if (val < p->val) {
        remove(p->left, val);
    } else {
        remove(p->right, val);
    }

    // 回溯更新 size
    if (success_flag && p) pushUp(p);
}

// 【操作 3】按名次查找 (Search by Rank)
// k: 第 k 小的数
void searchByRank(Node* p, int k) {
    if (p == nullptr) {
        success_flag = false;
        return;
    }

    xor_path ^= p->val; // 计入路径

    // 当前节点的左子树大小
    int leftSize = (p->left) ? p->left->size : 0;
    
    // 判断 k 落在哪一边
    // 当前节点的排名是 leftSize + 1
    if (k == leftSize + 1) {
        success_flag = true; // 找到了
        return;
    } else if (k <= leftSize) {
        // 目标在左子树，名次 k 不变
        searchByRank(p->left, k);
    } else {
        // 目标在右子树
        // 在右子树中的新名次 = 原名次 - (左子树数量 + 根节点自己)
        searchByRank(p->right, k - leftSize - 1);
    }
}

// 【操作 4】按名次删除 (Delete by Rank)
void removeByRank(Node* &p, int k) {
    if (p == nullptr) {
        success_flag = false;
        return;
    }

    xor_path ^= p->val; // 计入路径

    int leftSize = (p->left) ? p->left->size : 0;

    if (k == leftSize + 1) {
        success_flag = true;
        deleteNodeDetails(p); // 找到目标，直接删除
        return;
    } else if (k <= leftSize) {
        removeByRank(p->left, k);
    } else {
        removeByRank(p->right, k - leftSize - 1);
    }

    if (success_flag && p) pushUp(p);
}


// -------------------- 主函数 --------------------

int main() {
    // 初始化根节点为空
    Node* root = nullptr;
    
    int m;
    // 读入操作总数 m
    if (scanf("%d", &m) != 1) return 0;

    while (m--) {
        int op, val;
        // 读入 操作类型 和 操作数
        scanf("%d %d", &op, &val);

        // 每次操作前，重置全局状态
        xor_path = 0;
        success_flag = false;

        if (op == 0) { // --- 插入 ---
            insert(root, val);
            // 逻辑说明：
            // 1. 如果 success_flag 为 false，说明插入了重复值 -> 题目要求输出 0
            // 2. 如果 success_flag 为 true，输出 xor_path
            // 注意：如果是空树插入第一个节点，xor_path 为 0，也是正确的
            if (!success_flag) printf("0\n");
            else printf("%d\n", xor_path);
        }
        else if (op == 1) { // --- 查找 ---
            search(root, val);
            // 如果没找到 (flag=false)，输出 0
            if (!success_flag) printf("0\n");
            else printf("%d\n", xor_path);
        }
        else if (op == 2) { // --- 删除 ---
            remove(root, val);
            if (!success_flag) printf("0\n");
            else printf("%d\n", xor_path);
        }
        else if (op == 3) { // --- 按名次查找 ---
            // 边界检查：如果 k 超过了树的总节点数，显然不存在
            if (root && val > root->size) {
                printf("0\n");
            } else {
                searchByRank(root, val);
                if (!success_flag) printf("0\n");
                else printf("%d\n", xor_path);
            }
        }
        else if (op == 4) { // --- 按名次删除 ---
            // 边界检查
            if (root && val > root->size) {
                printf("0\n");
            } else {
                removeByRank(root, val);
                if (!success_flag) printf("0\n");
                else printf("%d\n", xor_path);
            }
        }
    }

    return 0;
}