#include <iostream>

// 定义常量
const int MAXN = 200005; // 最大点数
const int MAXM = 1000005; // 最大边数 (无向图边数 x 2)
const int INF = 0x3f3f3f3f;

// ----------------------
// 1. 链式前向星 (手动实现邻接表)
// ----------------------
struct Edge {
    int to;     // 边的终点
    int w;      // 边的权重
    int next;   // 指向下一条同起点的边的索引
    // 这里的NEXT不是指针，而是数组下标，具体是指向edges数组中的下一个边
    // 这样做是为了节省内存空间，避免使用指针带来的额外开销
};

Edge edges[MAXM];
int head[MAXN]; // head[u] 存储以 u 为起点的第一条边的索引
int edge_cnt = 0;

// 添加边函数
void add_edge(int u, int v, int w) {
    // U表示起点，V表示终点，W表示权重
    edge_cnt++;
    edges[edge_cnt].to = v;
    edges[edge_cnt].w = w;
    edges[edge_cnt].next = head[u];
    head[u] = edge_cnt;
}

// ----------------------
// 2. 手动实现最小堆 (Priority Queue)
// ----------------------
struct HeapNode {
    int u;  // 节点编号
    int d;  // 距离 (key)
    
    // 重载比较，用于逻辑判断
    bool operator<(const HeapNode& other) const {
        return d < other.d;
    }
};

HeapNode heap[MAXM]; // 堆的大小可能达到边数级别
int heap_size = 0;

// 上浮操作
void heap_push(int u, int d) {
    heap_size++;
    heap[heap_size] = {u, d};
    int curr = heap_size;
    while (curr > 1) {
        int parent = curr >> 1; // 除以2
        // 位运算效率更高
        if (heap[curr].d < heap[parent].d) { // 最小堆：子小于父则交换
            // 手动交换
            HeapNode temp = heap[curr];
            heap[curr] = heap[parent];
            heap[parent] = temp;
            curr = parent;
        } else {
            break;
        }
    }
}

// 下沉操作
HeapNode heap_pop() {
    HeapNode res = heap[1]; // 堆顶是最小值
    heap[1] = heap[heap_size];
    heap_size--;
    
    int curr = 1;
    while ((curr << 1) <= heap_size) {
        int child = curr << 1; // 左孩子
        // 如果右孩子存在且比左孩子更小，选择右孩子
        if (child < heap_size && heap[child + 1].d < heap[child].d) {
            child++;
        }
        // 如果孩子比当前节点小，交换
        if (heap[child].d < heap[curr].d) {
            HeapNode temp = heap[curr];
            heap[curr] = heap[child];
            heap[child] = temp;
            curr = child;
        } else {
            break;
        }
    }
    return res;
}

bool heap_empty() {
    return heap_size == 0;
}

// ----------------------
// 3. Prim 算法主逻辑
// ----------------------
int n, m;
int dis[MAXN];    // 存储节点到生成树的最小距离
bool vis[MAXN];   // 标记节点是否已加入生成树

long long prim() {
    // 初始化
    for (int i = 1; i <= n; ++i) {
        dis[i] = INF;
        vis[i] = false;
    }

    long long total_weight = 0;
    int edges_count = 0; // 记录加入生成树的边的数量

    // 从节点1开始
    dis[1] = 0;
    heap_push(1, 0);

    while (!heap_empty()) {
        HeapNode top = heap_pop();
        int u = top.u;
        int d = top.d;

        // 如果该点已经加入过生成树，跳过
        if (vis[u]) continue;

        // 将该点加入生成树
        vis[u] = true;
        total_weight += d;
        edges_count++;
        
        // 遍历 u 的所有邻居
        for (int i = head[u]; i != 0; i = edges[i].next) {
            int v = edges[i].to;
            int w = edges[i].w;

            // 如果 v 未访问，且新边权重小于 v 当前记录的最小距离
            if (!vis[v] && w < dis[v]) {
                dis[v] = w;
                heap_push(v, w); // 将更新后的距离加入堆
            }
        }
    }
    
    // 如果无法连通所有点 (可选检查)
    // if (edges_count < n) return -1; 

    return total_weight;
}

int main() {
    // 提高读写速度，虽然这里不是瓶颈
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    if (!(std::cin >> n >> m)) return 0;

    // 输入边
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        // 无向图，加双向边
        add_edge(u, v, w);
        add_edge(v, u, w);
    }

    std::cout << prim() << std::endl;

    return 0;
}