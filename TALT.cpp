#include <iostream>
#include "defs.h"
#include "Scanner.h"


int main()
{
    setlocale(LC_ALL, "rus");

    TScanner* scan;
    int type;
    LEX lex;

    scan = new TScanner("input.txt");

    do {
        type = scan->Scanner(lex);
        cout << lex << " - тип " << type << endl;
    } while (type != TEnd);

    return 0;
}
