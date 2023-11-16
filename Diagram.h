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
	void V(NData* res);		//выражение
	void Z(NData* res);		//сравнение
	void M(NData* res);		//множитель
	void Y(NData* res);		//сдвиг
	void L(NData* res);		//слагаемое
	void N(NData* res);		//со знаком
	void K(NData* res);		//вызов функции

public:
	TDiagram(TScanner* s) { scan = s; root = new Tree(scan); }
	~TDiagram() {}

	void PrintTree();

	void S();		//программа
};

