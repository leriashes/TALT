#pragma once
#include "Scanner.h"
#include "Semant.h"

class TDiagram
{
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
	void V();		//���������
	void Z();		//���������
	void M();		//���������
	void Y();		//�����
	void L();		//���������
	void N();		//�� ������
	void K();		//����� �������

public:
	TDiagram(TScanner* s) { scan = s; root = new Tree(scan); }
	~TDiagram() {}

	void S();		//���������
};

