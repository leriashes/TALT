#pragma once
#include "Scanner.h"
#include "Semant.h"

class TDiagram
{

	LEX OP_Name[13] = { "==", "!=", "<", ">", "<=", ">=",
						"<<", ">>", "+", "-", "*", "/", "%" };

private:
	TScanner* scan;
	Tree* root;

	int LookForward(int k);

	void D();		//описание данных
	void F();		//функция
	void Q();		//составной оператор
	void A();		//оператор
	void W();		//while
	void P();		//присваивание
	void R();		//return
	void B();		//break
	void V(DATA_TYPE* resType);		//выражение
	void Z(DATA_TYPE* resType);		//сравнение
	void M(DATA_TYPE* resType);		//множитель
	void Y(DATA_TYPE* resType);		//сдвиг
	void L(DATA_TYPE* resType);		//слагаемое
	void N(DATA_TYPE* resType);		//со знаком
	void K(DATA_TYPE* resType);		//вызов функции

public:
	TDiagram(TScanner* s) { scan = s; root = new Tree(scan); }
	~TDiagram() {}

	void PrintTree();

	void S();		//программа
};

