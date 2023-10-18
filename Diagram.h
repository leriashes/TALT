#pragma once
#include "Scanner.h"

class TDiagram
{
private:
	TScanner* sc;

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
	TDiagram(TScanner* scan) { sc = scan; }
	~TDiagram() {}

	void S();		//���������
};

