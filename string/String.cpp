#include <bits/stdc++.h>
using namespace std;

namespace StringAlgo {

    vector<int> kmp_prefix_function(const string& s) {
        int n = s.size();
        vector<int> pi(n);
        int j = 0;
        for (int i = 1; i < n; i++) {
            while (j > 0 && s[i] != s[j]) {
                j = pi[j - 1];
            }
            if (s[i] == s[j]) j++;
            pi[i] = j;
        }
        return pi;
    }

    vector<int> kmp_find_occurrences(const string& str, string pat) {
        int n = str.size();
        int m = pat.size();
        if (m == 0) return vector<int>(n, 0);
        pat.push_back('#');
        vector<int> pi_pat = kmp_prefix_function(pat);
        vector<int> occ(n, 0);
        int j = 0;
        for (int i = 0; i < n; i++) {
            while (j > 0 && str[i] != pat[j]) {
                j = pi_pat[j - 1];
            }
            if (str[i] == pat[j]) {
                j++;
            }
            if (j == m) {
                occ[i - m + 1] = 1;
            }
        }
        return occ;
    }

    vector<vector<int>> kmp_build_automaton(const string& pat) {
        int m = pat.size();
        vector<int> pi = kmp_prefix_function(pat);
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

    vector<int> kmp_count_border_occurrences(const vector<int>& pi) {
        int n = pi.size();
        vector<int> num(n + 1, 0);

        for (int i = 0; i < n; i++) {
            num[pi[i]]++;
        }
        for (int i = n; i > 0; i--) {
            num[pi[i - 1]] += num[i];
        }
        for (int i = 0; i <= n; i++) {
            num[i]++;
        }
        return num;
    }

    vector<int> z_function(const string& s) {
        int n = s.size();
        vector<int> z(n);
        int l = 0, r = 0;
        for (int i = 1; i < n; i++) {
            if (i <= r) {
                z[i] = min(r - i, z[i - l]);
            }
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
                z[i]++;
            }
            if (i + z[i] > r) {
                l = i;
                r = i + z[i];
            }
        }
        return z;
    }

    vector<int> manacher_odd(const string& s) {
        int n = s.size();
        vector<int> d1(n);
        int l = 0, r = -1;
        for (int i = 0; i < n; i++) {
            int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
            while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
            d1[i] = k--;
            if (i + k > r) {
                l = i - k;
                r = i + k;
            }
        }
        return d1;
    }

    vector<int> manacher_even(const string& s) {
        int n = s.size();
        vector<int> d2(n);
        int l = 0, r = -1;
        for (int i = 0; i < n; i++) {
            int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
            d2[i] = k--;
            if (i + k > r) {
                l = i - k - 1;
                r = i + k;
            }
        }
        return d2;
    }

    bool manacher_is_palindrome(int l, int r, const vector<int>& d1, const vector<int>& d2) {
        int len = r - l + 1;
        if (len % 2) {
            int mid = (l + r) / 2;
            return d1[mid] >= (len + 1) / 2;
        } else {
            int c = l + len / 2;
            return d2[c] >= len / 2;
        }
    }

    pair<int, int> manacher_longest_palindrome(const string& s) {
        int n = s.size();
        if (n == 0) return {0, 0};
        auto d1 = manacher_odd(s);
        auto d2 = manacher_even(s);
        int best = 1, bestStart = 0;
        for (int i = 0; i < n; i++) {
            int len = 2 * d1[i] - 1;
            if (len > best) { best = len; bestStart = i - d1[i] + 1; }
        }
        for (int i = 0; i < n; i++) {
            int len = 2 * d2[i];
            if (len > best) { best = len; bestStart = i - d2[i]; }
        }
        return {best, bestStart};
    }

    long long manacher_count_palindromic_substrings(const string& s) {
        auto d1 = manacher_odd(s);
        auto d2 = manacher_even(s);
        long long total = 0;
        for (int x : d1) total += x;
        for (int x : d2) total += x;
        return total;
    }

    struct SuffixArray {
        struct SparseTable {
            vector<vector<int>> table;
            vector<int> lg;

            void build(const vector<int>& arr) {
                int n = arr.size();
                lg.assign(n + 1, 0);
                for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;
                if (n == 0) return;
                int k = lg[n] + 1;
                table.assign(k, vector<int>(n));
                table[0] = arr;
                for (int j = 1; j < k; j++) {
                    for (int i = 0; i + (1 << j) <= n; i++) {
                        table[j][i] = min(table[j - 1][i], table[j - 1][i + (1 << (j - 1))]);
                    }
                }
            }

