#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;


lint main(){
    int n,m;
    int q;
    cin>>n>>m>>q;
    vector<pair<int,int>> item(n);
    vector<int> box(m);
    for(int i= 0;i<n;i++){
        int w,v;
        cin>>w>>v;
        item[i] = {w,v};
    }
    for(int i = 0;i<m;i++){
        int x;
        cin>>x;
        box[i] = x;
    }
    for(int k= 0;k<q;k++){
        int l,r;
        cin>>l>>r;
        l--,r--;
        priority_queue<int,vector<int>,greater<int>> q;
        for(int i= 0;i<m;i++){
            if(i<l||i>r){
                q.push(box[i]);
            }
        }
        int ans = 0;
        vector<bool> is_used(n);
        while(!q.empty()){
            int now = q.top();q.pop();
            int tmp = 0;
            int tmp_v = 0;
            for(int i = 0;i<n;i++){
                if(is_used[i]){
                    continue;
                }
                if(now>=item[i].first){
                    if(tmp_v<=item[i].second){
                        //cout<<"a"<<endl;
                        tmp = i;
                        tmp_v = item[tmp].second;
                    }
                }
            }
            ans += tmp_v;
            if(tmp_v!= 0) is_used[tmp] = true;
        }
        cout<<ans<<endl;
    }
}
