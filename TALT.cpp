#include <iostream>
#include "defs.h"
#include "Scanner.h"


int main()
{
    setlocale(LC_ALL, "rus");

    TScanner* scan;
    int type;
    LEX lex;

    char filename[] = "input.txt";

    scan = new TScanner(filename);

    do {
        type = scan->Scanner(lex);
        cout << lex << " - тип " << type << endl;
    } while (type != TEnd);

    return 0;
}
