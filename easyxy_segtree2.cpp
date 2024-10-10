/*
    Soluție complexitate O(q*log^3(n))

    La fel ca soluția precedentă, dar normalizăm elementele vectorului 

    Construim un arbore Merge Sort Tree cu elementele vectorului
    Acest arbore ne permite să aflăm câte elemente dintr-un interval 
    sunt mai mici decât un număr k.

    Pentru fiecare query, căutam binar valoarea celul de-al p-lea element 
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const int NMAX = 100005;
const int LOGMAX = 19;
vector<int> seg_tree[4 * NMAX];

struct elem {
    int val, ind, norm;
};
vector<elem> v(NMAX);

void build_tree(int node, int l, int r) {
    if (l == r) {
        seg_tree[node].push_back(v[l].norm);
        return ;
    }
    int mid = (l + r) / 2;
    build_tree(2 * node, l, mid);
    build_tree(2 * node + 1, mid + 1, r);
    merge(seg_tree[2 * node].begin(), seg_tree[2 * node].end(), 
        seg_tree[2 * node + 1].begin(), seg_tree[2 * node + 1].end(),
        back_inserter(seg_tree[node]));
    
}
int query(int node, int l, int r, int x, int y, int val) {
    if (r < x || l > y)
        return 0;
    if (x <= l && r <= y) {
        return upper_bound(seg_tree[node].begin(), seg_tree[node].end(), val) 
            - seg_tree[node].begin();
    }
    int mid = (l + r) / 2;
    return query(2 * node, l, mid, x, y, val) + query(2 * node + 1, mid + 1, r, x, y, val);
}
int main() {
    ifstream  fin("easyxy.in");
    ofstream fout("easyxy.out");

    int n, q;
    fin >> n >> q;

    v[0].val = -1;
    for (int i = 1; i <= n; ++i) {
        fin >> v[i].val;
        v[i].ind = i;
    }
    sort(v.begin() + 1, v.begin() + n + 1, [&] (elem a, elem b) {
        return a.val < b.val;
    });

    vector<int> orig(n + 1);
    int min_val = 1, max_val = 0;
    for (int i = 1; i <= n; ++i) {
        if (v[i].val != v[i - 1].val) {
            v[i].norm = ++max_val;
            orig[max_val] = v[i].val;
        }
        else {
            v[i].norm = max_val;
        }
    }
    sort(v.begin() + 1, v.begin() + n + 1, [&] (elem a, elem b) {
        return a.ind < b.ind;
    });

    build_tree(1, 1, n);

    while (q--) {
        int x, y, p;
        fin >> x >> y >> p;
        int l = min_val, r = max_val;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (query(1, 1, n, x, y, mid) < p - x + 1) {
                l = mid + 1;
            }
            else {
                r = mid - 1;
            }
        }
        fout << orig[l] << "\n";
    }
}
