#include <iostream>

#define MaxText 100000      //макисмальная длина текста ИМ
#define MaxLex 20           //максимальная длина лексемы
typedef char IM[MaxText];   //текст ИМ
typedef char LEX[MaxLex];   //лексема


#define TKInt       1
#define TKShort     2
#define TKLong      3
#define TKDouble    4
#define TKMain      5
#define TKReturn    6
#define TKWhile     7
#define TKBreak     8

#define TIdent      20

#define TConstInt       30
#define TConstDouble    31

#define TComma      40
#define TSemicolon  41
#define TLS         42
#define TRS         43
#define TFLS        44
#define TFRS        45

#define TSave       50
#define TEq         51
#define TNEq        52
#define TLT         53
#define TGT         54
#define TLE         55
#define TGE         56
#define TLShift     57
#define TRShift     58
#define TPlus       59
#define TMinus      60
#define TMult       61
#define TDiv        62
#define TMod        63

#define TEnd        100
#define TError      200



IM t;           //исходный модуль
int uk;         //указатель текущей позиции в ИМ
int line, pos;  //строка, позиция

int typ;        //тип лексемы
LEX l;          //изображение лексемы

int Scaner(LEX lex)
{
    int type;       //тип лексемы
    int i;          //текущая длина лексемы

    for (i = 0; i < MaxLex; i++) lex[i] = 0;    //очистка поля лексемы

    i = 0;
    while ((t[uk] == ' ') || (t[uk] == '\n') || (t[uk] == '\t')) uk++;      //пропуск незначащих элементов

    if (t[uk] == '\0')
    {
        lex[0] = t[uk];
        return TEnd;
    }
    else
    {
        return 0;
    }

    return type;
}



int main()
{
    std::cout << "Hello World!\n";
}
