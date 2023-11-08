#include <iostream>
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

    TDiagram* diag = new TDiagram(scan);
    diag->S();

    type = scan->Scanner(lex);

    if (type != TEnd)
    {
        scan->PrintError("Лишний текст в конце программы");
    }
    else
    {
        printf("\n\nОшибки не найдены.\n");
    }

    printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
    diag->PrintTree();
    printf("\n\n");

    return 0;
}
