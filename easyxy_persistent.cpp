/*
    Soluție complexitate O((n+q)*log(n))

    Normalizăm elementele vectorului.

    Folosim un arbore de intervale persistent (Persistent Segment Tree).
    Construim n arbori de intervale, fiecare dintre ei reprezentând un vector 
    de frecvență al valorilor din intervalul [1, k].

    Pentru a răspunde la interogările în intervalul [x, y], scădem arborele 
    de frecvențe din intervalul [1, x-1] din arborele de frecvențe din 
    intervalul [1, y], după care căutăm binar răspunsul.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node;
int count(Node *node);
Node *left(Node *node);
Node *right(Node *node);

struct Node {
    Node *left, *right;
    int cnt;
    Node(int cnt = 0) : left(nullptr), right(nullptr), cnt(cnt) {}
    Node(Node *left, Node *right) : left(left), right(right), 
        cnt(count(left) + count(right)) {}
};
int count(Node *node) {
    if (node == nullptr)
        return 0;
    return node->cnt;
}
Node *left(Node *node) {
    if (node == nullptr)
        return nullptr;
    return node->left;
}
Node *right(Node *node) {
    if (node == nullptr)
        return nullptr;
    return node->right;
}
Node *update(Node *node, int l, int r, int pos) {
    if (l == r) {
        return new Node(count(node) + 1);
    }
    
    int mid = (l + r) / 2;
    if (pos <= mid)
        return new Node(update(left(node), l, mid, pos), right(node));
    else
        return new Node(left(node), update(right(node), mid + 1, r, pos));
}

const int NMAX = 100005;
struct elem {
    int val, ind, norm;
};
vector<elem> v(NMAX);

int query(Node *left_tree, Node *right_tree, int l, int r, int val) {
    if (l == r) 
        return l;

    int mid = (l + r) / 2;
    int cnt_left = count(left(right_tree)) - count(left(left_tree));
    if (val <= cnt_left) {
        return query(left(left_tree), left(right_tree), l, mid, val);
    }
    else {
        return query(right(left_tree), right(right_tree), mid + 1, r, 
            val - cnt_left);
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
    sort(v.begin() + 1, v.begin() + n + 1, [&] (elem a, elem b) {
        return a.ind < b.ind;
    });

    vector<Node*> root(n + 1);
    root[0] = nullptr;
    for (int i = 1; i <= n; ++i) {
        root[i] = update(root[i - 1], 1, n, v[i].norm);
    }
    while (q--) {
        int x, y, p;
        fin >> x >> y >> p;
        fout << orig[query(root[x - 1], root[y], 1, n, p - x + 1)] << "\n";
    }
}
