#include <iostream>

using namespace std;

const int N = 100000;
int tr[N * 32];
int lp[N * 32];
int rp[N * 32];
int root = 0;
int lastn = 1;

void update(int& cur, int l, int r, int pos, int val) {
    if (cur == 0) {
        cur = lastn++;
    }
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
    while(m--) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t == 1) {
            update(root, 0, n - 1, x, y);
        } else {
            cout << get_sum(root, 0, n - 1, x, y) << endl;
        }
    }
    
    
    return 0;
}





