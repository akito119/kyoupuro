#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;


lint main(){
    int n;
    cin>>n;
    vector<int> a(n);
    rep(i, n){
        cin>>a[i];
    }

    priority_queue<int> q;
    rep(i, 3){
        q.push(a[i]);
    }
    vector<int> tmp = {};
    rep(i,3){
        tmp.push_back(q.top());
        if(i == 2){
            cout<<q.top()<<endl;
        }

        q.pop();
    }

    rep(i, 3){
        q.push(tmp[i]);
    }
    for(int i = 3;i<n;i++){
        q.push(a[i]);
        vector<int> tmp = {};
        rep(i,3){
            tmp.push_back(q.top());
            if(i == 2){
            cout<<q.top()<<endl;
        }

        q.pop();
    }

    rep(i, 3){
        q.push(tmp[i]);
    }
    }
}
