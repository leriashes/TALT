#pragma once
#include "defs.h"


class TScanner
{
private:
	IM text;		//�������� �����
	int uk;			//��������� ������� ������� � �������� ������

public:
	void PutUK(int i);
	int GetUK();

	void PrintError(string, string);
	int Scanner(LEX lex);
	void GetData(string);

	TScanner(string);
	~TScanner() {}
};

