# include <iostream>
using namespace std;

const int N = 100001;
int tr[N * 20];
int lp[N * 20];
int rp[N * 20];
int roots[N];
int lastn = 1;

void update(int& cur, int l, int r, int pos, int val) {
    lp[lastn] = lp[cur];
    rp[lastn] = rp[cur];
    tr[lastn] = tr[cur];
    cur = lastn;
    
    lastn++;
    if (l == r) {
        tr[cur] = val;
    } else {
        int m = (l + r) / 2;
        if (pos <= m) {
            update(lp[cur], l, m, pos, val);
        } else {
            update(rp[cur], m + 1, r, pos ,val);
        }
        tr[cur] = tr[lp[cur]] + tr[rp[cur]];
    }
}

int get_sum(int cur, int l, int r, int x, int y) {
    if (cur == 0) {
        return 0;
    }
    if (l == x && r == y) {
        return tr[cur];
    } else {
        int m = (l + r) / 2;
        if (y <= m) {
            return get_sum(lp[cur], l, m, x, y);
        } else if (x > m) {
            return get_sum(rp[cur], m + 1, r, x, y);
        } else {
            return get_sum(lp[cur], l, m, x, m) + get_sum(rp[cur], m + 1, r, m + 1, y);
        }
    }
}
int main() {
    
    
    int n;
    cin >> n;
    int m;
    cin >> m;
    for (int i = 1; i <= m; ++i) {
        roots[i] = roots[i - 1];
        int x, y;
        cin >> x >> y;
        update(roots[i], 0, n - 1, x, y);
    }
    int q;
    cin >> q;
    while(q--) {
        int v, x, y;
        cin >> v >> x >> y;
        cout << get_sum(roots[v], 0, n - 1, x, y) << endl;
    }
    
    
    return 0;
}





