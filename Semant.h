#pragma once
#include "defs.h"
#include "Scanner.h"

enum OBJ_TYPE {Empty = 0,
				ObjVar,		//простая переменная
				ObjFunct};	//функция

enum DATA_TYPE {NO_TYPE = 0, TYPE_INT, TYPE_SHORT, TYPE_FLOAT};

union DATA_VALUE {
	short DataAsShort;
	int DataAsInt;
	float DataAsFloat;
};

struct NData {
	DATA_TYPE type;	//тип значения
	DATA_VALUE value;	//значение
};

struct Node
{
	OBJ_TYPE objType;	//тип объекта
	LEX id;				//идентификатор объекта
	NData data;			//тип значения и само значение
};

class Tree			//элемент семантической таблицы
{
	LEX DT_Name[4] = { "неопр.", "int", "short", "float" };

private:
	Node* node;		//информация об объекте таблицы
	Tree* parent, * left, * right;		//родитель, левый и правый потомки

public:
	static TScanner* scan;
	static Tree* cur;		//текущий элемент дерева

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree(TScanner* scan);

	//Функции обработки бинарного дерева
	void SetLeft(Node* data);
	void SetRight(Node* data);

	Tree* FindRoot();
	Tree* FindUp(Tree* from, LEX id);
	Tree* FindUp(LEX id);
	Tree* FindUpOneLevel(Tree* from, LEX id);

	Tree* GetCurrentFunct();

	void Print();

	//Семантичексие подпрограммы
	void SetCur(Tree* a);			//установить текущий узел дерева
	Tree* GetCur();					//получить ссылку на текущий узел дерева
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//занесение идентификатора a в таблицу с типом t
	Tree* SemNewLevel();			//новый уровень (для составного оператора)
	Tree* SemGetVar(LEX a);		//найти в таблице переменную с именем a и вернуть ссылку на соответсвующий элемент дерева
	Tree* SemGetFunct(LEX a);		//найти в таблице функцию с именем a и вернуть ссылку на соответсвующий элемент дерева
	int DupControl(Tree* addr, LEX a);			//проверка идентификатора a на повторное описание внутри блока

	DATA_TYPE TypeCasting(DATA_TYPE firstType, DATA_TYPE secondType, LEX operation);
	void TypeCastingAssign(DATA_TYPE firstType, DATA_TYPE secondType);
	void CheckTypeInt(DATA_TYPE type);

	DATA_TYPE GetType();
	DATA_TYPE GetTypebyLex(int lexType);

	DATA_VALUE* GetValue();
};

