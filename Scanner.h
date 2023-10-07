#pragma once
#include "defs.h"


class TScanner
{
private:
	IM text;		//исходный текст
	int uk;			//указатель текущей позиции в исходном тексте

public:
	void PutUK(int i);
	int GetUK();

	void PrintError(const char*, char*);
	int Scanner(LEX lex);
	void GetData(char*);

	TScanner(char*);
	~TScanner() {}
};

