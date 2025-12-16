#include<iostream>
using namespace std;

int n;
int a[30];
long long ans = 0;

void dfs(int step, int count, long long currentSum){
    if(step == n){
        //差点写成乘法了
        ans = ans ^ currentSum;
        return;
    }

    dfs(step+1,count,currentSum);
    dfs(step+1,count+1,currentSum+(count+1)*a[step]);
    //这里又写成了（count+1）*a[step+1]
}

int main(){
    cin >> n;
    for (int i = 0; i <n; i++){
        cin >>a[i];
        //
    }
    dfs(0,0,0);
    cout << ans <<endl;
    return 0;
}