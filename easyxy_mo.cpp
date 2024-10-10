/*
    Soluție complexitate O((q+n)*sqrt(n)*log(n))

    Normalizăm elementele vectorului.

    Pentru fiecare interval, vom folosi un arbore indexat binar pentru
    statistici de ordine (arborele indexat binar este folosit pe post 
    de vector de frecvență, iar a (p - x + 1) valoare se găsește prin
    căutare binară).

    Rezolvăm query-urile offline prin intermediul algoritmului lui Mo.

*/

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

//
template <typename T>
class fenwick_tree {
private:
    int max_pos;
    vector<T> s;
public:
    fenwick_tree(int max_pos) : max_pos(max_pos) {
        s.resize(max_pos + 1);
    }
    void add(int pos, T val) {
        for(; pos <= max_pos; pos += pos & -pos) {
            s[pos] += val;
        }
    }
    T sum(int pos) {
        T ans = 0;
        for(; pos; pos -= pos & -pos) {
            ans += s[pos];
        }
        return ans;
    }
    int binary_search(T val) {
        int bit, ans = 0;
        for (bit = 1; bit <= max_pos; bit <<= 1);
        for (; bit; bit >>= 1) {
            int curr_pos = ans + bit;
            if (curr_pos <= max_pos && val > s[curr_pos]) {
                val -= s[curr_pos];
                ans = curr_pos;
            }
        }
        if (ans == max_pos)
            return -1;
        return ans + 1;
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
        fin >> u[i].x >> u[i].y >> u[i].p;
        u[i].ind = i;
    }

    int buc_size = sqrt(n);
    sort(u.begin() + 1, u.end(), [&] (query a, query b) {
        if (a.x / buc_size != b.x / buc_size)
            return a.x < b.x;
        return ((a.x / buc_size) % 2) ? (a.y < b.y) : (a.y > b.y);
    });

    vector<int> ans(q + 1);
    fenwick_tree<int> fr(max_val);
    int l = 1, r = 0;
    for (int i = 1; i <= q; ++i) {
        while (r < u[i].y)
            fr.add(v[++r].norm, 1);
        while (r > u[i].y)
            fr.add(v[r--].norm, -1);
        while (l < u[i].x)
            fr.add(v[l++].norm, -1);
        while (l > u[i].x)
            fr.add(v[--l].norm, 1);
        
        ans[u[i].ind] = orig[fr.binary_search(u[i].p - u[i].x + 1)];
    }
    for (int i = 1; i <= q; ++i)
        fout << ans[i] << "\n";

}