#include <iostream>

using namespace std;

const int MAXN = 100005;

int n;
int pre[MAXN]; // 存储前序序列
int in[MAXN];  // 存储中序序列
int pos[MAXN]; // 索引数组：pos[val] 表示数值 val 在中序序列中的下标

// 递归函数：处理前序区间 [preL, preR] 和 中序区间 [inL, inR]
// 目标是输出该区间的后序遍历
void solve(int preL, int preR, int inL, int inR) {
    // 递归基：区间无效时直接返回
    if (preL > preR || inL > inR) {
        return;
    }

    // 1. 前序遍历的第一个是根节点
    int rootVal = pre[preL];

    // 2. 找到根节点在中序遍历中的位置 (使用预处理的数组 O(1) 查找)
    int k = pos[rootVal];

    // 3. 计算左子树的节点数量
    // k 是根的下标，inL 是中序左边界，所以左边有 k - inL 个元素
    int leftLen = k - inL;

    // 4. 递归处理左子树
    // 前序范围：从根节点后移1位开始，长度为 leftLen
    // 中序范围：从原来的左边界到根节点前一位
    solve(preL + 1, preL + leftLen, inL, k - 1);

    // 5. 递归处理右子树
    // 前序范围：排除掉根和左子树剩下的部分
    // 中序范围：从根节点后一位到原来的右边界
    solve(preL + leftLen + 1, preR, k + 1, inR);

    // 6. 后序遍历要求：最后输出根节点
    cout << rootVal << " ";
}

int main() {
    // IO 加速，量化/竞赛大流量数据必备
    ios::sync_with_stdio(false);
    cin.tie(0);

    if (!(cin >> n)) return 0;

    // 读取前序
    for (int i = 1; i <= n; ++i) {
        cin >> pre[i];
    }

    // 读取中序，并记录每个值的位置
    for (int i = 1; i <= n; ++i) {
        cin >> in[i];
        // 核心优化：记录数值 in[i] 出现的位置是 i
        // 比如中序是 4 2 5...，则 pos[4]=1, pos[2]=2
        pos[in[i]] = i; 
    }

    // 开始递归求解
    // 初始范围：前序 1~n，中序 1~n
    solve(1, n, 1, n);

    cout << endl;

    return 0;
}