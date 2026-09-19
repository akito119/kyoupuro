#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
int ans = 0;

void dfs(vector<int>& ans, int n, int k,int sum){
    if(ans.size() == n -1){
        int res = k - sum;
        if(res%n ==0){
            ans.push_back(res/n);
        }
        else{
            return;
        }
        rep(i,n){
            if(i == n-1){
                cout<<ans[i]<<endl;
            }
            else{
                cout<<ans[i]<<" ";
            }
        }
        ans.pop_back();
        return;
    }


    for(int i = 0;i<=(k-sum)/(ans.size()+1);i++){
        ans.push_back(i);
        dfs(ans, n, k, sum+ans[ans.size()-1]*ans.size());
        ans.pop_back();
    }

    return;

}

lint main(){
   int n,k;
   cin>>n>>k;
   vector<int> ans(0);
   dfs(ans, n, k, 0);
}