#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_set>
#include <stdlib.h>
#include <string>
#include <vector>

#include "cykTable.h"

bool isVariablePair(string pair){
    return (isupper(pair[0])) && (isupper(pair[1])) && (pair.length() == 2);
}

pair <bool, CYKTable> cyk(string inputString,
                          map < string, unordered_set<string> > grammar){
    bool accepted = false;
    CYKTable table = (inputString);
    int n = inputString.length();

    for (int i = 0; i < n; i++) {
        string str(1,inputString[i]);
        for(auto production : grammar){
            unordered_set<string> stringSet = grammar[production.first];
            if (stringSet.find(str) != stringSet.end())
                table.addToEntry(production.first, i+1, i);
            else
                continue;
        }
    }

    std::cout << table.toString() << '\n';

    for (int m = 2; m <= n; m++) {
        for (int i = 0; i <= n-m; i++) {
            for (int j = 1; j < m; j++) {

                string strB = inputString.substr(i, j);
                string strC = inputString.substr(i+j, m-j);

                for(auto production : grammar){
                    unordered_set<string> stringSet = grammar[production.first];
                    unordered_set<string>::iterator it;
                    for (it = stringSet.begin(); it != stringSet.end(); ++it) {
                        if (isVariablePair(*it)) {
                            string pair = *it;
                            string prodB(1, pair[0]);
                            string prodC(1, pair[1]);

                            if ((table.isInEntry(prodB, i+j, i)) && (table.isInEntry(prodC, i+m, i+j))) {
                                table.addToEntry(production.first, i+m, i);
                            }

                        }
                    }
                }
            }
        }
    }
    std::cout << table.toString() << '\n';
    return make_pair(accepted, table);
}
