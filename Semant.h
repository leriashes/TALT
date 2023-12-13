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

struct FStart {
	int uk;
	int line;
	int pos;
};

struct Node
{
	OBJ_TYPE objType;	//��� �������
	LEX id;				//������������� �������
	NData data;			//��� �������� � ���� ��������
	FStart funcStart; //������ �������
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

	bool flagInterpret = true;
	bool flagReturn = false;

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
	void CleanTree();
	void CleanChild();

	//������������� ������������
	void SetCur(Tree* a);			//���������� ������� ���� ������
	Tree* GetCur();					//�������� ������ �� ������� ���� ������
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//��������� �������������� a � ������� � ����� t
	Tree* SemInclude(Tree* first);
	Tree* SemNewLevel();			//����� ������� (��� ���������� ���������)
	Tree* SemGetVar(LEX a);		//����� � ������� ���������� � ������ a � ������� ������ �� �������������� ������� ������
	Tree* SemGetFunct(LEX a);		//����� � ������� ������� � ������ a � ������� ������ �� �������������� ������� ������
	int DupControl(Tree* addr, LEX a);			//�������� �������������� a �� ��������� �������� ������ �����

	void TypeCasting(NData* firstData, NData secondData, int operation, LEX operationName);
	NData TypeCastingAssign(DATA_TYPE firstType, NData secondData);
	void CheckTypeInt(DATA_TYPE type);

	DATA_TYPE GetType();
	DATA_TYPE GetTypebyLex(int lexType);

	DATA_VALUE* GetValue();

	void SetStart(int uk, int line, int pos);
	FStart GetStart();
};