            int query(int l, int r) {
                int len = r - l + 1;
                int k = lg[len];
                return min(table[k][l], table[k][r - (1 << k) + 1]);
            }
        };

        string s;
        int n;
        vector<int> sa;
        vector<int> rnk;
        vector<int> lcp;
        SparseTable sparse;

        SuffixArray(const string& str) : s(str) {
            n = s.size();
            build_sa();
            build_lcp();
            sparse.build(lcp);
        }

        void build_sa() {
            string t = s + '\1';
            int m = t.size();
            const int alphabet = 256;

            vector<int> p(m), c(m), cnt(alphabet, 0);
            for (unsigned char ch : t) cnt[ch]++;
            for (int i = 1; i < alphabet; i++) cnt[i] += cnt[i - 1];
            for (int i = 0; i < m; i++) p[--cnt[(unsigned char)t[i]]] = i;

            c[p[0]] = 0;
            int classes = 1;
            for (int i = 1; i < m; i++) {
                if (t[p[i]] != t[p[i - 1]]) classes++;
                c[p[i]] = classes - 1;
            }

            vector<int> pn(m), cn(m);
            for (int h = 0; (1 << h) < m; h++) {
                for (int i = 0; i < m; i++) {
                    pn[i] = p[i] - (1 << h);
                    if (pn[i] < 0) pn[i] += m;
                }

                vector<int> cnt2(classes, 0);
                for (int i = 0; i < m; i++) cnt2[c[i]]++;
                for (int i = 1; i < classes; i++) cnt2[i] += cnt2[i - 1];
                for (int i = m - 1; i >= 0; i--) p[--cnt2[c[pn[i]]]] = pn[i];

                cn[p[0]] = 0;
                classes = 1;
                for (int i = 1; i < m; i++) {
                    pair<int, int> cur{c[p[i]], c[(p[i] + (1 << h)) % m]};
                    pair<int, int> prv{c[p[i - 1]], c[(p[i - 1] + (1 << h)) % m]};
                    if (cur != prv) classes++;
                    cn[p[i]] = classes - 1;
                }
                swap(c, cn);
            }

            sa.assign(p.begin() + 1, p.end());
            rnk.assign(n, 0);
            for (int i = 0; i < n; i++) rnk[sa[i]] = i;
        }

        void build_lcp() {
            lcp.assign(n > 0 ? n - 1 : 0, 0);
            int k = 0;
            for (int i = 0; i < n; i++) {
                if (rnk[i] == n - 1) { k = 0; continue; }
                int j = sa[rnk[i] + 1];
                while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
                lcp[rnk[i]] = k;
                if (k) k--;
            }
        }

        int lcp_of(int i, int j) {
            if (i == j) return n - i;
            int l = rnk[i], r = rnk[j];
            if (l > r) swap(l, r);
            return sparse.query(l, r - 1);
        }

        long long count_distinct_substrings() {
            long long total = 1LL * n * (n + 1) / 2;
            for (int x : lcp) total -= x;
            return total;
        }
    };

    struct AhoCorasick {
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
        AhoCorasick() {
            trie.emplace_back();
        }

        int char_to_index(char c) {
            return c - 'a';
        }

        void add_string(const string& s, int id) {
            int cur = 0;
            for (char ch : s) {
                int c = char_to_index(ch);
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
                        auto& fail_out = trie[trie[v].fail].out;
                        trie[v].out.insert(trie[v].out.end(), fail_out.begin(), fail_out.end());
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
                cur = trie[cur].nxt[char_to_index(text[i])];
                for (int id : trie[cur].out) {
                    matches.push_back(id);
                }
            }
            return matches;
        }
    };

    struct SuffixAutomaton {
        struct State {
            int len = 0;
            int link = -1;
            map<char, int> next;
            long long cnt = 0;
        };

        vector<State> st;
        vector<int> order;
        vector<int> firstpos;
        string s;
        int last;

        SuffixAutomaton(int maxLen = 0) {
            st.reserve(maxLen * 2 + 5);
            firstpos.reserve(maxLen * 2 + 5);
            st.push_back(State());
            firstpos.push_back(-1);
            last = 0;
        }

