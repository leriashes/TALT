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

	void D();		//�������� ������
	void F();		//�������
	void Q();		//��������� ��������
	void A();		//��������
	void W();		//while
	void P();		//������������
	void R();		//return
	void B();		//break
	void V(NData* res);		//���������
	void Z(NData* res);		//���������
	void M(NData* res);		//���������
	void Y(NData* res);		//�����
	void L(NData* res);		//���������
	void N(NData* res);		//�� ������
	void K(NData* res);		//����� �������

public:
	TDiagram(TScanner* s) { scan = s; root = new Tree(scan); }
	~TDiagram() {}

	void PrintTree();

	void S();		//���������
};

