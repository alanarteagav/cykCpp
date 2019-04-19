#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_set>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class CYKTable {
private:
    vector< vector<unordered_set <string> > >  table;

public:
    CYKTable (string input){
        vector< vector<unordered_set <string> > > newTable(input.length());
        for (int i = 0; i < input.length(); i++) {
            vector<unordered_set <string> > step(i+1);
            newTable[i] = step;
        }
        table = newTable;
    }

    bool isInEntry (string str, int i, int j){
        unordered_set <string> stringSet = table[i-1][j];
        if (stringSet.find(str) != stringSet.end())
            return true;
        else
            return false;
    }

    string getEntry (int i, int j){
        string entry;
        unordered_set <string> stringSet = table[i-1][j];

        unordered_set<string>::iterator it;
        for (it = stringSet.begin(); it != stringSet.end(); ++it) {
            entry += *it + " : ";
        }
        return entry;
    }

    void addToEntry (string str, int i, int j){
        table[i-1][j].insert(str);
    }

    string toString(){
        string str;
        unordered_set<string>::iterator it;
        for (auto vec : table){
            for(auto stringSet : vec){
                str += "| ";
                for (it = stringSet.begin(); it != stringSet.end(); ++it) {
                    str += *it + " ";
                }
            }
            str += "| \n";
        }
        return str;
    }
};
