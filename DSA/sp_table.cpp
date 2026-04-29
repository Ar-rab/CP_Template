//
// Created by Arrab on 4/29/2026.
#include <bits/stdc++.h>
using namespace std;
class sparsTable{
private:
    int n;
    vector<int> lg;
    int k = 20;
    vector<vector<int>> st;
    void pw() {
        for (int i = 2; i <= n; i++)lg[i] = lg[i >> 1] + 1;
    }
    void build(vector<int> &arr){
        for (int j = 0; j < k; j++){
            for (int i = 0; i + (1 << j) - 1 < n; ++i){
                if(!j){
                    st[i][j]= arr[i];
                }
                else{
                    st[i][j] = merge(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
                }
            }
        }
    }
    int merge(int a , int b){
        return gcd(a,b);
    }
public:
    sparsTable(vector<int> &arr){
        n = (int)arr.size();
        if (n == 0) { k = 0; return;}
        lg.assign(n + 1, 0);
        pw();
        k = lg[n] + 1;
        st.assign(n, vector<int>(k, 0));
        build(arr);
    }
    int query(int l , int r){
        int len = r - l + 1;
        int pow = lg[len];
        return merge(st[l][pow] , st[r - (1 << pow) + 1 ][pow]);
    }
};
//
