//
// Created by Arrab on 4/29/2026.
#include <bits/stdc++.h>
using namespace std;
namespace Math {
    using ll = long long;
    ll mod = 1e9+7;

    inline ll add(ll a, ll b) {
        a %= mod; b %= mod;
        ll r = a + b;
        if (r >= mod) r -= mod;
        return r;
    }
    inline ll sub(ll a, ll b) {
        a %= mod; b %= mod;
        ll r = a - b;
        if (r < 0) r += mod;
        return r;
    }
    inline ll mul(ll a, ll b) {
        return (a % mod) * (b % mod) % mod;
    }
    ll power_mod(ll a, ll p) {
        ll res = 1;
        a %= mod;
        while (p) {
            if (p & 1) res = mul(res, a);
            a = mul(a, a);
            p >>= 1;
        }
        return res;
    }
    ll inv(ll a) { return power_mod(a, mod - 2); }

    ll gcd(ll a, ll b) {
        while (b) {
            a %= b;
            swap(a, b);
        }
        return a;
    }
    ll lcm(ll a, ll b) {
        return (a / gcd(a, b)) * b;
    }
    vector<int> primes;
    vector<int> spf;
    vector<bool> is_prime;

    void sieve(int n) {
        primes.clear();
        is_prime.assign(n+1, true);
        spf.assign(n+1, 0);

        is_prime[0] = is_prime[1] = false;

        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (1LL * i * p > n) break;
                spf[i * p] = p;
                is_prime[i * p] = false;
                if (i % p == 0) break;
            }
        }
    }

    vector<pair<int,int>> factorize(int x) {
        vector<pair<int,int>> res;
        while (x > 1) {
            int p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            res.push_back({p, cnt});
        }
        return res;
    }

    vector<int> get_divisors(int x) {
        vector<pair<int,int>> f = factorize(x);
        vector<int> divisors = {1};

        for (auto &[p, cnt] : f) {
            int sz = divisors.size();
            ll mul = 1;
            for (int i = 1; i <= cnt; i++) {
                mul *= p;
                for (int j = 0; j < sz; j++) {
                    divisors.push_back(divisors[j] * mul);
                }
            }
        }
        return divisors;
    }

    vector<ll> fact, inv_fact;
    void init_comb(int n) {
        fact.assign(n+1, 1);
        inv_fact.assign(n+1, 1);

        for (int i = 1; i <= n; i++)
            fact[i] = mul(fact[i-1], i);

        inv_fact[n] = inv(fact[n]);
        for (int i = n-1; i >= 0; i--)
            inv_fact[i] = mul(inv_fact[i+1], i+1);
    }
    ll nCr(int n, int r) {
        if (r < 0 || r > n) return 0;
        return mul(fact[n], mul(inv_fact[r], inv_fact[n-r]));
    }

}

//
