#include <iostream>

// 定义常量
const int MAXN = 200005; // 最大点数
const int MAXM = 500005; // 最大边数

// ----------------------
// 1. 数据结构定义
// ----------------------
struct Edge {
    int u;  // 起点
    int v;  // 终点
    int w;  // 权重
};

Edge edges[MAXM]; // 存储所有边的数组
int parent[MAXN]; // 并查集数组

// ----------------------
// 2. 手动实现快速排序 (Quick Sort)
// ----------------------
// 替代 std::sort，时间复杂度 O(M log M)
void quick_sort(Edge arr[], int left, int right) {
    if (left >= right) return;

    int i = left;
    int j = right;
    int pivot = arr[(left + right) / 2].w; // 取中间值作为基准

    while (i <= j) {
        // 从左向右找第一个大于等于 pivot 的数
        while (arr[i].w < pivot) i++;
        // 从右向左找第一个小于等于 pivot 的数
        while (arr[j].w > pivot) j--;

        if (i <= j) {
            // 手动交换两个元素
            Edge temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }

    // 递归处理左右两部分
    if (left < j) quick_sort(arr, left, j);
    if (i < right) quick_sort(arr, i, right);
}

// ----------------------
// 3. 并查集 (DSU) 实现
// ----------------------
// 初始化并查集
void init_dsu(int n) {
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
    }
}

// 查找根节点 + 路径压缩
int find_root(int x) {
    if (parent[x] == x) {
        return x;
    }
    // 递归查找并进行路径压缩，直接指向根节点
    return parent[x] = find_root(parent[x]);
}

// 合并两个集合
// 返回 true 表示合并成功（之前不连通），返回 false 表示之前已连通
bool unite(int x, int y) {
    int rootX = find_root(x);
    int rootY = find_root(y);

    if (rootX != rootY) {
        parent[rootX] = rootY; // 将 x 的根指向 y 的根
        return true;
    }
    return false;
}

// ----------------------
// 4. 主程序
// ----------------------
int main() {
    // 提高 I/O 效率
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    if (!(std::cin >> n >> m)) return 0;

    // 读取边信息
    for (int i = 0; i < m; ++i) {
        std::cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    // 1. 对边按权重进行排序
    quick_sort(edges, 0, m - 1);

    // 2. 初始化并查集
    init_dsu(n);

    long long total_weight = 0; // 记录总权重，防止溢出用 long long
    int edges_count = 0;        // 记录已选择的边数

    // 3. Kruskal 核心循环
    for (int i = 0; i < m; ++i) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].w;

        // 如果 u 和 v 不在同一个集合中（不构成环）
        if (unite(u, v)) {
            total_weight += w;
            edges_count++;
            
            // 优化：如果是连通图，选够 n-1 条边即可停止
            if (edges_count == n - 1) break;
        }
    }

    std::cout << total_weight << std::endl;

    return 0;
}