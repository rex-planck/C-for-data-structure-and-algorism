#include <iostream>
#include <cstdio>

using namespace std;

// 全局数组模拟堆
// N=5000 是题目可能的最大规模，开大一点防止越界
// 这里的堆是"小顶堆"（Min-Heap），即堆顶元素是最小的
int heap[10005]; 
int heapSize = 0; // 记录当前堆中元素的个数

// 手写交换函数
// 参数使用引用 (int &a)，这样可以直接修改传入变量的内存值，避免数据拷贝，提高效率
void mySwap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

// 【上浮操作】 (Shift Up)
// 用于插入新元素后，将其移动到正确位置
// 逻辑：如果当前节点比它的父节点更小，就交换（维持小顶堆特性），直到不能交换为止
void shiftUp(int index) {
    while (index > 1) { // 只要不是根节点(下标1)，就有父节点
        int parent = index / 2; // 父节点的下标是当前下标除以2
        
        // 如果当前节点比父节点小，违反了小顶堆规则，需要交换
        if (heap[index] < heap[parent]) {
            mySwap(heap[index], heap[parent]);
            index = parent; // 指针向上移动，继续检查上一层
        } else {
            break; // 如果比父节点大（或相等），说明位置正确，停止上浮
        }
    }
}

// 【下沉操作】 (Shift Down)
// 用于删除堆顶或建堆时。将当前节点向下移动，保持堆的有序性
// 逻辑：找到左右孩子中更小的那个，如果当前节点比它大，就交换
void shiftDown(int index) {
    // 只要当前节点有左孩子 (index * 2 <= heapSize)，就继续循环
    while (index * 2 <= heapSize) {
        int child = index * 2; // 默认先看左孩子
        
        // 如果右孩子存在 (child + 1 <= heapSize) 且 右孩子比左孩子更小
        // 那么我们就应该跟右孩子比较（因为我们要选最小的浮上来）
        if (child + 1 <= heapSize && heap[child + 1] < heap[child]) {
            child++; // child 变成右孩子的下标
        }
        
        // 比较当前节点和较小的那个子节点
        if (heap[child] < heap[index]) {
            mySwap(heap[child], heap[index]); // 子节点更小，交换，让小的上来
            index = child; // 指针向下移动，继续检查下一层
        } else {
            break; // 如果当前节点比子节点都小，说明位置正确，停止下沉
        }
    }
}

// 【插入操作】
// 逻辑：先把新元素放到数组末尾，然后执行上浮操作
void insert(int val) {
    heapSize++;          // 堆的大小+1
    heap[heapSize] = val; // 放入数组末尾
    shiftUp(heapSize);    // 让新元素上浮到正确位置
}

// 【弹出堆顶】
// 逻辑：将堆顶元素用数组末尾元素覆盖，然后删除末尾，最后让新的堆顶下沉
void pop() {
    if (heapSize < 1) return; // 堆为空，直接返回
    
    heap[1] = heap[heapSize]; // 用最后一个元素覆盖堆顶（根节点）
    heapSize--;               // 堆大小减1（相当于删除了原来的堆顶）
    
    if (heapSize > 0) {
        shiftDown(1); // 让新的根节点下沉到正确位置
    }
}

// 【获取堆顶】
// 小顶堆的堆顶就是整个堆的最小值
int top() {
    return heap[1];
}

// 【建堆操作】 (Floyd建堆算法)
// 时间复杂度是 O(n)，比一个个插入 O(nlogn) 更快
void build(int nums[], int n) {
    heapSize = 0;
    // 1. 先将数据原样填入 heap 数组（从下标1开始填）
    for (int i = 0; i < n; i++) {
        heap[i + 1] = nums[i];
    }
    heapSize = n;
    
    // 2. 从最后一个"非叶子节点"开始，依次向前执行下沉操作
    // heapSize / 2 是最后一个非叶子节点的下标
    for (int i = heapSize / 2; i >= 1; i--) {
        shiftDown(i);
    }
}

int main() {
    // IO 加速：取消 C++ iostream 和 C stdio 的同步，加快 cin/cout 速度
    // 这不违反"不使用 STL 算法"的规则，只是加速输入输出
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    // 读取初始元素个数，如果读取失败直接退出
    if (!(cin >> n)) return 0;

    // 临时数组存储初始数据
    int initialNums[5005];
    for (int i = 0; i < n; i++) {
        cin >> initialNums[i];
    }

    // 1. 初始建堆
    build(initialNums, n);
    // 输出建堆后的最小值（堆顶）
    cout << top() << endl;

    int m;
    cin >> m; // 读取操作次数
    while (m--) {
        int op;
        cin >> op;
        
        if (op == 1) { 
            // 操作 1: 插入一个新数
            int num;
            cin >> num;
            insert(num);
            cout << top() << endl; // 输出当前的最小值
        } 
        else if (op == 2) {
            // 操作 2: 删除当前最小值（堆顶）
            pop();
            cout << top() << endl; // 输出删除后的新最小值
        } 
        else if (op == 3) {
            // 操作 3: 给定一组新数据，要求排序输出
            // 注意：这个操作利用了我们现有的堆逻辑，通过覆盖全局 heap 来实现
            int sortCount;
            cin >> sortCount;
            
            // 读取待排序数组
            int tempNums[1005];
            for(int i = 0; i < sortCount; i++){
                cin >> tempNums[i];
            }
            
            // 利用 build 函数将这组新数据建成一个小顶堆
            // 这会覆盖掉之前 heap 里的数据（根据题目逻辑，这可能是独立的或最后的操作）
            build(tempNums, sortCount);
            
            bool first = true;
            // 【堆排序逻辑】
            // 不断弹出堆顶（最小值），直到堆为空
            while(heapSize > 0) {
                if(!first) cout << " "; // 控制空格格式，第一个数前没空格
                cout << top(); // 输出最小值
                pop();         // 弹出它，让剩下的数重新排好
                first = false;
            }
            cout << endl; // 换行
        }
    }
    return 0;
}