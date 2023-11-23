#pragma once
#include "defs.h"
#include "Scanner.h"

enum OBJ_TYPE {Empty = 0,
				ObjVar,		//������� ����������
				ObjFunct};	//�������

enum DATA_TYPE {NO_TYPE = 0, TYPE_INT, TYPE_SHORT, TYPE_FLOAT};

union DATA_VALUE {
	short DataAsShort;
	int DataAsInt;
	float DataAsFloat;
};

struct NData {
	DATA_TYPE type;	//��� ��������
	DATA_VALUE value;	//��������
};

struct Node
{
	OBJ_TYPE objType;	//��� �������
	LEX id;				//������������� �������
	NData data;			//��� �������� � ���� ��������
};

class Tree			//������� ������������� �������
{
	LEX DT_Name[4] = { "�����.", "int", "short", "float" };

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

	Tree* FindRoot();
	Tree* FindUp(Tree* from, LEX id);
	Tree* FindUp(LEX id);
	Tree* FindUpOneLevel(Tree* from, LEX id);

	Tree* GetCurrentFunct();

	void Print();

	//������������� ������������
	void SetCur(Tree* a);			//���������� ������� ���� ������
	Tree* GetCur();					//�������� ������ �� ������� ���� ������
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//��������� �������������� a � ������� � ����� t
	Tree* SemNewLevel();			//����� ������� (��� ���������� ���������)
	Tree* SemGetVar(LEX a);		//����� � ������� ���������� � ������ a � ������� ������ �� �������������� ������� ������
	Tree* SemGetFunct(LEX a);		//����� � ������� ������� � ������ a � ������� ������ �� �������������� ������� ������
	int DupControl(Tree* addr, LEX a);			//�������� �������������� a �� ��������� �������� ������ �����

	DATA_TYPE TypeCasting(DATA_TYPE firstType, DATA_TYPE secondType, LEX operation);
	void TypeCastingAssign(DATA_TYPE firstType, DATA_TYPE secondType);
	void CheckTypeInt(DATA_TYPE type);

	DATA_TYPE GetType();
	DATA_TYPE GetTypebyLex(int lexType);

	DATA_VALUE* GetValue();
};

