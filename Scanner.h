#pragma once
#include "defs.h"


class TScanner
{
private:
	IM text;		//�������� �����
	int uk;			//��������� ������� ������� � �������� ������
	int line;		//������
	int pos;		//�������

public:
	void PutUK(int i);
	int GetUK();

	void SetLine(int i);
	void NewLine();
	int GetLine();

	void SetPos(int i);
	int GetPos();

	void PrintError(const char*, char*);
	void PrintError(const char*);
	int Scanner(LEX lex);
	void GetData(char*);

	TScanner(char*);
	~TScanner() {}
};