        void extend(char c) {
            int cur = st.size();
            st.push_back(State());
            st[cur].len = st[last].len + 1;
            st[cur].cnt = 1;
            firstpos.push_back(st[cur].len - 1);
            int p = last;

            while (p != -1 && !st[p].next.count(c)) {
                st[p].next[c] = cur;
                p = st[p].link;
            }

            if (p == -1) {
                st[cur].link = 0;
            } else {
                int q = st[p].next[c];
                if (st[p].len + 1 == st[q].len) {
                    st[cur].link = q;
                } else {
                    int clone = st.size();
                    st.push_back(State());
                    st[clone].len = st[p].len + 1;
                    st[clone].next = st[q].next;
                    st[clone].link = st[q].link;
                    st[clone].cnt = 0;
                    firstpos.push_back(firstpos[q]);

                    while (p != -1 && st[p].next[c] == q) {
                        st[p].next[c] = clone;
                        p = st[p].link;
                    }

                    st[q].link = clone;
                    st[cur].link = clone;
                }
            }

            last = cur;
        }

        void build(const string& str) {
            s = str;
            for (char c : s) extend(c);
            computeCounts();
        }

        void computeCounts() {
            int n = st.size();
            order.resize(n - 1);
            for (int i = 1; i < n; i++) order[i - 1] = i;
            sort(order.begin(), order.end(), [&](int a, int b) {
                return st[a].len > st[b].len;
            });
            for (int v : order) {
                if (st[v].link != -1)
                    st[st[v].link].cnt += st[v].cnt;
            }
        }

        long long countDistinctSubstrings() {
            long long total = 0;
            for (size_t v = 1; v < st.size(); v++)
                total += st[v].len - st[st[v].link].len;
            return total;
        }

        string longestCommonSubstring(const string& t) {
            int cur = 0, length = 0, best = 0, bestPos = 0;
            for (int i = 0; i < (int)t.size(); i++) {
                char c = t[i];
                while (cur != 0 && !st[cur].next.count(c)) {
                    cur = st[cur].link;
                    length = st[cur].len;
                }
                auto it = st[cur].next.find(c);
                if (it != st[cur].next.end()) {
                    cur = it->second;
                    length++;
                } else {
                    cur = 0;
                    length = 0;
                }
                if (length > best) { best = length; bestPos = i; }
            }
            return t.substr(bestPos - best + 1, best);
        }

        string substrFromState(int v) {
            int end = firstpos[v];
            int len = st[v].len;
            return s.substr(end - len + 1, len);
        }

        vector<long long> cntPaths;

        void computeCntPaths() {
            cntPaths.assign(st.size(), 1);
            for (auto it = order.rbegin(); it != order.rend(); ++it) {
                int v = *it;
                long long res = 1;
                for (auto& [c, u] : st[v].next) res += cntPaths[u];
                cntPaths[v] = res;
            }
            long long res0 = 0;
            for (auto& [c, u] : st[0].next) res0 += cntPaths[u];
            cntPaths[0] = res0;
        }

        string kthSubstring(long long k) {
            if (cntPaths.empty()) computeCntPaths();
            if (k > cntPaths[0]) return "";

            string result;
            int cur = 0;
            while (k > 0) {
                bool advanced = false;
                for (auto& [c, u] : st[cur].next) {
                    if (k <= cntPaths[u]) {
                        result += c;
                        k--;
                        cur = u;
                        advanced = true;
                        if (k == 0) return result;
                        break;
                    } else {
                        k -= cntPaths[u];
                    }
                }
                if (!advanced) break;
            }
            return result;
        }

        bool contains(const string& t) {
            int cur = 0;
            for (char c : t) {
                auto it = st[cur].next.find(c);
                if (it == st[cur].next.end()) return false;
                cur = it->second;
            }
            return true;
        }

        string longestRepeatedSubstring() {
            int bestState = -1, bestLen = 0;
            for (size_t v = 1; v < st.size(); v++) {
                if (st[v].cnt > 1 && st[v].len > bestLen) {
                    bestLen = st[v].len;
                    bestState = v;
                }
            }
            if (bestState == -1) return "";
            return substrFromState(bestState);
        }

        pair<string, long long> mostFrequentSubstring() {
            int bestState = -1;
            long long bestCnt = 0;
            for (size_t v = 1; v < st.size(); v++) {
                if (st[v].cnt > bestCnt) {
                    bestCnt = st[v].cnt;
                    bestState = v;
                }
            }
            if (bestState == -1) return {"", 0};
            return {substrFromState(bestState), bestCnt};
        }

