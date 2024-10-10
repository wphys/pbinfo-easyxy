/*
    Soluție complexitate O(n*q*log(n))
    Pentru fiecare query, sortăm intervalul [x, y] și afișăm al p-lea element
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ifstream  fin("easyxy.in");
    ofstream fout("easyxy.out");

    int n, q;
    fin >> n >> q;

    vector<int> v(n + 1);
    for (int i = 1; i <= n; ++i) 
        fin >> v[i];

    while (q--) {
        int x, y, p;
        fin >> x >> y >> p;
        vector<int> tmp(v.begin() + x, v.begin() + y + 1);
        sort(tmp.begin(), tmp.end());
        fout << tmp[p - x] << "\n";
    }
}
