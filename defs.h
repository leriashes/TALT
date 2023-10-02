#pragma once
#include <iostream>
#include <fstream>
using namespace std;

#define MAX_TEXT 100000     //макисмальная длина текста ИМ
#define MAX_LEX 100         //максимальная длина лексемы
#define MAX_KEYW 8			//число ключевых слов

typedef char IM[MAX_TEXT];   //текст ИМ
typedef char LEX[MAX_LEX];   //лексема

//ключевые слова
#define TInt       1
#define TShort     2
#define TLong      3
#define TFloat    4
#define TMain      5
#define TReturn    6
#define TWhile     7
#define TBreak     8

//идентификаторы
#define TIdent      20

//константы
#define TConstInt       30
#define TConstFloat    31

//специальные знаки
#define TComma      40
#define TSemicolon  41
#define TLS         42
#define TRS         43
#define TFLS        44
#define TFRS        45

//знаки операций
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

//конец исходного модуля
#define TEnd        100

//ошибочный символ
#define TError      200