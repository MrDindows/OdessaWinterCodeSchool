#include <iostream>
using namespace std;

const int N = 100001;
long long tr[N * 4];
int a[N];

void build(int cur, int l, int r) {
    if (l == r) {
        tr[cur] = a[l];
    } else {
        int m = (l + r) / 2;
        int dcur = cur + cur;
        build(dcur, l, m);
        build(dcur + 1, m + 1, r);
        
        tr[cur] = tr[dcur] + tr[dcur + 1];
    }
}

void update(int cur, int l, int r, int pos, int val) {
    if (l == r) {
        tr[cur] = val;
    } else {
        int m = (l + r) / 2;
        int dcur = cur + cur;
        if (pos <= m) {
            update(dcur, l, m, pos, val);
        } else {
            update(dcur + 1, m + 1, r, pos, val);
        }
        
        tr[cur] = tr[dcur] + tr[dcur + 1];
    }
}

long long get_sum(int cur, int l, int r, int x, int y) {
    if (l == x && r == y) {
        return tr[cur];
    } else {
        int m = (l + r) / 2;
        int dcur = cur + cur;
        if (y <= m) {
            return get_sum(dcur, l, m, x, y);
        } else if (x > m) {
            return get_sum(dcur + 1, m + 1, r, x, y);
        } else {
            return get_sum(dcur, l, m, x, m) + get_sum(dcur + 1, m + 1, r, m + 1, y);
        }
    }
}

int main() {
    
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    build(1, 0, n - 1);
    int m;
    cin >> m;
    for (int i = 1; i <= m; ++i) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t == 1) {
            update(1, 0, n - 1, x, y);
        } else {
            cout << get_sum(1, 0, n - 1, x, y) << endl;
        }
    }
    
    return 0;
}





