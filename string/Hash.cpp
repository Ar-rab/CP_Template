//
// Created by Arrab on 4/29/2026.
#include <bits/stdc++.h>
using namespace std;
namespace Hashing {

    using ll = long long;
    using ull = unsigned long long;
    using pi = pair<int,int>;

    // ================= RANDOM =================
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    unordered_map<int, ull> rnd;

    inline ull get_rand(int x) {
        auto it = rnd.find(x);
        if (it != rnd.end()) return it->second;
        return rnd[x] = rng();
    }

    // ================= ROLLING HASH =================
    const int mod = 1e9 + 7;
    const int b1 = 31, b2 = 69;
    const int b1I = 129032259, b2I = 579710149;

    vector<int> Pb1, Pb2;

    void pre(int n) {
        Pb1.assign(n+1, 1);
        Pb2.assign(n+1, 1);
        for (int i = 1; i <= n; i++) {
            Pb1[i] = 1LL * Pb1[i-1] * b1 % mod;
            Pb2[i] = 1LL * Pb2[i-1] * b2 % mod;
        }
    }

    // ================= PREFIX HASH =================
    struct PrefixHash {
        vector<int> h1, h2;
        int n;

        PrefixHash(const string &s) {
            n = s.size();
            h1.assign(n+1, 0);
            h2.assign(n+1, 0);

            for (int i = 0; i < n; i++) {
                int x = (unsigned char)s[i];
                h1[i+1] = (1LL * h1[i] * b1 + x) % mod;
                h2[i+1] = (1LL * h2[i] * b2 + x) % mod;
            }
        }

        inline pi get(int l, int r) const {
            int x1 = (h1[r+1] - 1LL * h1[l] * Pb1[r-l+1] % mod + mod) % mod;
            int x2 = (h2[r+1] - 1LL * h2[l] * Pb2[r-l+1] % mod + mod) % mod;
            return {x1, x2};
        }
    };

    // ================= DYNAMIC HASH =================
    struct Hash {
        pi code{0,0};
        int size = 0;

        inline void push_back(int x) {
            code.first  = (code.first  + 1LL * Pb1[size] * x) % mod;
            code.second = (code.second + 1LL * Pb2[size] * x) % mod;
            size++;
        }

        inline void push_front(int x) {
            code.first  = (1LL * code.first * b1 + x) % mod;
            code.second = (1LL * code.second * b2 + x) % mod;
            size++;
        }

        inline void pop_back(int x) {
            size--;
            code.first  = (code.first  - 1LL * Pb1[size] * x % mod + mod) % mod;
            code.second = (code.second - 1LL * Pb2[size] * x % mod + mod) % mod;
        }

        inline void pop_front(int x) {
            code.first  = 1LL * (code.first  - x + mod) * b1I % mod;
            code.second = 1LL * (code.second - x + mod) * b2I % mod;
            size--;
        }

        inline void clear() {
            code = {0,0};
            size = 0;
        }

        inline Hash operator+(const Hash &o) const {
            Hash res;
            res.code.first  = (1LL * code.first  * Pb1[o.size] + o.code.first) % mod;
            res.code.second = (1LL * code.second * Pb2[o.size] + o.code.second) % mod;
            res.size = size + o.size;
            return res;
        }

        inline bool operator==(const Hash &o) const {
            return size == o.size && code == o.code;
        }

        inline bool operator!=(const Hash &o) const {
            return !(*this == o);
        }

        inline bool operator<(const Hash &o) const {
            return code == o.code ? size < o.size : code < o.code;
        }
    };

    // ================= PAIR HASH =================
    struct pair_hash {
        template <class T1, class T2>
        size_t operator()(const pair<T1, T2>& p) const {
            size_t h1 = hash<T1>{}(p.first);
            size_t h2 = hash<T2>{}(p.second);
            return h1 ^ (h2 + 0x9e3779b9 + (h1<<6) + (h1>>2));
        }
    };

}

//
