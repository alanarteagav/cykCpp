#include <iterator>
#include <map>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

// Clase tabla CYK, permite abstraer la tabla de posibles derivaciones para la
// ejecución del algoritmo CYK.
class CYKTable {
private:
    // La tabla CYK, implementada como un vector multidimensional, cuyos
    // elementos son conjuntos sin orden.
    vector< vector<unordered_set <string> > >  table;

public:
    // Constructor de la tabla CYK. Recibe una cadena a partir de la cual se
    // inicializa la tabla con conjuntos vacíos.
    CYKTable (string input){
        vector< vector<unordered_set <string> > > newTable(input.length());
        for (int i = 0; i < input.length(); i++) {
            vector<unordered_set <string> > step(i+1);
            newTable[i] = step;
        }
        table = newTable;
    }

    // Función que determina si una cadena figura en el conjunto subyaciente a
    // cierta entrada de la tabla.
    bool isInEntry (string str, int i, int j){
        unordered_set <string> stringSet = table[i-1][j];
        if (stringSet.find(str) != stringSet.end())
            return true;
        else
            return false;
    }

    // Función que añade una cadena figura al conjunto subyaciente a
    // cierta entrada de la tabla.
    void addToEntry (string str, int i, int j){
        table[i-1][j].insert(str);
    }

    // Función que reegresa la representación en cadena de la tabla CYK.
    string toString(){
        string str;
        unordered_set<string>::iterator it;
        for (auto vec : table){
            str += "[";
            for(auto stringSet : vec){
                if (stringSet.empty()) {
                    str += " Ø , ";
                } else {
                    str += "[";
                    for (it = stringSet.begin(); it != stringSet.end(); ++it)
                        str += *it + ", ";
                    str.pop_back();
                    str.pop_back();
                    str += "], ";
                }
            }
            str.pop_back();
            str.pop_back();
            str += "]\n";
        }
        return str;
    }
};
