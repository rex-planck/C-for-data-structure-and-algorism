#include <cstdio>  // 使用 scanf, printf 替代 cin, cout
#include <cstdlib> // 使用 malloc, free (虽然 new/delete 也可以，这里混合用不影响，但保持 C++ 风格用 new)

using namespace std;

/**
 * 结构体：MatrixTerm
 * 作用：存储稀疏矩阵的一个非零元 (行, 列, 值)
 * 先验收必问点：为什么用 struct 而不是 class？
  1. struct 默认 public，更适合存储数据
  2. struct 语义上更轻量，强调数据聚合
  matrixterm 有ROW，COL，VALUE三个成员变量
  用于存储稀疏矩阵中的非零元素的信息
  默认是不排序的，所以需要下文的 operator< 来排序
 */
struct MatrixTerm {
    //matrixterm is a struct representing a non-zero element in a sparse matrix
    int row;
    // row is the row index (1-based)
    int col;
    // col is the column index (1-based)
    int value;
    // value is the non-zero value at (row, col)

    // 重载小于号：用于排序，保证行主序 (Row Major)
    // 只有数据有序，后续的加法和乘法才能使用高效的线性扫描
    /*
    OPERATOR< 是这个结构体的一个成员函数，用于比较两个 MatrixTerm 对象的大小关系
    通过行优先级进行比较，如果行号不同则按行号比较
    他的语法结构是：
    bool operator<(const MatrixTerm& other) const
    其中 other 是另一个 MatrixTerm 对象的引用
    函数返回一个布尔值，表示当前对象是否小于 other 对象
    该函数被声明为 const，表示不会修改当前对象的状态
    这个函数的实现逻辑是：
    1. 首先比较当前对象的行号 row 和 other 对象的行号 other.row
       - 如果当前对象的行号小于 other 的行号，则返回 true，表示当前对象小于 other
       - 如果当前对象的行号大于 other 的行号，则返回 false，表示当前对象不小于 other
    2. 如果行号相同，则比较列号 col 和 other.col
       - 如果当前对象的列号小于 other 的列号，则返回 true
       - 否则返回 false
    通过这个比较函数，可以实现对 MatrixTerm 对象的排序，确保它们按照行优先级排列
    这对于稀疏矩阵的加法和乘法操作非常重要
    */
    bool operator<(const MatrixTerm& other) const {
        //operator< compares two MatrixTerm objects for sorting based on row proiority
        if (row != other.row) {
            return row < other.row;
        }
        return col < other.col;
    }
};



/**
 * 类：SparseMatrix
 * 作用：手动管理内存的稀疏矩阵类，不依赖 STL
 * 类相比于结构体更适合封装复杂行为和手动内存管理
 *  先验收必问点：为什么要手动管理内存？
  1. 避免 STL 容器的额外开销，提升性能
    2. 更好地控制内存布局，优化缓存利用
    这个自定义的 SparseMatrix 类用于表示一个稀疏矩阵
    它包含了矩阵的行数、列数、非零元个数、动态数组容量以及存储非零元的动态数组指针
    通过手动管理内存，可以更高效地处理大规模稀疏矩阵的运算
    其中的成员函数实现了矩阵的基本操作，如重置、加法、乘法、转置和输出
     MatrixTerm* data是一个指向MatrixTerm结构体数组的指针
     用于存储稀疏矩阵中的非零元素
     这样就实现了稀疏矩阵和非零元素之间的连接
      MatrixTerm* data的语法结构是：
        MatrixTerm* data;
        其中 MatrixTerm 是结构体的名称，* 表示这是一个指针类型，*之后的 data 是指针变量的名称，实现对非零元素数组的动态管理
 */
