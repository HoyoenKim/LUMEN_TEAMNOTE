#include <iostream>
#include <vector>
#include <string>
// DONE
// LCS, KMP
// TODO
// 보어 무어법
int LeastCommonString() {
    std::string s1;
    std::string s2;
    std::cin >> s1 >> s2;
    int n = s1.length();
    int m = s2.length();
    std::vector<std::vector<int>> array(n, std::vector<int>(m, 0));
    int max = 0;
    for(int i = 0 ; i < n ; i++) {
        for(int j = 0 ; j < m ; j++) {
            if(s1[i] == s2[j]) {
                if(i == 0 || j == 0) {
                    array[i][j] = 1;
                }
                else{
                    array[i][j] = array[i - 1][j - 1] + 1;
                }
            }
            else {
                if(i == 0 || j == 0) {
                    if(i == 0 && j == 0) {
                    
                    }
                    else if(i == 0) {
                        array[i][j] = array[i][j - 1];
                    }
                    else {
                        array[i][j] = array[i - 1][j];
                    }
                }
                else {
                    array[i][j] = std::max(array[i - 1][j], array[i][j - 1]);
                }
            }
            if(array[i][j] > max) {
                max = array[i][j];
            }
        }
    }
    std::cout << max << "\n";
}

std::vector<int> makeTable(std::string p) {
    int pSize = p.length();
    std::vector<int> ret(pSize, 0);
    int j = 0;

    for(int i = 1 ; i < pSize ; i++) {
        while(j > 0 && p[i] != p[j]) {
            j = ret[j - 1];
        }
        if(p[i] == p[j]) {
            j += 1;
            ret[i] = j;
        }
    }

    return ret;
}

int KMP() {
    std::string T, P;
    std::getline(std::cin, T);
    std::getline(std::cin, P);
    
    int Tsize = T.size();
    int Psize = P.size();

    std::vector<int> table = makeTable(P);
    int j = 0;
    int count = 0;
    std::vector<int> index;
    for(int i = 0 ; i < Tsize ; i++) {
        while(j > 0 && T[i] != P[j]) {
            j = table[j - 1];
        }
        if(T[i] == P[j]) {
            if(j == Psize - 1) {
                index.push_back(i - Psize + 2);
                count += 1;
                j = table[j];
            }
            else {
                j++;
            }
        }
    }

    std::cout << count << "\n";
    for(int i = 0 ; i < index.size() ; i++) {
        std::cout << index[i] << " ";
    }
    std::cout << "\n";
}