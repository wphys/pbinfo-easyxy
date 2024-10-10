/*
    Soluție complexitate O(n*q)
    Pentru fiecare query, folosim algoritmul Quick Select
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
        
        nth_element(tmp.begin(), tmp.begin() + p - x, tmp.end());
        fout << tmp[p - x] << "\n";
    }
}
