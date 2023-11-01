#pragma once
#include "defs.h"
#include "Scanner.h"

enum OBJ_TYPE {Empty = 0,
				ObjVar,		//������� ����������
				ObjFunct};	//�������

enum DATA_TYPE {NO_TYPE = 0, TYPE_INT, TYPE_SHORT, TYPE_FLOAT};

struct Node
{
	OBJ_TYPE objType;	//��� �������
	LEX id;				//������������� �������
	DATA_TYPE dataType;	//��� ��������
};

class Tree			//������� ������������� �������
{
private:
	Node* node;		//���������� �� ������� �������
	Tree* parent, * left, * right;		//��������, ����� � ������ �������

public:
	static TScanner* scan;
	static Tree* cur;		//������� ������� ������

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree(TScanner* scan);

	//������� ��������� ��������� ������
	void SetLeft(Node* data);
	void SetRight(Node* data);

	Tree* FindUp(Tree* from, LEX id);
	Tree* FindUp(LEX id);
	Tree* FindUpOneLevel(Tree* from, LEX id);

	void Print();

	//������������� ������������
	void SetCur(Tree* a);			//���������� ������� ���� ������
	Tree* GetCur();					//�������� ������ �� ������� ���� ������
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//��������� �������������� a � ������� � ����� t
	Tree* SemNewLevel();			//����� ������� (��� ���������� ���������)
	Tree* SemGetVar(LEX a);		//����� � ������� ���������� � ������ a � ������� ������ �� �������������� ������� ������
	Tree* SemGetFunct(LEX a);		//����� � ������� ���������� � ������ a � ������� ������ �� �������������� ������� ������
	int DupControl(Tree* addr, LEX a);			//�������� �������������� a �� ��������� �������� ������ �����

	DATA_TYPE GetType(int lexType);
};

