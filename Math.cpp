#include <bits/stdc++.h>
using namespace std;

namespace Math {
    using ll = long long;
    ll MOD = 1e9 + 7;

    // O(1) | (a+b) mod m
    inline ll add(ll a, ll b, ll mod = MOD) {
        a %= mod;
        b %= mod;
        ll r = a + b;
        if (r >= mod) r -= mod;
        if (r < 0) r += mod;
        return r;
    }

    // O(1) | (a-b) mod m
    inline ll sub(ll a, ll b, ll mod = MOD) {
        a %= mod;
        b %= mod;
        ll r = a - b;
        if (r < 0) r += mod;
        return r;
    }

    // O(1) | (a*b) mod m
    inline ll mul(ll a, ll b, ll mod = MOD) {
        a %= mod;
        b %= mod;
        if (a < 0) a += mod;
        if (b < 0) b += mod;
        return (ll)((__int128)a * b % mod);
    }

    // O(log p) | a^p mod m
    inline ll power_mod(ll a, ll p, ll mod = MOD) {
        ll res = 1;
        a %= mod;
        if (a < 0) a += mod;
        while (p) {
            if (p & 1) res = mul(res, a, mod);
            a = mul(a, a, mod);
            p >>= 1;
        }
        return res;
    }

    // O(log m) | a^(m-2) mod m, m prime (Fermat)
    inline ll inv(ll a, ll mod = MOD) {
        return power_mod(a, mod - 2, mod);
    }

    // O(log m) | a/b mod m, m prime
    inline ll div_(ll a, ll b, ll mod = MOD) {
        return mul(a, inv(b, mod), mod);
    }
}

// mint: fixed-modulus int type. +,-,* are O(1); pow()/inv() are O(log MOD).
template <int MOD>
struct ModInt {
    long long v;

    ModInt() : v(0) {}

    ModInt(long long x) {
        x %= MOD;
        if (x < 0) x += MOD;
        v = x;
    }

    ModInt &operator+=(const ModInt &o) {
        v = Math::add(v, o.v, MOD);
        return *this;
    }

    ModInt &operator-=(const ModInt &o) {
        v = Math::sub(v, o.v, MOD);
        return *this;
    }

    ModInt &operator*=(const ModInt &o) {
        v = Math::mul(v, o.v, MOD);
        return *this;
    }

    ModInt operator+(const ModInt &o) const { return ModInt(*this) += o; }
    ModInt operator-(const ModInt &o) const { return ModInt(*this) -= o; }
    ModInt operator*(const ModInt &o) const { return ModInt(*this) *= o; }
    ModInt operator-() const { return ModInt(-v); }

    // O(log p) | v^p mod MOD
    ModInt pow(long long p) const {
        return ModInt(Math::power_mod(v, p, MOD));
    }

    // O(log MOD) | v^(MOD-2) mod MOD, MOD prime
    ModInt inv() const {
        return ModInt(Math::inv(v, MOD));
    }

    ModInt &operator/=(const ModInt &o) { return *this *= o.inv(); }
    ModInt operator/(const ModInt &o) const { return ModInt(*this) /= o; }

    bool operator==(const ModInt &o) const { return v == o.v; }
    bool operator!=(const ModInt &o) const { return v != o.v; }

    friend ostream &operator<<(ostream &os, const ModInt &m) { return os << m.v; }

    friend istream &operator>>(istream &is, ModInt &m) {
        long long x;
        is >> x;
        m = ModInt(x);
        return is;
    }
};

using mint = ModInt<1000000007>;

namespace Math {

    // O(log(min(a,b))) | gcd(a,b)
    inline ll gcd(ll a, ll b) {
        while (b) {
            a %= b;
            swap(a, b);
        }
        return a;
    }

    // O(log(min(a,b))) | lcm(a,b) = a/gcd(a,b)*b
    inline ll lcm(ll a, ll b) {
        return a / gcd(a, b) * b;
    }

    // O(log(min(a,b))) | g = gcd(a,b), a*x + b*y = g (Bezout)
    inline ll extgcd(ll a, ll b, ll &x, ll &y) {
        if (!b) {
            x = 1;
            y = 0;
            return a;
        }
        ll x1, y1;
        ll g = extgcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    }

    vector<int> primes;
    vector<int> spf;

