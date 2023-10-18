#pragma once
#include "Scanner.h"

class TDiagram
{
private:
	TScanner* scan;

	void D();		//описание данных
	void F();		//функция
	void Q();		//составной оператор
	void A();		//оператор
	void W();		//while
	void P();		//присваивание
	void R();		//return
	void B();		//break
	void V();		//выражение
	void Z();		//сравнение
	void M();		//множитель
	void Y();		//сдвиг
	void L();		//слагаемое
	void N();		//со знаком
	void K();		//вызов функции

public:
	TDiagram(TScanner* s) { scan = s; }
	~TDiagram() {}

	void S();		//программа
};

