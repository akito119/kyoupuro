#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;

//imosっぽくね？
//たぶん普通のimosやと見落としが発生するから上下左右の4方向imosするのがよさげ？

lint main(){
    int h,w;
    cin>>h>>w;
    int n,m;
    cin>>n>>m;
    vector<vector<int>> map(h,vector<int> (w));
    for(int i= 0;i<n;i++){
        int a,b;
        cin>>a>>b;
        a--;
        b--;
        map[a][b]=1;
    }
    for(int i = 0;i<m;i++){
        int c,d;
        cin>>c>>d;
        c--;
        d--;
        map[c][d] = -1;
    }
    int  status = 0;
    vector<vector<int>> ansmap(h,vector<int> (w));
    for(int i= 0;i<h;i++){
        for(int j=0;j<w;j++){
            if(map[i][j]==1){
                status = 1;
            }
            if(map[i][j]==-1){
                status = 0;
            }
            if(status > 0){
                ansmap[i][j]++;
            }
        }
        status = 0;
    }
    for(int i = 0;i<h;i++){
        for(int j=w-1;j>=0;j--){
            if(map[i][j]==1){
                status = 1;
            }
            if(map[i][j]==-1){
                status = 0;
            }
            if(status > 0){
                ansmap[i][j]++;
            }
        }
        status = 0;
    }
    for(int j=0;j<w;j++){
        for(int i= 0;i<h;i++){
            if(map[i][j]==1){
                status = 1;
            }
            if(map[i][j]==-1){
                status = 0;
            }
            if(status > 0){
                ansmap[i][j]++;
            }
        }
        status = 0;
    }
    for(int j = 0;j<w;j++){
        for(int i = h-1;i>=0;i--){
            if(map[i][j]==1){
                status = 1;
            }
            if(map[i][j]==-1){
                status = 0;
            }
            if(status > 0){
                ansmap[i][j]++;
            }
        }
        status = 0;
    }
    int ans = 0;
    for(int i= 0;i<h;i++){
        for(int j =0;j<w;j++){
            if(ansmap[i][j]>0){
                ans++;
            }
            //cout<<ansmap[i][j];
        }
        //cout<<endl;
    }
    cout<<ans<<endl;
}