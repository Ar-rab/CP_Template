#include <bits/stdc++.h>

namespace KMP {
    using std::vector;
    using std::string;
    vector<int> pre(const string& str){
        int n = str.size();
        vector<int> pi(n);
        int j = 0;
        for(int i = 1 ; i < n; i++){
            while (j > 0 && str[i] != str[j]){
                j = pi[j-1];
            }
            if (str[i] == str[j]) j++;
            pi[i] = j;
        }
        return pi;
    }
    vector<int> match(const string& str, string pat){
        int n = str.size();
        int m = pat.size();
        pat.push_back('#');
        vector<int> pi_pat = pre(pat);
        vector<int> occ(n, 0);
        int j = 0;
        for (int i = 0; i < n; i++){
            while (j > 0 && str[i] != pat[j]){
                j = pi_pat[j-1];
            }
            if (str[i] == pat[j]){
                j++;
            }
            if (j == m){
                occ[i - m + 1] = 1;
            }
        }
        return occ;
    }
    vector<int> count_occ(const vector<int>& pi){
        int n = pi.size();
        vector<int> num(n+1, 0);

        for (int i = 0; i < n; i++){
            num[pi[i]]++;
        }
        for (int i = n; i > 0; i--){
            num[pi[i-1]] += num[i];
        }
        for (int i = 0; i <= n; i++){
            num[i]++;
        }
        return num;
    }
    vector<int> z_function(const string& s) {
        int n = s.size();
        vector<int> z(n);
        int l = 0, r = 0;
        for(int i = 1; i < n; i++) {
            if(i <= r) {
                z[i] = std::min(r - i, z[i - l]);
            }
            while(i + z[i] < n && s[z[i]] == s[i + z[i]]) {
                z[i]++;
            }
            if(i + z[i] > r) {
                l = i;
                r = i + z[i];
            }
        }
        return z;
    }

}