    // O(n log log n) | builds spf[]/primes[] up to n
    void sieve(int n) {
        primes.clear();
        spf.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (1LL * i * p > n || p > spf[i]) break;
                spf[i * p] = p;
            }
        }
    }

    // O(log x), requires sieve(n>=x) | x = p1^e1 * p2^e2 * ...
    vector<pair<int,int>> factorize(int x) {
        vector<pair<int,int>> res;
        while (x > 1) {
            int p = spf[x], cnt = 0;
            while (x % p == 0) {
                x /= p;
                cnt++;
            }
            res.push_back({p, cnt});
        }
        return res;
    }

    // O(sqrt(x)) | x = p1^e1 * p2^e2 * ...
    vector<pair<ll,int>> factorizeSlow(ll x) {
        vector<pair<ll,int>> res;
        for (ll p = 2; p * p <= x; p++) {
            if (x % p == 0) {
                int cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    cnt++;
                }
                res.push_back({p, cnt});
            }
        }
        if (x > 1) res.push_back({x, 1});
        return res;
    }

    // O(sqrt(x) + d(x)) | full divisor list
    vector<ll> get_divisors(ll x) {
        vector<pair<ll,int>> f = factorizeSlow(x);
        vector<ll> divisors = {1};
        for (auto &[p, cnt] : f) {
            int cur = (int)divisors.size();
            ll mulp = 1;
            for (int i = 1; i <= cnt; i++) {
                mulp *= p;
                for (int j = 0; j < cur; j++)
                    divisors.push_back(divisors[j] * mulp);
            }
        }
        return divisors;
    }

    // O(sqrt(n))
    inline bool isPrime(ll n) {
        if (n < 2) return false;
        for (ll p = 2; p * p <= n; p++)
            if (n % p == 0) return false;
        return true;
    }

    // O(log^2 n) to O(log^3 n) | deterministic Miller-Rabin for n < 2^64
    inline bool isPrimeFast(ll n) {
        if (n < 2) return false;
        for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
            if (n % p == 0) return n == p;

        ll d = n - 1;
        int r = 0;
        while (d % 2 == 0) {
            d /= 2;
            r++;
        }

        for (ll a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
            if (a % n == 0) continue;
            ll x = power_mod(a, d, n);
            if (x == 1 || x == n - 1) continue;

            bool composite = true;
            for (int i = 0; i < r - 1; i++) {
                x = mul(x, x, n);
                if (x == n - 1) {
                    composite = false;
                    break;
                }
            }
            if (composite) return false;
        }
        return true;
    }

    // O(n^(1/4)) expected per factor
    inline ll pollardRho(ll n) {
        if (n % 2 == 0) return 2;

        ll x = rand() % n, y = x, c = rand() % n + 1, d = 1;
        while (d == 1) {
            x = add(mul(x, x, n), c, n);
            y = add(mul(y, y, n), c, n);
            y = add(mul(y, y, n), c, n);
            d = gcd(llabs(x - y), n);
        }
        return d != n ? d : -1;
    }

    // O(n^(1/4) log n) expected, n up to ~1e18 | full factorization via Miller-Rabin + Pollard Rho
    inline void factorizeFastImpl(ll n, map<ll,int> &out) {
        if (n == 1) return;
        if (isPrimeFast(n)) {
            out[n]++;
            return;
        }
        ll d = n;
        while (d == n || d == -1) d = pollardRho(n);
        factorizeFastImpl(d, out);
        factorizeFastImpl(n / d, out);
    }

    inline vector<pair<ll,int>> factorizeFast(ll n) {
        map<ll,int> m;
        factorizeFastImpl(n, m);
        return vector<pair<ll,int>>(m.begin(), m.end());
    }

    // O(sqrt(n)) | phi(n) = n * PRODUCT(1 - 1/p) over distinct primes p|n
    inline ll phi(ll n) {
        ll result = n;
        for (auto &[p, e] : factorizeSlow(n))
            result -= result / p;
        return result;
    }

    vector<mint> fact, inv_fact;

    // O(n) | precomputes n!, (n!)^-1
    void init_comb(int n) {
        fact.assign(n + 1, mint(1));
        inv_fact.assign(n + 1, mint(1));

        for (int i = 1; i <= n; i++)
            fact[i] = fact[i-1] * i;

        inv_fact[n] = fact[n].inv();
        for (int i = n - 1; i >= 0; i--)
            inv_fact[i] = inv_fact[i+1] * (i + 1);
    }

    // O(1), requires init_comb(n) | C(n,r) = n! / (r!(n-r)!)
    inline mint nCr(int n, int r) {
        if (r < 0 || r > n || n < 0) return mint(0);
        return fact[n] * inv_fact[r] * inv_fact[n-r];
    }

    // O(1), requires init_comb(n) | P(n,r) = n! / (n-r)!
    inline mint nPr(int n, int r) {
        if (r < 0 || r > n || n < 0) return mint(0);
        return fact[n] * inv_fact[n-r];
    }

    // O(1), requires init_comb(2n) | C_n = C(2n,n) - C(2n,n+1)
    inline mint catalan(int n) {
        return nCr(2*n, n) - nCr(2*n, n+1);
    }

    // O(r), n up to ~1e18 | C(n,r) = n(n-1)...(n-r+1) / r!
    inline mint nCrBigNSmallR(ll n, ll r) {
        if (r < 0 || r > n) return mint(0);

        mint num(1);
        for (ll i = 0; i < r; i++)
            num *= mint(n - i);

        mint den(1);
        for (ll i = 1; i <= r; i++)
            den *= mint(i);

        return num / den;
    }

    // O(log(min(m1,m2))) | x = r1 (mod m1), x = r2 (mod m2) -> {x, lcm(m1,m2)} or {-1,-1}
    inline pair<ll,ll> crt(ll r1, ll m1, ll r2, ll m2) {
        ll x, y;
        ll g = extgcd(m1, m2, x, y);
        if ((r2 - r1) % g != 0) return {-1, -1};

        ll lcm_ = m1 / g * m2;
        ll mul_ = (r2 - r1) / g % (m2 / g);
        ll res = r1 + m1 * ((mul_ * (x % (m2 / g)) % (m2 / g) + (m2 / g)) % (m2 / g));
        res = ((res % lcm_) + lcm_) % lcm_;
        return {res, lcm_};
    }

    // O(k log(max modulus)) | extended CRT for k congruences, moduli need not be coprime
    inline pair<ll,ll> crtMulti(const vector<pair<ll,ll>> &eqs) {
        ll r = eqs[0].first, m = eqs[0].second;

        for (size_t i = 1; i < eqs.size(); i++) {
            auto [nr, nm] = crt(r, m, eqs[i].first, eqs[i].second);
            if (nr == -1 && nm == -1) return {-1, -1};
            r = nr;
            m = nm;
        }
        return {r, m};
    }

    // O(n) | D(0)=1, D(1)=0, D(n) = (n-1)(D(n-1) + D(n-2))
    inline mint derangements(int n) {
        if (n == 0) return mint(1);
        if (n == 1) return mint(0);

        vector<mint> D(n + 1);
        D[0] = mint(1);
        D[1] = mint(0);

        for (int i = 2; i <= n; i++)
            D[i] = mint(i - 1) * (D[i-1] + D[i-2]);

        return D[n];
    }

    // O(n), requires init_comb(n) | perms with exactly k fixed points = C(n,k) * D(n-k)
    inline mint permsWithKFixedPoints(int n, int k) {
        if (k < 0 || k > n) return mint(0);
        return nCr(n, k) * derangements(n - k);
    }

    // O(n) | perms with exactly one fixed point = n * D(n-1)
    inline mint permsWithOneFixedPoint(int n) {
        return mint(n) * derangements(n - 1);
    }

    // O(1), requires init_comb(n+k) | n identical items into k distinct bins (empty ok) = C(n+k-1, k-1)
    inline mint starsAndBars(int n, int k) {
        if (k <= 0) return mint(n == 0 ? 1 : 0);
        return nCr(n + k - 1, k - 1);
    }

    // O(1), requires init_comb(n) | same, every bin >= 1, n >= k required = C(n-1, k-1)
    inline mint starsAndBarsPositive(int n, int k) {
        if (n < k || k <= 0) return mint(0);
        return nCr(n - 1, k - 1);
    }

    // O(log p) | a^(p-1) = 1 (mod p), p prime, a not divisible by p
    inline bool fermatCheck(ll a, ll p) {
        return power_mod(a, p - 1, p) == 1;
    }

    // O(n) | n prime <=> (n-1)! = -1 (mod n)
    inline bool wilsonIsPrime(ll n) {
        if (n < 2) return false;

        ll r = 1;
        for (ll i = 2; i < n; i++)
            r = mul(r, i, n);

        return r == n - 1;
    }

    // O(p + log_p(n)), p small (~<=1e6), n up to 1e18 | C(n,r) mod p via Lucas
    inline ll nCrModPrimeSmall(ll n, ll r, ll p) {
        if (r < 0 || r > n) return 0;

        ll res = 1;
        while (n > 0 || r > 0) {
            ll ni = n % p, ri = r % p;
            if (ri > ni) return 0;

            ll num = 1, den = 1;
            for (ll i = 0; i < ri; i++) {
                num = mul(num, ni - i, p);
                den = mul(den, i + 1, p);
            }

            res = mul(res, mul(num, power_mod(den, p - 2, p), p), p);
            n /= p;
            r /= p;
        }
        return res;
    }

    // O(log_p n) | exponent of prime p in n! = SUM floor(n / p^i)
    inline ll legendreExponent(ll n, ll p) {
        ll cnt = 0;
        while (n) {
            n /= p;
            cnt += n;
        }
        return cnt;
    }

    // O(n) worst case, requires gcd(a,n) = 1 | smallest k > 0: a^k = 1 (mod n)
    inline ll multiplicativeOrder(ll a, ll n) {
        ll k = 1, cur = a % n;
        while (cur != 1) {
            cur = cur * a % n;
            k++;
            if (k > n) return -1;
        }
        return k;
    }

    // O(#distinct primes) | d(n) = PRODUCT(e_i + 1)
    inline ll countDivisorsFromFactors(const vector<pair<ll,int>> &f) {
        ll res = 1;
        for (auto &[p, e] : f)
            res *= (e + 1);
        return res;
    }

    // O(SUM e_i) | sigma(n) = PRODUCT(1 + p + ... + p^e)
    inline ll sumDivisorsFromFactors(const vector<pair<ll,int>> &f) {
        ll res = 1;
        for (auto &[p, e] : f) {
            ll term = 0, pw = 1;
            for (int i = 0; i <= e; i++) {
                term += pw;
                pw *= p;
            }
            res *= term;
        }
        return res;
    }

    // O(N^2) | S(n,k) = k*S(n-1,k) + S(n-1,k-1)
    inline vector<vector<mint>> stirling2Table(int N) {
        vector<vector<mint>> S(N + 1, vector<mint>(N + 1, mint(0)));
        S[0][0] = mint(1);

        for (int n = 1; n <= N; n++)
            for (int k = 1; k <= n; k++)
                S[n][k] = mint(k) * S[n-1][k] + S[n-1][k-1];

        return S;
    }

    // O(N^2) | B(n) = SUM_k S(n,k)
    inline vector<mint> bellNumbers(int N) {
        auto S = stirling2Table(N);
        vector<mint> B(N + 1, mint(0));

        for (int n = 0; n <= N; n++)
            for (int k = 0; k <= n; k++)
                B[n] = B[n] + S[n][k];

        return B;
    }

    // O(1), requires gcd(a,b) = 1 | Frobenius number = a*b - a - b
    inline ll frobeniusNumber(ll a, ll b) {
        return a * b - a - b;
    }

    // O(log n) | F(2k) = F(k)(2F(k+1) - F(k)), F(2k+1) = F(k)^2 + F(k+1)^2. Returns {F(n), F(n+1)}
    inline pair<mint,mint> fibPair(ll n) {
        if (n == 0) return {mint(0), mint(1)};

        auto [a, b] = fibPair(n / 2);
        mint c = a * (mint(2) * b - a);
        mint d = a * a + b * b;

        if (n & 1) return {d, c + d};
        return {c, d};
    }

    inline mint fib(ll n) {
        return fibPair(n).first;
    }

    // O(2^k * cost(f)) for k conditions
    // Pattern: count = SUM over non-empty subsets S of conditions: (-1)^(|S|+1) * f(S)
    // Caller supplies f(mask) evaluating the "all conditions in mask hold" count.
    // Example usage:
    //   ll total = 0;
    //   for (int mask = 1; mask < (1<<k); mask++) {
    //       int bits = __builtin_popcount(mask);
    //       ll term = f(mask);
    //       total += (bits % 2) ? term : -term;
    //   }
}

