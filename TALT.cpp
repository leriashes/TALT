﻿#include <iostream>
#include "defs.h"
#include "Scanner.h"
#include "Diagram.h"

int main()
{
    setlocale(LC_ALL, "rus");

    TScanner* scan;
    int type;
    LEX lex;

    char filename[] = "input.txt";

    scan = new TScanner(filename);

    /*do {
        type = scan->Scanner(lex);
        cout << lex << " - тип " << type << endl;
    } while (type != TEnd);
    */

    TDiagram* diag = new TDiagram(scan);
    diag->S();

    return 0;
}
