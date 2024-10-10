/*
    Soluție complexitate O((q+n)*sqrt(n))

    Normalizăm elementele vectorului.

    Vom folosi o structură de date ce permite inserarea și ștergerea în O(1)
    și determinarea celui de-al (p - x + 1)-lea element în O(sqrt(n)).

    Această structură este complementară algoritmului lui Mo, unde,
    la fiecare query, avem (în medie) O(sqrt(n)) inserări/ștergeri și
    o singură operație de tipul query.

*/

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

//
class sqrt_data_structure {
private:
    int max_pos, buc_size;
    vector<int> fr, buc_fr;
public:
    sqrt_data_structure(int max_pos) : max_pos(max_pos) {
        buc_size = sqrt(max_pos);
        fr.resize(max_pos + 1);
        buc_fr.resize(max_pos/buc_size + 1);
    }
    void insert(int val) {
        ++fr[val];
        ++buc_fr[val / buc_size];
    }
    void erase(int val) {
        --fr[val];
        --buc_fr[val / buc_size];
    }
    int query(int pos) {
        int buc_no = 0;
        while (buc_fr[buc_no] < pos) {
            pos -= buc_fr[buc_no];
            ++buc_no;
        }
        int val = buc_no * buc_size;
        do {
            pos -= fr[val];
            ++val;
           
        } while (pos > 0);

        return val - 1;
    }
};
//

int main() {
    ifstream  fin("easyxy.in");
    ofstream fout("easyxy.out");

    int n, q;
    fin >> n >> q;

    struct elem {
        int val, ind, norm;
    };
    vector<elem> v(n + 1);
    v[0].val = -1;
    for (int i = 1; i <= n; ++i) {
        fin >> v[i].val;
        v[i].ind = i;
    }
    sort(v.begin() + 1, v.end(), [&] (elem a, elem b) {
        return a.val < b.val;
    });

    vector<int> orig(n + 1);
    int max_val = 0;
    for (int i = 1; i <= n; ++i) {
        if (v[i].val != v[i - 1].val) {
            v[i].norm = ++max_val;
            orig[max_val] = v[i].val;
        }
        else {
            v[i].norm = max_val;
        }
    }
    sort(v.begin() + 1, v.end(), [&] (elem a, elem b) {
        return a.ind < b.ind;
    });

    struct query {
        int x, y, p, ind;
    };
    vector<query> u(q + 1);
    for (int i = 1; i <= q; ++i) {
        fin >> u[i].x >> u[i].y;
        u[i].ind = i;
    }

    int buc_size = sqrt(n);
    sort(u.begin() + 1, u.end(), [&] (query a, query b) {
        if (a.x / buc_size != b.x / buc_size)
            return a.x < b.x;
        return ((a.x / buc_size) % 2) ? (a.y < b.y) : (a.y > b.y);
    });

    vector<int> ans(q + 1);
    sqrt_data_structure s(max_val);
    int l = 1, r = 0;
    for (int i = 1; i <= q; ++i) {
        while (r < u[i].y)
            s.insert(v[++r].norm);
        while (r > u[i].y)
            s.erase(v[r--].norm);
        while (l < u[i].x)
            s.erase(v[l++].norm);
        while (l > u[i].x)
            s.insert(v[--l].norm);
        
        ans[u[i].ind] = orig[s.query((u[i].y - u[i].x + 2) / 2)];
    }
    for (int i = 1; i <= q; ++i)
        fout << ans[i] << "\n";

}