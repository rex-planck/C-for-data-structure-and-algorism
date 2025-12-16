#include <iostream>
#include <cstdio>

using namespace std;

// 定义最大节点数
const int MAXN = 100005;

// === 1. 手动实现邻接链表 ===
struct Edge {
    int to;
    Edge* next;
};

// head[u] 存储指向 u 的第一个邻居的指针
Edge* head[MAXN];
bool visited[MAXN]; // 访问标记数组
int dist[MAXN];     // 距离数组

// 初始化图
void init_graph(int n) {
    for (int i = 1; i <= n; i++) {
        head[i] = nullptr;
    }
}

// 插入边 (无向图，主函数需调用两次或在此处处理，题目输入只需处理一次调用)
// 注意：题目中的 0 u v 意为在 u 和 v 之间增加一条边，因为是无向图，我们要存 u->v 和 v->u
void add_edge_node(int u, int v) {
    Edge* node = new Edge;
    node->to = v;
    node->next = head[u];
    head[u] = node;
}

void insert_edge(int u, int v) {
    add_edge_node(u, v);
    add_edge_node(v, u);
}

// 删除边 u-v (同时删除 u->v 和 v->u)
void delete_one_direction(int u, int v) {
    Edge* curr = head[u];
    Edge* prev = nullptr;
    while (curr != nullptr) {
        if (curr->to == v) {
            if (prev == nullptr) {
                // 删除的是头节点
                head[u] = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            return; // 删除一个后即可返回（假设没有重复边）
        }
        prev = curr;
        curr = curr->next;
    }
}

void remove_edge(int u, int v) {
    delete_one_direction(u, v);
    delete_one_direction(v, u);
}

// === 2. 手动实现快速排序 (用于邻居排序) ===
void quick_sort(int arr[], int left, int right) {
    if (left >= right) return;
    int i = left, j = right;
    int pivot = arr[(left + right) / 2];
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }
    quick_sort(arr, left, j);
    quick_sort(arr, i, right);
}

// === 辅助函数：获取某点的所有邻居并排序 ===
// 返回邻居数量，结果存入 neighbors 数组
int get_sorted_neighbors(int u, int neighbors[]) {
    int count = 0;
    Edge* curr = head[u];
    while (curr != nullptr) {
        neighbors[count++] = curr->to;
        curr = curr->next;
    }
    if (count > 0) {
        quick_sort(neighbors, 0, count - 1);
    }
    return count;
}

// === 3. 算法实现 ===

// 全局变量用于存储遍历结果
int seq_dfs[MAXN], cnt_dfs = 0;
int seq_bfs[MAXN], cnt_bfs = 0;

// 清空访问标记
void reset_visited(int n) {
    for (int i = 0; i <= n; i++) visited[i] = false;
}

// DFS (递归版)
void dfs(int u) {
    visited[u] = true;
    seq_dfs[cnt_dfs++] = u;

    // 获取并排序邻居，保证字典序最小
    int neighbors[MAXN]; // 注意：如果在深层递归中，局部大数组可能爆栈。但一般OJ栈大小够用。
                         // 如果担心爆栈，可以用动态分配或全局辅助数组(需小心覆盖)。
                         // 考虑到邻居数通常不多，且总边数有限，这里简化处理。
    // 为了安全，更严谨的做法是动态开辟：
    int deg = 0;
    for(Edge* e = head[u]; e; e=e->next) deg++;
    int* nbrs = new int[deg];
    
    int idx = 0;
    for(Edge* e = head[u]; e; e=e->next) nbrs[idx++] = e->to;
    
    quick_sort(nbrs, 0, deg - 1);

    for (int i = 0; i < deg; i++) {
        int v = nbrs[i];
        if (!visited[v]) {
            dfs(v);
        }
    }
    delete[] nbrs;
}

// BFS (手写队列)
// 参数 t: 起点, s: 目标点(用于计算距离, 如果不需要设为-1)
// 返回 s 到 t 的距离
int bfs(int start_node, int target_node, int n) {
    int q[MAXN];
    int front = 0, rear = 0;
    
    // 初始化距离
    for(int i=0; i<=n; i++) dist[i] = -1;

    // 入队
    q[rear++] = start_node;
    visited[start_node] = true;
    dist[start_node] = 0;

    while (front < rear) {
        int u = q[front++];
        seq_bfs[cnt_bfs++] = u;

        // 获取并排序邻居
        int deg = 0;
        for(Edge* e = head[u]; e; e=e->next) deg++;
        int* nbrs = new int[deg];
        int idx = 0;
        for(Edge* e = head[u]; e; e=e->next) nbrs[idx++] = e->to;
        quick_sort(nbrs, 0, deg - 1);

        for (int i = 0; i < deg; i++) {
            int v = nbrs[i];
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                q[rear++] = v;
            }
        }
        delete[] nbrs;
    }
    
    if (target_node != -1) return dist[target_node];
    return -1;
}

// 简单的DFS用于标记连通分量 (不记录序列，仅用于标记)
void mark_component(int u) {
    visited[u] = true;
    Edge* curr = head[u];
    while (curr != nullptr) {
        if (!visited[curr->to]) {
            mark_component(curr->to);
        }
        curr = curr->next;
    }
}

int main() {
    // 提高cin/cout速度
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, s, t;
    if (!(cin >> n >> m >> s >> t)) return 0;

    init_graph(n);

    // 处理 m 次操作
    for (int i = 0; i < m; i++) {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 0) {
            insert_edge(u, v);
        } else {
            remove_edge(u, v);
        }
    }

    // === 输出 1 & 2: 连通分量数量及每个分量最小点 ===
    reset_visited(n);
    int component_count = 0;
    int min_nodes[MAXN];
    int min_nodes_cnt = 0;

    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            component_count++;
            min_nodes[min_nodes_cnt++] = i; // 因为i是从小到大遍历的，遇到的第一个未访问节点一定是该分量的最小值
            mark_component(i); // 将该连通分量所有点标记为已访问
        }
    }

    cout << component_count << endl;
    for (int i = 0; i < min_nodes_cnt; i++) {
        cout << min_nodes[i] << (i == min_nodes_cnt - 1 ? "" : " ");
    }
    cout << endl;

    // === 输出 3 & 4: 从 s 开始的 DFS ===
    reset_visited(n);
    cnt_dfs = 0;
    dfs(s);

    cout << cnt_dfs << endl;
    for (int i = 0; i < cnt_dfs; i++) {
        cout << seq_dfs[i] << (i == cnt_dfs - 1 ? "" : " ");
    }
    cout << endl;

    // === 输出 5 & 6 & 7: 从 t 开始的 BFS 以及 s 到 t 的距离 ===
    // 注意题目要求：
    // 第5,6行：从 t 开始的 BFS
    // 第7行：从 s 到 t 的最短路径
    // 无向图中，s->t 的距离等于 t->s 的距离。
    // 我们在做从 t 开始的 BFS 时，顺便可以计算出到 s 的距离。
    
    reset_visited(n);
    cnt_bfs = 0;
    int distance = bfs(t, s, n); // 从 t 开始跑 BFS，目标是 s

    cout << cnt_bfs << endl;
    for (int i = 0; i < cnt_bfs; i++) {
        cout << seq_bfs[i] << (i == cnt_bfs - 1 ? "" : " ");
    }
    cout << endl;

    cout << distance << endl;

    return 0;
}