//
// Created by Arrab on 4/29/2026.
#include <bits/stdc++.h>
using namespace std;

struct Node{
    int val;
    bool lazy;
    int prp;
    Node(){};
    // don't forget to set the base
    Node(int a){

    }
};
struct segmentTree{
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((l + r) / 2)
private:
    // don't forget to set the apply function and the base node
    // base node must be something won't effect your segment
    int n , sz;
    vector<Node> seg;
    Node base;
    Node merge(const Node &a , const Node& b){
        Node ret;
        return ret;
    }
    void build(int l, int r, int node, vector<int> &arr) {
        if (l >=n) return;
        if (l == r) {
            seg[node].val = arr[l];
            return;
        }
        build(l, mid, L, arr);
        build(mid + 1, r, R, arr);
        seg[node] = merge(seg[L],seg[R]);
    }
    void build (int l , int r , int node){
        if (l == r){
            seg[node].val = 0;
            return;
        }
        build(l,mid,L);
        build(mid+1,r,R);
        seg[node] = merge(seg[L],seg[R]);
    }
    Node querySeg(int l, int r, int node, int lq, int rq){
        if (seg[node].lazy) apply(l,r,node);
        if (l > rq || r < lq) return base;
        if (l >= lq && r <= rq) return seg[node];
        Node left = querySeg(l, mid, L, lq, rq);
        Node right = querySeg(mid + 1, r, R, lq, rq);
        return merge(left,right);
    }
    void apply(int l , int r, int node){
        if (!seg[node].lazy) return;

        if (l != r){
            seg[L].lazy = seg[R].lazy = true;

        }

        seg[node].lazy = false;
        seg[node].prp = 0;
    }
    void updateSeg(int l , int r, int node , int lq ,int rq, int val){
        if (seg[node].lazy) apply(l,r,node);
        if (l > rq || r < lq) return;
        if (l >= lq && r <= rq){
            seg[node].lazy = true;
            seg[node].prp += val;
            apply(l,r,node);
            return;
        }
        // left
        updateSeg(l,mid,L,lq,rq,val);
        // right
        updateSeg(mid+1,r,R,lq,rq,val);
        seg[node] = merge(seg[L],seg[R]);
    }
public:
    segmentTree(vector<int>& arr){
        n = arr.size();
        sz = 1;
        while (sz < n) sz<<=1;
        seg.assign(sz*2,base);
        build(0,sz-1,0,arr);
    }
    segmentTree(int _n){
        n = _n;
        sz = 1;
        while (sz < n) sz<<=1;
        seg.assign(sz*2,base);
        build(0,sz-1,0);
    }
    Node query (int idx){
        return query(idx,idx);
    }
    Node query(int l , int r){
        return querySeg(0,sz-1,0,l,r);
    }
    void update(int idx,int val){
        update(idx,idx,val);
    }
    void update(int l , int r,int val){
        updateSeg(0,sz-1,0,l,r,val);
    }
#undef L
#undef R
#undef mid
};



struct Fenwick{
#define o(one) (one &(-one))
    int n;
    vector<int> Bit;
    Fenwick(int _n): n (_n){
        Bit.resize(_n);
    }

    void update(int idx, int val){
        while (idx < n){
            Bit[idx] = Bit[idx] + val;
            idx += o(idx);
        }
    }
    int query(int idx){
        int ret = 0;
        while (idx >0){
            ret += Bit[idx];
            idx -= o(idx);
        }
        return ret;
    }
};
//
namespace subproblem{
struct PersistentSegTree {
    struct Node {
        int sum = 0;
        int left = -1, right = -1;
    };

    vector<Node> tree;
    vector<int> root;
    int n;

    PersistentSegTree(int n_) : n(n_) {
        tree.reserve(4 * n * 20);
        root.push_back(build(0, n - 1));
    }

    int build(int lo, int hi) {
        int idx = tree.size();
        tree.push_back(Node());
        if (lo == hi) return idx;
        int mid = (lo + hi) / 2;
        tree[idx].left = build(lo, mid);
        tree[idx].right = build(mid + 1, hi);
        return idx;
    }

    int update(int prevIdx, int lo, int hi, int pos, int val) {
        int idx = tree.size();
        tree.push_back(tree[prevIdx]);
        if (lo == hi) {
            tree[idx].sum = val;
            return idx;
        }
        int mid = (lo + hi) / 2;
        if (pos <= mid)
            tree[idx].left = update(tree[prevIdx].left, lo, mid, pos, val);
        else
            tree[idx].right = update(tree[prevIdx].right, mid + 1, hi, pos, val);
        tree[idx].sum = tree[tree[idx].left].sum + tree[tree[idx].right].sum;
        return idx;
    }

    int query(int idx, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return 0;
        if (l <= lo && hi <= r) return tree[idx].sum;
        int mid = (lo + hi) / 2;
        return query(tree[idx].left, lo, mid, l, r) +
               query(tree[idx].right, mid + 1, hi, l, r);
    }

    void addVersion(int prevVersion, int pos, int val) {
        root.push_back(update(root[prevVersion], 0, n - 1, pos, val));
    }

    int query(int version, int l, int r) {
        return query(root[version], 0, n - 1, l, r);
    }
};

struct DistinctCounter {
    PersistentSegTree pst;
    int n;
    vector<int> versionAfter;

    DistinctCounter(const vector<int> &arr) : pst(arr.size()), n(arr.size()) {
        unordered_map<int, int> lastPos;
        versionAfter.resize(n);
        int curVersion = 0;

        for (int i = 0; i < n; i++) {
            int val = arr[i];

            if (lastPos.count(val)) {
                int p = lastPos[val];
                pst.addVersion(curVersion, p, 0);
                curVersion = pst.root.size() - 1;
            }

            pst.addVersion(curVersion, i, 1);
            curVersion = pst.root.size() - 1;

            versionAfter[i] = curVersion;
            lastPos[val] = i;
        }
    }

    int countDistinct(int l, int r) {
        return pst.query(versionAfter[r], l, r);
    }
};

}
