#include <iostream>

using namespace std;

int n;
long long a[15];
long long ans = 0;
int visited[15] = {0};

void dfs(int step,long long currentSum){
    if(step > n){
        ans = ans|currentSum;
        return;
    }
    for(int i=1;i<=n;i++){
        if(visited[i]==0){
            visited[i]=1;
            long long val = a[i]^step;;
            dfs(step+1,currentSum+val);
            visited[i]=0;
        }
    }
}

int main(){
    cin >> n;
    for(int i =1;i<=n;i++){
        cin >> a[i];
    }
    dfs(1,0);
    cout << ans << endl;
    return 0;
}