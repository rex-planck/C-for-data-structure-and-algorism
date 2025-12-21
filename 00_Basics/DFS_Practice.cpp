#include <iostream>
using namespace std;

int n;
int a[30];
int target;
int ans = 0; // 用来记录符合条件的子集数量

// 请完成这个函数
// index: 当前考虑到数组的第几个位置
// currentSum: 当前子集的和
void dfs(int index, int currentSum) {
    
    // 1. 写出递归出口（Base Case）
    // 当 index 到达 n 时，说明一种子集方案已经生成完毕
    // 此时需要检查 currentSum 是否等于 target
    if (index == n) {
        if(currentSum > target) 
        return;
        // 在这里写你的判断逻辑
        if (currentSum == target) {
            ans++; // 找到一个符合条件的子集，计数加一
        }
        return;
    }

    // 2. 写出递归分支
    // 分支一：不选当前数字 a[index]
    dfs(index + 1, currentSum);
    
    // 分支二：选当前数字 a[index]
    // 注意：这里是加法运算，且要防止数组越界（虽然 base case 挡住了，但逻辑要清晰）
    dfs(index+1,currentSum + a[index]);
    
}

int main() {
    // 模拟输入数据
    n = 3;
    a[0] = 1; a[1] = 2; a[2] = 3;
    target = 3;
    
    // 从第0个索引开始，初始和为0
    dfs(0, 0);
    
    cout << "找到的幸运子集数量: " << ans << endl; // 预期输出 2
    
    return 0;
}