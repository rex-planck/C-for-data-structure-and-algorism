#include <cstdio>  // 使用 scanf, printf 替代 cin, cout
#include <cstdlib> // 使用 malloc, free (虽然 new/delete 也可以，这里混合用不影响，但保持 C++ 风格用 new)

using namespace std;

/**
 * 结构体：MatrixTerm
 * 作用：存储稀疏矩阵的一个非零元 (行, 列, 值)
 */
struct MatrixTerm {
    int row;
    int col;
    int value;

    // 重载小于号：用于排序，保证行主序 (Row Major)
    // 只有数据有序，后续的加法和乘法才能使用高效的线性扫描
    bool operator<(const MatrixTerm& other) const {
        if (row != other.row) {
            return row < other.row;
        }
        return col < other.col;
    }
};

/**
 * 类：SparseMatrix
 * 作用：手动管理内存的稀疏矩阵类，不依赖 STL
 */
class SparseMatrix {
public:
    int rows;       // 行数 n
    int cols;       // 列数 m
    int terms;      // 非零元个数 t
    int capacity;   // 动态数组容量
    MatrixTerm* data; // 动态数组指针

    // 构造函数
    SparseMatrix(int cap = 20) {
        rows = 0; cols = 0; terms = 0;
        capacity = cap;
        data = new MatrixTerm[capacity];
    }

