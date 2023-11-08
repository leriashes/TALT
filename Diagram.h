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
	void V(DATA_TYPE* resType);		//���������
	void Z(DATA_TYPE* resType);		//���������
	void M(DATA_TYPE* resType);		//���������
	void Y(DATA_TYPE* resType);		//�����
	void L(DATA_TYPE* resType);		//���������
	void N(DATA_TYPE* resType);		//�� ������
	void K(DATA_TYPE* resType);		//����� �������

public:
	TDiagram(TScanner* s) { scan = s; root = new Tree(scan); }
	~TDiagram() {}

	void PrintTree();

	void S();		//���������
};

