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

// Función auxiliar que determina si una producción es de la forma A -> BC
bool isVariablePair(string pair){
    return (isupper(pair[0])) && (isupper(pair[1])) && (pair.length() == 2);
}

// Implementación del algoritmo CYK.
// Recibe como argumentos una cadena y una gramática libre de contexto en forma
// normal de Chomsky (mediante un diccionario cuyas llaves son las variables
// del lado derecho de las producciones y los valores son conjuntos sin orden
// de producciones, para acceder a ellos en tiempo O(1) ).
//
// Regresa un par booleano, tabla CYK, con el booleano indicando si la cadena es
// generada por el lenguaje y la tabla representando las posibles derivaciones
// para la cadena dada la gramática.
pair <bool, CYKTable> cyk(string inputString,
                          map < string, unordered_set<string> > grammar){
    bool accepted = false;
    CYKTable table = (inputString);
    int n = inputString.length();
    // Rellena las producciones para subcadenas unitarias.
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
    // Rellena las producciones para subcadenas de tamaño 2 <= k <= n
    // con k el tamaño de la subcadena y n el tamaño de la cadena de entrada.
    for (int m = 2; m <= n; m++)
        for (int i = 0; i <= n-m; i++)
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

                            if ((table.isInEntry(prodB, i+j, i))
                            && (table.isInEntry(prodC, i+m, i+j)))
                                table.addToEntry(production.first, i+m, i);
                        }
                    }
                }
            }
    // Determina si la cadena es aceptada, dado si el símbolo 'S' figura en la
    // entrada (0,n) de la tabla CYK, con n el tamaño de la cadena de entrada.
    accepted = table.isInEntry("S", n, 0);
    return make_pair(accepted, table);
}
