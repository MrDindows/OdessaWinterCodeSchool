#include <iostream>
#include <algorithm>
using namespace std;

const int N = 100001;

int tr[N * 32];
int lp[N * 32];
int rp[N * 32];
int root[N];
int lastn = 1;

void update(int& cur, int l, int r, int pos) {
    tr[lastn] = tr[cur];
    lp[lastn] = lp[cur];
    rp[lastn] = rp[cur];
    cur = lastn++;
    
    if (l == r) {
        tr[cur]++;
    } else {
        int m = (l + r) / 2;
        if (pos <= m) {
            update(lp[cur], l, m, pos);
        } else {
            update(rp[cur], m + 1, r, pos);
        }
        tr[cur] = tr[lp[cur]] + tr[rp[cur]];
    }
}

int get(int cur1, int cur2, int l, int r, int k) {
    int leftSize = tr[lp[cur1]] - tr[lp[cur2]];
    if (l == r) return l;
    int m = (l + r) / 2;
    if (leftSize >= k) {
        return get(lp[cur1], lp[cur2], l, m, k);
    } else {
        return get(rp[cur1], rp[cur2], m + 1, r, k - leftSize);
    }
}

const int K = 1000000000;
int getKthOnRange(int l, int r, int k) {
    return get(root[r + 1], root[l], 0, K, k);
}

int a[N];
int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    for (int i = 0; i < n; ++i) {
        root[i + 1] = root[i];
        update(root[i + 1], 0, K, a[i]);
    }
    
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        int l, r, k;
        cin >> l >> r >> k;
        --l; --r;
        cout << getKthOnRange(l, r, k) << '\n';
    }
    
    
    return 0;
}