    // 析构函数：防止内存泄漏，验收必问点！
    ~SparseMatrix() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }

    // 复制构造函数 (Deep Copy)
    SparseMatrix(const SparseMatrix& other) {
        rows = other.rows;
        cols = other.cols;
        terms = other.terms;
        capacity = other.capacity;
        data = new MatrixTerm[capacity];
        for (int i = 0; i < terms; ++i) {
            data[i] = other.data[i];
        }
    }

    // 赋值运算符重载
    SparseMatrix& operator=(const SparseMatrix& other) {
        if (this == &other) return *this;
        delete[] data;
        rows = other.rows;
        cols = other.cols;
        terms = other.terms;
        capacity = other.capacity;
        data = new MatrixTerm[capacity];
        for (int i = 0; i < terms; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    // 手写 append：替代 vector.push_back，带自动扩容
    void append(int r, int c, int v) {
        if (terms >= capacity) {
            // 容量不足时，翻倍扩容
            int new_capacity = (capacity == 0) ? 10 : capacity * 2;
            MatrixTerm* new_data = new MatrixTerm[new_capacity];
            for (int i = 0; i < terms; ++i) new_data[i] = data[i];
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
        data[terms].row = r;
        data[terms].col = c;
        data[terms].value = v;
        terms++;
    }

    // 手写快速排序：替代 std::sort
    // 复杂度：O(T log T)
    void quickSort(int l, int r) {
        if (l >= r) return;
        MatrixTerm pivot = data[(l + r) / 2]; // 选取中间值作为基准，避免最坏情况
        int i = l, j = r;
        while (i <= j) {
            while (data[i] < pivot) i++;
            while (pivot < data[j]) j--;
            if (i <= j) {
                MatrixTerm temp = data[i];
                data[i] = data[j];
                data[j] = temp;
                i++; j--;
            }
        }
        if (l < j) quickSort(l, j);
        if (i < r) quickSort(i, r);
    }

    // 核心操作 1：重置矩阵
    void reset(int n, int m, int t, bool read_from_full = false) {
        rows = n;
        cols = m;
        terms = 0;
        
        if (read_from_full) {
            // 针对 Case 1：读取 n 行 m 列的全矩阵
            // 使用 scanf 极大提升速度
            for (int i = 1; i <= n; ++i) {
                for (int j = 1; j <= m; ++j) {
                    int val;
                    scanf("%d", &val); 
                    if (val != 0) {
                        append(i, j, val);
                    }
                }
            }
            // 全矩阵顺序读取，天生有序，无需排序
        } else {
            // 针对 Case 2/3 的 Q 矩阵：读取 t 个三元组
            for (int i = 0; i < t; ++i) {
                int r, c, v;
                scanf("%d%d%d", &r, &c, &v);
                append(r, c, v);
            }
            // 稀疏输入必须排序
            if (terms > 1) quickSort(0, terms - 1);
        }
    }

    // 静态函数：读取 Q 矩阵
    static SparseMatrix read_Q() {
        SparseMatrix Q;
        int n, m, t;
        // 注意：这里用 scanf，如果读取失败会返回 -1 (EOF)
        if (scanf("%d%d%d", &n, &m, &t) != 3) {
            return Q;
        }
        Q.reset(n, m, t, false); // Q 矩阵默认为稀疏格式输入
        return Q;
    }

    // 核心操作 5：转置
    // 复杂度：O(m + t)，使用辅助数组一次扫描完成，非常快
    SparseMatrix transpose() const {
        SparseMatrix Q(terms > 0 ? terms : 10);
        Q.rows = cols;
        Q.cols = rows;
        Q.terms = terms;

        if (terms > 0) {
            // 动态分配辅助数组 (替代 vector)
            int* col_count = new int[cols + 1];
            int* col_start = new int[cols + 1];
            
            // 初始化
            for(int i=0; i<=cols; ++i) col_count[i] = 0;
            
            // 统计每列元素个数
            for (int i = 0; i < terms; ++i) col_count[data[i].col]++;

            // 计算起始位置 (前缀和)
            col_start[1] = 0;
            for (int i = 2; i <= cols; ++i) col_start[i] = col_start[i-1] + col_count[i-1];
            
            // 放置元素
            for (int i = 0; i < terms; ++i) {
                int c = data[i].col;
                int pos = col_start[c];
                Q.data[pos].row = data[i].col;
                Q.data[pos].col = data[i].row;
                Q.data[pos].value = data[i].value;
                col_start[c]++;
            }

            delete[] col_count;
            delete[] col_start;
        }
        return Q;
    }

    // 核心操作 3：加法
    // 复杂度：O(t1 + t2)，双指针归并
    int add(const SparseMatrix& Q) {
        if (rows != Q.rows || cols != Q.cols) {
            *this = Q;
            return -1;
        }

        SparseMatrix R(terms + Q.terms);
        R.rows = rows;
        R.cols = cols;

        int i = 0, j = 0;
        while (i < terms && j < Q.terms) {
            if (data[i] < Q.data[j]) {
                R.append(data[i].row, data[i].col, data[i].value);
                i++;
            } else if (Q.data[j] < data[i]) {
                R.append(Q.data[j].row, Q.data[j].col, Q.data[j].value);
                j++;
            } else {
                long long sum = (long long)data[i].value + Q.data[j].value;
                if ((int)sum != 0) {
                    R.append(data[i].row, data[i].col, (int)sum);
                }
                i++; j++;
            }
        }
        while (i < terms) {
            R.append(data[i].row, data[i].col, data[i].value);
            i++;
        }
        while (j < Q.terms) {
            R.append(Q.data[j].row, Q.data[j].col, Q.data[j].value);
            j++;
        }
        
        *this = R;
        return 0;
    }

    // 核心操作 2：乘法
    // 复杂度：优化后接近 O(T_result)，使用行索引加速
    int multiply(const SparseMatrix& Q_orig) {
        if (cols != Q_orig.rows) {
            *this = Q_orig;
            return -1;
        }

        SparseMatrix R(terms); 
        R.rows = rows;
        R.cols = Q_orig.cols;

        // 对 Q 转置，便于连续访问内存 (Cache Friendly)
        SparseMatrix Q_trans = Q_orig.transpose();
        
        // 构建 P 的行索引 (类似 CSR 格式)
        int* p_row_start = new int[rows + 2];
        for(int k = 0; k <= rows + 1; ++k) p_row_start[k] = terms;
        for (int i = terms - 1; i >= 0; --i) p_row_start[data[i].row] = i;

        // 构建 Q_trans 的行索引
        int* q_row_start = new int[Q_trans.rows + 2];
        for(int k = 0; k <= Q_trans.rows + 1; ++k) q_row_start[k] = Q_trans.terms;
        for (int i = Q_trans.terms - 1; i >= 0; --i) q_row_start[Q_trans.data[i].row] = i;

        // 计算乘积
        for (int i = 1; i <= rows; ++i) { 
            int p_idx_end = p_row_start[i + 1];
            // 优化：如果 P 的当前行为空，直接跳过
            if (p_row_start[i] == p_idx_end) continue; 

            for (int j = 1; j <= Q_orig.cols; ++j) {
                int q_idx_end = q_row_start[j + 1];
                // 优化：如果 Q 的当前列为空，直接跳过
                if (q_row_start[j] == q_idx_end) continue; 

                long long dot_product = 0;
                int p_idx = p_row_start[i];
                int q_idx = q_row_start[j];

                // 双指针计算点积
                while (p_idx < p_idx_end && q_idx < q_idx_end) {
                    int p_col = data[p_idx].col;
                    int q_col = Q_trans.data[q_idx].col; 

                    if (p_col < q_col) {
                        p_idx++;
                    } else if (q_col < p_col) {
                        q_idx++;
                    } else {
                        dot_product += (long long)data[p_idx].value * Q_trans.data[q_idx].value;
                        p_idx++;
                        q_idx++;
                    }
                }
                
                if ((int)dot_product != 0) {
                    R.append(i, j, (int)dot_product);
                }
            }
        }

        delete[] p_row_start;
        delete[] q_row_start;

        *this = R;
        return 0;
    }

    // 核心操作 4：输出
    // 使用 printf 极大提升大量数据输出时的速度，解决 TLE 关键
    void output() const {
        printf("%d %d\n", rows, cols);

        int curr = 0; 
        for (int i = 1; i <= rows; ++i) {
            for (int j = 1; j <= cols; ++j) {
                int value = 0;
                if (curr < terms && data[curr].row == i && data[curr].col == j) {
                    value = data[curr].value;
                    curr++; 
                }
                // 输出数字，行末换行，中间空格
                if (j == cols) {
                    printf("%d\n", value);
                } else {
                    printf("%d ", value);
                }
            }
        }
    }
};

int main() {
    int w;
    // 使用 scanf 读取指令组数
    if (scanf("%d", &w) != 1) return 0;

    SparseMatrix P;
    int op_code;

    for (int k = 0; k < w; ++k) {
        if (scanf("%d", &op_code) != 1) break;

        switch (op_code) {
            case 1: { 
                int n, m;
                // 针对你的特殊 Case 1：读取 n, m 后紧接全矩阵
                if (scanf("%d%d", &n, &m) != 2) break;
                P.reset(n, m, 0, true); 
                break;
            }
            case 2: { // P = P * Q
                SparseMatrix Q = SparseMatrix::read_Q();
                if (P.multiply(Q) == -1) {
                    printf("-1\n");
                }
                break;
            }
            case 3: { // P = P + Q
                SparseMatrix Q = SparseMatrix::read_Q();
                if (P.add(Q) == -1) {
                    printf("-1\n");
                }
                break;
            }
            case 4: { // Output
                P.output();
                break;
            }
            case 5: { // Transpose
                P = P.transpose();
                break;
            }
            default:
                break;
        }
    }

    return 0;
}