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
    vector<vector<int>> build_automaton(const string& pat) {
        int m = pat.size();
        vector<int> pi = pre(pat);
        vector<vector<int>> nxt(m + 1, vector<int>(26, 0));
        for (int c = 0; c < 26; c++) {
            nxt[0][c] = (pat[0] == char('a' + c));
        }
        for (int i = 1; i <= m; i++) {
            for (int c = 0; c < 26; c++) {
                if (i < m && pat[i] == char('a' + c)) {
                    nxt[i][c] = i + 1;
                } else {
                    nxt[i][c] = nxt[pi[i - 1]][c];
                }
            }
        }
        return nxt;
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
struct suffixArray {
    /*
     * first we have to build the one of len 2^0
     * the string must be in the form of 2^k
     * the base is (n(log^2)) -> without Radix sort
     * assign classes to the strings so that the compare will be easy
     * copy the string in the constructor don't ask why
     */
    struct LCP {
        vector<vector<int>> lcp;
        vector<int> lg;
        int n, k;

        LCP(vector<int>& arr) {
            n = arr.size();
            lg.assign(n + 1, 0);
            pw();
            k = lg[n] + 1;
            lcp.assign(n, vector<int>(k));
            build(arr);
        }

        LCP() {
        }

        int merge(int a, int b) {
            return min(a, b);
        }

        void pw() {
            for (int i = 2; i <= n; i++) {
                lg[i] = lg[i >> 1] + 1;
            }
        }

        void build(vector<int>& arr) {
            for (int j = 0; j < k; j++) {
                for (int i = 0; i + (1 << j) - 1 < n; ++i) {
                    if (!j) {
                        lcp[i][j] = arr[i];
                    } else {
                        lcp[i][j] = merge(
                                lcp[i][j - 1],
                                lcp[i + (1 << (j - 1))][j - 1]
                        );
                    }
                }
            }
        }

        int query(int l, int r) {
            int len = r - l + 1;
            int pow = lg[len];
            return merge(
                    lcp[l][pow],
                    lcp[r - (1 << pow) + 1][pow]
            );
        }
    };

    string str;
    vector<int> p, c;
    LCP lc;
    int one = 1;

    void order(vector<int>& pi, vector<int>& cl) {
        int n = pi.size();
        vector<int> cnt(n);
        for (auto el : cl) {
            cnt[el]++;
        }
        vector<int> new_pi(n), pos(n);
        pos[0] = 0;
        for (int i = 1; i < n; i++) {
            pos[i] = pos[i - 1] + cnt[i - 1];
        }
        for (auto& x : pi) {
            int i = cl[x];
            new_pi[pos[i]++] = x;
        }
        swap(new_pi, pi);
    }

    void build(string tmp) {
        str = tmp;
        str.push_back(char(NULL));

        int n = str.size();
        vector<int> pi(n), cl(n);
        int k = 0;

        // build for k = 0
        {
            vector<pair<char, int>> a(n);
            for (int i = 0; i < n; i++) {
                a[i] = {str[i], i};
            }

            sort(a.begin(), a.end());// i can do this in o(n) but i will skip:)

            for (int i = 0; i < n; i++) {
                pi[i] = a[i].second;
            }

            cl[pi[0]] = 0;
            for (int i = 1; i < n; i++) {
                cl[pi[i]] = cl[pi[i - 1]] + (a[i].first != a[i - 1].first);
            }
        }

        while ((one << k) < n) {
            for (int i = 0; i < n; i++) {
                pi[i] = (pi[i] - (one << k) + n) % n;
            }

            order(pi, cl);

            vector<int> new_cl(n);
            new_cl[pi[0]] = 0;

            for (int i = 1; i < n; i++) {
                pair<int, int> cur{cl[pi[i]], cl[(pi[i] + (one << k)) % n]};
                pair<int, int> prv{cl[pi[i - 1]], cl[(pi[i - 1] + (one << k)) % n]};

                if (cur == prv) {
                    new_cl[pi[i]] = new_cl[pi[i - 1]];
                } else {
                    new_cl[pi[i]] = new_cl[pi[i - 1]] + 1;
                }
            }

            swap(new_cl, cl);
            k++;
        }

        p = pi;
        c = cl;
    }

    void build_lcp() {
        int n = p.size();
        vector<int> lcp(n);
        int k = 0;

        for (int i = 0; i < n - 1; i++) {
            int pi = c[i];
            int j = p[pi - 1];

            while (i + k < n && j + k < n && str[i + k] == str[j + k]) {
                k++;
            }

            lcp[pi] = k;
            k = max(0, k - 1);
        }

        lc = LCP(lcp);
    }
};
struct Aho_Corasick {
    static const int base = 26;
    struct Node {
        vector<int> nxt;
        int fail;
        vector<int> out;
        Node() {
            nxt.assign(base, -1);
            fail = 0;
        }
    };
    vector<Node> trie;
    Aho_Corasick() {
        trie.emplace_back();
    }

    int get(char c) {
        return c - 'a';
    }
    void add_string(const string& s, int id) {
        int cur = 0;
        for (char ch : s) {
            int c = get(ch);

            if (trie[cur].nxt[c] == -1) {
                trie[cur].nxt[c] = trie.size();
                trie.emplace_back();
            }

            cur = trie[cur].nxt[c];
        }

        trie[cur].out.push_back(id);
    }
    void build() {
        queue<int> q;
        for (int c = 0; c < base; c++) {
            int v = trie[0].nxt[c];
            if (v != -1) {
                trie[v].fail = 0;
                q.push(v);
            } else {
                trie[0].nxt[c] = 0;
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int c = 0; c < base; c++) {
                int v = trie[u].nxt[c];
                if (v != -1) {
                    trie[v].fail = trie[trie[u].fail].nxt[c];
                    auto &fail_out = trie[trie[v].fail].out;
                    trie[v].out.insert(trie[v].out.end(),fail_out.begin(),fail_out.end());
                    q.push(v);
                } else {
                    trie[u].nxt[c] = trie[trie[u].fail].nxt[c];
                }
            }
        }
    }
    vector<int> search(const string& text) {
        vector<int> matches;
        int cur = 0;
        for (int i = 0; i < (int)text.size(); i++) {
            cur = trie[cur].nxt[get(text[i])];
            for (int id : trie[cur].out) {
                matches.push_back(id);
            }
        }
        return matches;
    }
};