class SparseMatrix {
    //sparsematrix is a class representing a sparse matrix with manual memory management
public:
    int rows;       // 行数 n
    // row is the number of rows in the matrix
    int cols;       // 列数 m
    // col is the number of columns in the matrix
    int terms;      // 非零元个数 t
    // terms is the number of non-zero elements in the matrix
    int capacity;   // 动态数组容量
    // capacity is the capacity of the dynamic array
    MatrixTerm* data; // 动态数组指针
    // data is a pointer to the dynamic array of MatrixTerm
    /*
    class类相比struct结构体的优势在于：
    1. 类支持封装复杂行为，适合实现矩阵的各种操作
    2. 类可以包含成员函数，便于组织代码逻辑
    3. 类可以实现构造函数和析构函数，方便内存管理
    4. 类可以实现复制构造函数和赋值运算符重载，支持深拷贝
    5. 类可以隐藏实现细节，提供清晰的接口

    */
    // 构造函数
    /*
    这是 SparseMatrix 类的构造函数初始化器
    它用于创建一个空的稀疏矩阵对象，默认容量为 20
    构造函数的语法结构是：
    SparseMatrix(int cap = 20)
    其中 cap 是一个整数参数，表示动态数组的初始容量，默认为 20
    在构造函数体内，首先初始化矩阵的行数 rows、列数 cols 和非零元个数 terms 为 0
    然后将 capacity 设置为传入的参数 cap
    最后使用 new 运算符动态分配一个 MatrixTerm 类型的数组，大小为 capacity
    再将其赋值给 data 指针，实现对非零元数组的动态管理
    通过这个构造函数，可以创建一个空的稀疏矩阵对象，并为其分配初始的存储空间
    这样就为后续的矩阵操作做好了准备
    */
    SparseMatrix(int cap = 20) {
        // Initialize an empty sparse matrix with given capacity
        // so sparsematrix and maturixterm are connected
        //sparsemasmatrix is used to represent a sparse matrix
        //matrixtrem is used to represent a non-zero element in the sparse matrix
        rows = 0; cols = 0; terms = 0;
        //initialize rows, cols, and terms to 0
        capacity = cap;
        data = new MatrixTerm[capacity];
        //
    }

    // 析构函数：防止内存泄漏，验收必问点！
    /*
    这是 SparseMatrix 类的析构函数
    它用于释放稀疏矩阵对象占用的动态内存，防止内存泄漏
    析构函数的语法结构是：  
    ~SparseMatrix()
    它没有参数，也没有返回值
    在析构函数体内，首先检查 data 指针是否为 nullptr
    如果 data 不为 nullptr，说明之前分配了动态内存
    则使用 delete[] 运算符释放 data 指向的动态数组内存
    释放后，将 data 指针设置为 nullptr，避免悬空指针
    通过这个析构函数，可以确保当稀疏矩阵对象生命周期结束时
    占用的动态内存能够被正确释放，防止内存泄漏
    这是 C++ 中手动内存管理的一个重要环节
    delete[] 用于释放通过 new 分配的动态数组内存
    以确保程序的内存使用效率和稳定性

    */


