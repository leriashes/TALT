#pragma once
#include "defs.h"

enum OBJ_TYPE {ObjVar = 1,		//������� ����������
				ObjFunct};		//�������

enum DATA_TYPE {TYPE_INT = 1, TYPE_SHORT, TYPE_FLOAT};

struct Node
{
	OBJ_TYPE objType;	//��� �������
	LEX id;				//������������� �������
	DATA_TYPE dataType;	//��� ��������
};

class Tree			//������� ������������� �������
{
	Node* node;		//���������� �� ������� �������
	Tree* parent, * left, * right;		//��������, ����� � ������ �������

public:
	static Tree* cur;		//������� ������� ������

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree();
	~Tree();

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
	Tree* SemInclude(LEX a, DATA_TYPE t);		//��������� �������������� a � ������� � ����� t
	Tree* SemGetObj(LEX a);		//����� � ������� ������ � ������ a � ������� ������ �� �������������� ������� ������
	int DupControl(Tree* addr, LEX a);			//�������� �������������� a �� ��������� �������� ������ �����
};

