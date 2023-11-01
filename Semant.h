#pragma once
#include "defs.h"
#include "Scanner.h"

enum OBJ_TYPE {Empty = 1,
				ObjVar,		//простая переменная
				ObjFunct};	//функция

enum DATA_TYPE {TYPE_INT = 1, TYPE_SHORT, TYPE_FLOAT};

struct Node
{
	OBJ_TYPE objType;	//тип объекта
	LEX id;				//идентификатор объекта
	DATA_TYPE dataType;	//тип значения
};

class Tree			//элемент семантической таблицы
{
private:
	static TScanner* scan;

	Node* node;		//информация об объекте таблицы
	Tree* parent, * left, * right;		//родитель, левый и правый потомки

public:
	static Tree* cur;		//текущий элемент дерева

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree();
	Tree(TScanner* scan);

	//Функции обработки бинарного дерева
	void SetLeft(Node* data);
	void SetRight(Node* data);

	Tree* FindUp(Tree* from, LEX id);
	Tree* FindUp(LEX id);
	Tree* FindUpOneLevel(Tree* from, LEX id);

	void Print();

	//Семантичексие подпрограммы
	void SetCur(Tree* a);			//установить текущий узел дерева
	Tree* GetCur();					//получить ссылку на текущий узел дерева
	Tree* SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t);		//занесение идентификатора a в таблицу с типом t
	Tree* SemGetVar(LEX a);		//найти в таблице переменную с именем a и вернуть ссылку на соответсвующий элемент дерева
	Tree* SemGetFunct(LEX a);		//найти в таблице переменную с именем a и вернуть ссылку на соответсвующий элемент дерева
	int DupControl(Tree* addr, LEX a);			//проверка идентификатора a на повторное описание внутри блока
};