    ~SparseMatrix() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }
    // 时间复杂度为 O(1)

    // 复制构造函数 (Deep Copy)
    /*
    这是 SparseMatrix 类的复制构造函数
    因为类默认是浅拷贝，所以需要手动实现深拷贝
    复制构造函数的语法结构是：
    SparseMatrix(const SparseMatrix& other)
    其中 other 是另一个 SparseMatrix 对象的引用
    在复制构造函数体内，首先将 other 对象的行数 rows、列数 cols、非零元个数 terms 和容量 capacity 复制到当前对象
    然后使用 new 运算符动态分配一个 MatrixTerm 类型的数组，大小为 capacity
    并将其赋值给 data 指针
    接着使用一个循环，将 other 对象的 data 数组中的每个 MatrixTerm 元素逐一复制到当前对象的 data 数组中
    此时DATA指向的是一个新的动态数组，存储了与OTHER对象相同的非零元信息
    OTHER对象的动态数组和当前对象的动态数组是独立的，互不影响
    & OTHER是传递引用，避免了不必要的拷贝开销
    */
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
    /*
    这是 SparseMatrix 类的赋值运算符重载函数
    它用于将一个 SparseMatrix 对象赋值给另一个 SparseMatrix 对象
    这与刚才的复制构造函数类似，也是为了实现深拷贝
    赋值运算符重载函数的语法结构是：
    SparseMatrix& operator=(const SparseMatrix& other)
    其中 other 是另一个 SparseMatrix 对象的引用
    在函数体内，首先检查当前对象是否与 other 对象是同一个对象
    如果是同一个对象，则直接返回当前对象的引用，避免自我赋值   
    if (this == &other) return *this;
    然后释放当前对象 data 指向的动态数组内存，防止内存泄漏，这里的DATA指向的是旧的动态数组
        delete[] data;
    接着将 other 对象的行数 rows、列数 cols、非零元个数 terms 和容量 capacity 复制到当前对象
    然后使用 new 运算符动态分配一个 MatrixTerm 类型的数组，大小为 capacity
    并将其赋值给 data 指针
    这里赋值运算重载与复制构造函数的区别在于
    赋值运算符重载需要先释放旧的动态数组内存
    接着使用一个循环，将 other 对象的 data 数组中的每个 MatrixTerm 元素逐一复制到当前对象的 data 数组中
    最后返回当前对象的引用 *this
    而复制构造函数则是在创建新对象时直接分配新的动态数组内存
    通过这个赋值运算符重载函数，可以确保当一个稀疏矩阵对象被赋值给另一个对象时
    占用的动态内存能够被正确管理，防止内存泄漏
    */
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
            //这里选择后删除旧数据，防止内存泄漏，更是因为要将旧数据复制到新数据中
            data = new_data;
            capacity = new_capacity;
        }
        data[terms].row = r;
        data[terms].col = c;
        data[terms].value = v;
        terms++;
        // 在末尾添加新元素
    }

    // 手写快速排序：替代 std::sort
    /*
    快速排序实现，用于对稀疏矩阵中的非零元进行排序
    通过排序，确保非零元按照行主序排列，便于后续的矩阵运算
    这里使用快速排序算法，平均时间复杂度为 O(T log T)
    快速排序的基本思想是选择一个基准元素
    空间复杂度为 O(log T) 的递归划分数组
    将小于基准的元素放在左边，大于基准的元素放
    */

    void quickSort(int l, int r) {
        if (l >= r) return;
        MatrixTerm pivot = data[(l + r) / 2]; // 选取中间值作为基准，避免最坏情况
        /*这里的DATA是一个动态数组，存储了稀疏矩阵的非零元
        PIVOT是选取的基准元素，用于划分数组
        二者之所以能够相等
        是因为PIVOT是DATA数组中的一个元素的副本
        DATA[(L + R) / 2]表示DATA数组中索引为(L + R) / 2的元素
        而MatrixTerm在这里的作用是定义PIVOT的类型
        使其与DATA数组中的元素类型一致
        L和R是当前排序区间的左右边界索引
        通过这个划分过程，可以将数组分成两部分
        小于基准的元素放在左边，大于基准的元素放在右边
        这样就为后续的递归排序做好了准备
        */

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
    /*
    这里运用的是递归思想
    通过递归调用 quicksort 函数，对划分后的子数组继续进行排序
    具体逻辑是：
    1. 首先检查当前排序区间是否有效 (l < r)
       - 如果无效，直接返回
    2. 选择基准元素 pivot，并初始化左右指针 i 和 j
    3. 使用双指针法进行划分
       - 左指针 i 向右移动，直到找到一个不小于基准的元素
       - 右指针 j 向左移动，直到找到一个不大于基准的元素
       - 如果 i <= j，则交换 data[i] 和 data[j]，然后移动指针
    4. 划分完成后，递归调用 quicksort 函数
       - 对左子数组 (l 到 j) 进行排序
         - 对右子数组 (i 到 r) 进行排序
    时间复杂度之所以是 O(T log T)
    是因为快速排序在平均情况下
    每次划分都能将数组大致分成两半
    递归深度为 log T
    每层递归需要 O(T) 的时间进行划分
    因此总体时间复杂度为 O(T log T)
    空间复杂度之所以是 O(log T)
    是因为递归调用栈的深度为 log T
    每次递归调用会占用一定的栈空间

    */

    // 核心操作 1：重置矩阵
    /*
    RESET函数用于初始化或重置稀疏矩阵对象的状态
    它接受四个参数：行数 N、列数 M、非零元个数 T 以及一个布尔标志 READ_FROM_FULL
    T表示非零元的数量
    READ_FROM_FULL表示是否从完整矩阵读取数据
    该函数首先将矩阵的行数、列数和非零元个数初始化为传入的参数值
    然后根据 READ_FROM_FULL 的值
    选择不同的读取方式
    如果 READ_FROM_FULL 为真
    则使用双重循环读取完整矩阵的每个元素
    对于非零元素，调用 APPEND 函数将其添加到稀疏矩阵中
    由于完整矩阵是按行顺序输入的
    因此添加的非零元天然有序
    无需额外排序
    如果 READ_FROM_FULL 为假
    则使用单循环读取 T 个三元组形式的非零元
    并调用 APPEND 函数添加到稀疏矩阵中
    由于三元组形式的输入可能无序
    需要在读取完成后调用 QUICKSORT 函数对非零元进行排序
    以确保它们按照行主序排列
    */
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
    // 时间复杂度是 O(N*M) 或 O(T log T)，取决于输入格式

    // 静态函数：读取 Q 矩阵
    /*
    STATIC是C++中的一个关键字
    用于声明类的静态成员函数或静态成员变量
    STATIC函数属于类本身，而不是类的某个对象
    因此可以在不创建类对象的情况下调用该函数
    这里的READ_Q函数是SparseMatrix类的一个静态成员函数
    它用于读取一个稀疏矩阵Q的输入数据
    函数首先声明一个SparseMatrix对象Q
    然后使用SCANF函数读取矩阵的行数N、列数M和非零元个数T
    如果读取失败，函数直接返回空的矩阵Q
    ！=3表示读取的参数数量不等于3，说明输入格式有误
    */
    static SparseMatrix read_Q() {
        SparseMatrix Q;
        int n, m, t;
        // 注意：这里用 scanf，如果读取失败会返回 -1 (EOF)健壮性
        if (scanf("%d%d%d", &n, &m, &t) != 3) {
            return Q;
            //直接返回原始矩阵Q，保持健壮性
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
            /*
            col_count: 统计每列元素个数
            col_start: 记录每列在转置后矩阵中的起始位置
            具体步骤如下：
            1. 初始化 col_count 数组为 0
            2. 遍历原矩阵的非零元，统计每列的元素个数，存入 col_count
            3. 计算 col_start 数组，表示每列在转置后矩阵中的起始位置
               - col_start[1] = 0
               - col_start[i] = col_start[i-1] + col_count[i-1] (i >= 2)
            4. 再次遍历原矩阵的非零元，根据 col_start 数组将元素放入转置后矩阵的正确位置
               - 对于每个非零元，找到其列 c
                - 计算放置位置 pos = col_start[c]
                - 将该非零元放入 Q.data[pos]
                - 更新 col_start[c]++
            5. 释放辅助数组内存
            通过这种方法，可以在 O(m + t) 的时间复杂度内完成矩阵的转置操作
            其中 m 是矩阵的列数，t 是非零元的个数
            */
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
    // 时间复杂度为 O(m + t)

    // 核心操作 3：加法
    // 复杂度：O(t1 + t2)，双指针归并
    int add(const SparseMatrix& Q) {
        if (rows != Q.rows || cols != Q.cols) {
            *this = Q;
            return -1;
        //使得函数健壮，维度不匹配时返回 -1 并保持原矩阵不变   
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
    // 时间复杂度为 O(t1 + t2)

    // 核心操作 2：乘法
    // 复杂度：优化后接近 O(T_result)，使用行索引加速
    int multiply(const SparseMatrix& Q_orig) {
        /*
        这里的MULTIPLY函数用于计算当前稀疏矩阵与另一个稀疏矩阵Q_ORIG的乘积
        并将结果存储在当前矩阵中
        */
        if (cols != Q_orig.rows) {
            /*
            矩阵乘法的维度不匹配，返回 -1 并保持原矩阵不变
            这里的COLS表示当前矩阵的列数
            而Q_ORIG.ROWS表示矩阵Q_ORIG的行数
            矩阵乘法的规则要求当前矩阵的列数必须等于Q_ORIG的行数
            如果不相等，说明无法进行矩阵乘法运算
            于是函数返回 -1，表示乘法操作失败
            */

            *this = Q_orig;
            /*
            这里的THIS是当前稀疏矩阵对象的指针
            通过THIS = Q_ORIG;这行代码
            将当前矩阵对象赋值为矩阵Q_ORIG的内容
            这样做的目的是在乘法操作失败时
            保持当前矩阵不变，直接将其内容替换为Q_ORIG的内容
            这样可以确保在乘法操作无法进行时
            当前矩阵仍然保持有效状态
            */
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

// 间复杂度接近 转置 B: $O(Cols_B + T_B)$。
// 构建行索引: $O(Rows_A + T_A + Cols_B + T_B)$。

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


