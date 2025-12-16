#include <iostream>
using namespace std;

// 排序类模板
template <class T>
class Sort {
private:
    T* arr;   // 动态数组
    int size; // 数组大小

    // 辅助函数：交换两个元素（替代 std::swap，满足不使用 STL 的要求）
    void mySwap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }

public:
    // 构造函数
    Sort(T* inputArr, int n) : size(n) {
        arr = new T[size];
        for (int i = 0; i < size; i++) {
            arr[i] = inputArr[i];
        }
    }

    // 析构函数
    ~Sort() {
        delete[] arr;
    }

    // 1. 名次排序 (Rank Sort)
    // 原理：统计每个元素比其他多少个元素大，算出它的“名次”，然后直接放到对应位置
    void rankSort() {
        int* rank = new int[size]; // 存放每个元素的名次
        // 初始化名次为 0
        for (int i = 0; i < size; i++) rank[i] = 0;

        // 计算名次：比较所有元素对
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < i; j++) {
                if (arr[i] < arr[j]) {
                    rank[j]++; // j 比 i 大，j 的名次+1
                } else {
                    rank[i]++; // i 比 j 大（或相等），i 的名次+1
                }
            }
        }

        // 根据名次将元素放入临时数组
        T* temp = new T[size];
        for (int i = 0; i < size; i++) {
            temp[rank[i]] = arr[i];
        }

        // 将排序好的数据拷回原数组
        for (int i = 0; i < size; i++) {
            arr[i] = temp[i];
        }

        // 释放辅助空间
        delete[] rank;
        delete[] temp;
    }

    // 2. 及时终止的选择排序 (Selection Sort with Early Termination)
    // 解释：标准选择排序即使有序也会跑完 O(n^2)。
    // "及时终止"通常指增加一个检查：如果在某轮扫描中发现数组已经有序，则提前退出。
    void selectionSort() {
        bool sorted = false; // 一开始先保持警惕，假设还没排好
        // 只要 i 还没走到头，并且(!sorted)警报响着（说明还没排好），就继续干
        for (int i = 0; i < size - 1 && !sorted; i++) {
            int minIdx = i;  // 假设当前站在这里的这个就是最小的
            sorted = true;   // 【关键点 1】每一轮开始前，先做一个“乐观的假设”：
                         // “我觉得这一轮跑完，后面肯定都是整齐的，我想下班了。”

            for (int j = i + 1; j < size; j++) {
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                    sorted = false; // 发生了更小值的更新，说明可能还未完全有序
                } else if (arr[j] < arr[j-1]) {
                    // 额外的检查：如果发现后面有乱序对，说明肯定没排好
                    sorted = false; 
                }
            }

            if (minIdx != i) {
                mySwap(arr[i], arr[minIdx]);
                sorted = false; // 发生了交换，说明之前没排好
            }
        }
    }

    // 3. 及时终止的冒泡排序 (Bubble Sort with Early Termination)
    // 原理：标准冒泡。如果某一轮比较中没有发生任何交换，说明数组已经有序，直接 break。
    void bubbleSort() {
        for (int i = 0; i < size - 1; i++) {
            bool swapped = false; // 标记本轮是否有交换
            for (int j = 0; j < size - 1 - i; j++) {
                if (arr[j] > arr[j + 1]) {
                    mySwap(arr[j], arr[j + 1]);
                    swapped = true; // 发生了交换
                }
            }
            // 及时终止：如果没有发生交换，说明已经有序
            if (!swapped) break;
        }
    }

    // 4. 插入排序 (Insertion Sort)
    // 原理：类似打扑克摸牌
    void insertionSort() {
        for (int i = 1; i < size; i++) {
            T key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

    // 打印数组
    void printSorted() {
        for (int i = 0; i < size; i++) {
            if (i > 0) cout << " ";
            cout << arr[i];
        }
        cout << endl;
    }
};


int main() {
    int n;
    // cout << "请输入元素个数 n: "; // 提交OJ时通常不需要提示语，根据需要注释掉
    cin >> n;

    int* input = new int[n];
    // cout << "请输入 " << n << " 个整数: ";
    for (int i = 0; i < n; i++) {
        cin >> input[i];
    }

    // 创建对象
    Sort<int> sorter(input, n);

    // 这里为了演示，你可以选择调用任意一种排序方法
    // 比如调用 及时终止的冒泡排序：
    sorter.bubbleSort(); 

    // 输出结果
    sorter.printSorted();

    delete[] input;
    return 0;
}