        long long occurrences(const string& t) {
            int cur = 0;
            for (char c : t) {
                auto it = st[cur].next.find(c);
                if (it == st[cur].next.end()) return 0;
                cur = it->second;
            }
            return st[cur].cnt;
        }
    };

    struct Trie {
        struct Node {
            int child[26];
            int cntPrefix = 0;
            int cntEnd = 0;
            Node() { fill(begin(child), end(child), -1); }
        };

        vector<Node> t;

        Trie() { t.push_back(Node()); }

        void insert(const string& s) {
            int cur = 0;
            t[cur].cntPrefix++;
            for (char ch : s) {
                int c = ch - 'a';
                if (t[cur].child[c] == -1) {
                    t[cur].child[c] = t.size();
                    t.push_back(Node());
                }
                cur = t[cur].child[c];
                t[cur].cntPrefix++;
            }
            t[cur].cntEnd++;
        }

        bool erase(const string& s) {
            if (!search(s)) return false;
            int cur = 0;
            t[cur].cntPrefix--;
            for (char ch : s) {
                int c = ch - 'a';
                cur = t[cur].child[c];
                t[cur].cntPrefix--;
            }
            t[cur].cntEnd--;
            return true;
        }

        bool search(const string& s) {
            int cur = 0;
            for (char ch : s) {
                int c = ch - 'a';
                if (t[cur].child[c] == -1 || t[t[cur].child[c]].cntPrefix == 0)
                    return false;
                cur = t[cur].child[c];
            }
            return t[cur].cntEnd > 0;
        }

        bool startsWith(const string& s) {
            int cur = 0;
            for (char ch : s) {
                int c = ch - 'a';
                if (t[cur].child[c] == -1 || t[t[cur].child[c]].cntPrefix == 0)
                    return false;
                cur = t[cur].child[c];
            }
            return true;
        }

        int countPrefix(const string& s) {
            int cur = 0;
            for (char ch : s) {
                int c = ch - 'a';
                if (t[cur].child[c] == -1) return 0;
                cur = t[cur].child[c];
                if (t[cur].cntPrefix == 0) return 0;
            }
            return t[cur].cntPrefix;
        }
    };

    struct BinaryTrie {
        static const int BITS = 30;

        struct Node {
            int child[2];
            int cnt = 0;
            Node() { child[0] = child[1] = -1; }
        };

        vector<Node> t;

        BinaryTrie() { t.push_back(Node()); }

        void insert(int x) {
            int cur = 0;
            t[cur].cnt++;
            for (int i = BITS; i >= 0; i--) {
                int b = (x >> i) & 1;
                if (t[cur].child[b] == -1) {
                    t[cur].child[b] = t.size();
                    t.push_back(Node());
                }
                cur = t[cur].child[b];
                t[cur].cnt++;
            }
        }

        bool erase(int x) {
            if (!contains(x)) return false;
            int cur = 0;
            t[cur].cnt--;
            for (int i = BITS; i >= 0; i--) {
                int b = (x >> i) & 1;
                cur = t[cur].child[b];
                t[cur].cnt--;
            }
            return true;
        }

        bool contains(int x) {
            int cur = 0;
            for (int i = BITS; i >= 0; i--) {
                int b = (x >> i) & 1;
                if (t[cur].child[b] == -1 || t[t[cur].child[b]].cnt == 0)
                    return false;
                cur = t[cur].child[b];
            }
            return true;
        }

        int maxXor(int x) {
            int cur = 0, result = 0;
            for (int i = BITS; i >= 0; i--) {
                int b = (x >> i) & 1;
                int want = b ^ 1;
                if (t[cur].child[want] != -1 && t[t[cur].child[want]].cnt > 0) {
                    result |= (1 << i);
                    cur = t[cur].child[want];
                } else {
                    cur = t[cur].child[b];
                }
            }
            return result;
        }

        int minXor(int x) {
            int cur = 0, result = 0;
            for (int i = BITS; i >= 0; i--) {
                int b = (x >> i) & 1;
                if (t[cur].child[b] != -1 && t[t[cur].child[b]].cnt > 0) {
                    cur = t[cur].child[b];
                } else {
                    result |= (1 << i);
                    cur = t[cur].child[b ^ 1];
                }
            }
            return result;
        }
    };

} // namespace StringAlgo