// Matrix: fixed-size square matrix of mint. Mirrors mint's design (data + O(1) ops as
// members, O(log p) ops as members too) instead of free-floating matMul/matPow functions.
struct Matrix {
    vector<vector<mint>> a;
    int n;

    Matrix(int n_) : a(n_, vector<mint>(n_, mint(0))), n(n_) {}

    Matrix(vector<vector<mint>> data) : a(std::move(data)), n((int)a.size()) {}

    vector<mint> &operator[](int i) { return a[i]; }
    const vector<mint> &operator[](int i) const { return a[i]; }

    // O(n^2) | n x n identity matrix
    static Matrix identity(int n) {
        Matrix I(n);
        for (int i = 0; i < n; i++)
            I[i][i] = mint(1);
        return I;
    }

    // O(n^3) | standard matrix multiplication
    Matrix operator*(const Matrix &o) const {
        Matrix c(n);
        for (int i = 0; i < n; i++)
            for (int t = 0; t < n; t++) {
                if (a[i][t].v == 0) continue;
                for (int j = 0; j < n; j++)
                    c[i][j] += a[i][t] * o[t][j];
            }
        return c;
    }

    Matrix &operator*=(const Matrix &o) {
        *this = *this * o;
        return *this;
    }

    // O(n^3 log p) | this matrix raised to the p-th power
    Matrix pow(long long p) const {
        Matrix base = *this;
        Matrix res = identity(n);

        while (p) {
            if (p & 1) res = res * base;
            base = base * base;
            p >>= 1;
        }
        return res;
    }
};
