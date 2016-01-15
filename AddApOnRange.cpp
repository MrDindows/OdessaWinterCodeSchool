#include <iostream>

using namespace std;

long long tr[4 * 101010];
long long a[4 * 101010];
long long d[4 * 101010];

void push(int cur, int l, int r) {
    int m = (l + r) / 2;
    int dcur = cur + cur;
    
    a[dcur] += a[cur];
    d[dcur] += d[cur];
    int lk = m - l + 1;
    tr[dcur] += a[cur] * lk + d[cur] * lk * (lk - 1) / 2;
    
    a[dcur + 1] += a[cur] + lk * d[cur];
    d[dcur + 1] += d[cur];
    int rk = r - m;
    tr[dcur + 1] += (a[cur] + lk * d[cur]) * rk + d[cur] * rk * (rk - 1) / 2;
    
    a[cur] = 0;
    d[cur] = 0;
}
void update(int cur, int l, int r, int x, int y, int qa, int qd) {
    if (y > r) y = r;
    if (x < l) x = l;
    if (x > r || y < l) return;
    
    if (l == x && r == y) {
        a[cur] += qa;
        d[cur] += qd;
        int k = r - l + 1;
        tr[cur] += qa * k + k * (k - 1) / 2 * qd;
    } else {
        int m = (l + r) / 2;
        int dcur = cur + cur;
        
        if (d[cur] != 0 || a[cur] != 0) {
            push(cur, l, r);
        }
        
        update(dcur, l, m, x, y, qa, qd);
        update(dcur + 1, m + 1, r, x, y, qa + qd * max(0, m - x + 1), qd);
        
        tr[cur] = tr[dcur] + tr[dcur + 1];
    }
}

long long get_sum(int cur, int l, int r, int x, int y) {
    if (y > r) y = r;
    if (x < l) x = l;
    if (x > r || y < l) return 0;
    if (x == l && y == r) {
        return tr[cur];
    } else {
        int m = (l + r) / 2;
        int dcur = cur + cur;
        
        if (d[cur] != 0 || a[cur] != 0) {
            push(cur, l, r);
        }
        return get_sum(dcur, l, m, x, y) + get_sum(dcur + 1, m + 1, r, x, y);
    }
}

int main() {
    int n;
    cin >> n;
    
    for (int i = 0; i < n; ++i) {
        int t;
        cin >> t;
        if (t == 1) {
            int x, y, a, d;
            cin >> x >> y >> a >> d;
            update(1, 0, n - 1, x, y, a, d);
        } else {
            int x, y;
            cin >> x >> y;
            cout << get_sum(1, 0, n - 1, x, y) << endl;
        }
    }
    
    
    return 0;
}










