#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_set>
#include <stdlib.h>
#include <string>
#include <vector>

#include "cyk.h"

using namespace std;

// Función auxiliar que termina el programa, enviando un ensaje que recibe
// como parámetro.
void exitProgram(string message) {
    cout << message << '\n';
    exit(1);
}

// Función que recibe el nombre de un archivo, y regresa un vector con todas
// las reglas de derivación de la gramática.
vector<string> readFile(string fileString) {
    vector<string> rules;
    ifstream grammarFile(fileString);
    string rule;
    if (grammarFile.is_open()) {
        while (getline(grammarFile, rule)) {
            rules.push_back(rule);
        }
        grammarFile.close();
        return rules;
    } else {
        return rules;
    }
}

// Función auxiliar que permite dividir una cadena según otra cadena que actúa
// como delimitador.
vector<string> splitString (string str, string delimiter){
    vector<string> splittedString;
    size_t position = 0;
    string slice;
    while ((position = str.find(delimiter)) != string::npos) {
        slice = str.substr(0, position);
        splittedString.push_back(slice);
        str.erase(0, position + delimiter.length());
    }
    splittedString.push_back(str);
    return splittedString;
}

// Función auxiliar que permite verificar si los símbolos terminales son
// válidos.
void verifyTerminal(string terminal) {
    for (char c : terminal) {
        if (isalpha(c) == 0 && isdigit(c) == 0)
            exitProgram("GRAMATICA INVÁLIDA: terminal inválido");
    }
}

// Función que obtiene una gramática en forma normal de Chomsky, verificando que
// la gramática sea válida, de lo contrario, el programa termina.
map< string, unordered_set<string> > getGrammar(vector<string> rules){
    map< string, unordered_set<string> > grammar;
    bool hasInitial = false;
    for (auto line : rules) {
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        if (line.find("->") == string::npos)
            exitProgram("GRAMATICA INVÁLIDA: existe regla sin símbolo '->'");
        vector<string> rule = splitString(line, "->");
        string var = rule[0];
        if (!isupper(var[0]) || var.length() != 1)
            exitProgram("GRAMATICA INVÁLIDA: símbolo no terminal inválido");
        if (var.compare("S") == 0)
            hasInitial = true;
        vector<string> prods = splitString(rule[1], "|") ;
        for(string prod : prods){
            verifyTerminal(prod);
            grammar[var].insert(prod);
        }
    }
    if (!hasInitial)
        exitProgram("GRAMATICA INVÁLIDA: no se encontró símbolo inicial");
    return grammar;
}

int main(int argc, char const *argv[]) {
    // El nombre del archivo que contiene la gramática.
    string fileString = "";
    // La cadena a verificar.
    string inputString = "";

    // Se recibe la entrada.
    cout << "Archivo con la gramática: ";
    cin >> fileString;
    cout << "Cadena a evaluar: ";
    cin >> inputString;

    // Se obtienen las reglas de la gramática a partir del archivo.
    vector<string> rules = readFile(fileString);
    // Se obtienen las producciones de las reglas.
    map< string, unordered_set<string> > grammar = getGrammar(rules);
    // Se genera la tabla CYK para la cadena.
    CYKTable table = (inputString);
    // Se ejecuta el algoritmo CYK con la cadena y la gramática.
    pair <bool, CYKTable> result = cyk(inputString, grammar);
    // Se imprime el resultado.
    cout << "Resultado:" << '\n';
    if (result.first)
        cout << inputString + " está en L(G)." << "\n\n";
    else
        cout << inputString + " no está en L(G)." << "\n\n";
    cout << "La tabla de posibles derivaciones es:" << '\n';
    cout << result.second.toString() << '\n';
    // Si la cadena es acepada, se imprimen las derivaciones más a la izquierda.
    if (result.first) {
        cout << "Derivación mas a la izquierda:" << '\n';
        cout << leftmost(inputString, result.second, grammar) << '\n';
    }
    return 0;
}
