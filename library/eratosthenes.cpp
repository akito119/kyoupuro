#include <bits/stdc++.h>
using namespace std;
using lint = int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;

// https://qiita.com/sakofsuken/items/210e082169daba690570

const int bigNum = pow(10, 5);

vector<bool> isp(bigNum + 1, true);

void sieve()
{
    isp[0] = false;
    isp[1] = false;
    for (int i = 2; pow(i, 2) <= bigNum; i++)
    {
        if (isp[i])
            for (int j = 2; i * j <= bigNum; j++)
                isp[i * j] = false;
    }
}
