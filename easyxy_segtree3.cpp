/*
    Soluție complexitate O(q*log^2(n))

    Sortăm vectorul. Pentru fiecare element din vectorul sortat ținem
    minte și când a fost introdus (indicele acestuia).

    Răspunsul la query-uri este al (p - x + 1)-lea indice care se află în
    intervalul [x, y], numărând de la începutul listei.

    Pentru a găsi acest indice eficient, vom utiliza o căutare binară 
    în cadrul unui Merge Sort Tree construit cu indicii din listă.
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
    int val, ind;
};
vector<elem> v(NMAX);

void build_tree(int node, int l, int r) {
    if (l == r) {
        seg_tree[node].push_back(v[l].ind);
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
    if (l == r)
        return v[l].val;

    int mid = (l + r) / 2;
    int cnt_left = upper_bound(seg_tree[2 * node].begin(), seg_tree[2 * node].end(), y)
        - lower_bound(seg_tree[2 * node].begin(), seg_tree[2 * node].end(), x);
    if (val <= cnt_left) {
        return query(2 * node, l, mid, x, y, val);
    }
    else {
        return query(2 * node + 1, mid + 1, r, x, y, val - cnt_left); 
    }
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

    build_tree(1, 1, n);

    while (q--) {
        int x, y, p;
        fin >> x >> y >> p;
        fout << query(1, 1, n, x, y, p - x + 1) << "\n";
    }
}
