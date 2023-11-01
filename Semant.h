#pragma once
#include "defs.h"

enum OBJ_TYPE {ObjVar = 1,		//простая переменная
				ObjFunct};		//функция

enum DATA_TYPE {TYPE_INT = 1, TYPE_SHORT, TYPE_FLOAT};

struct Node
{
	OBJ_TYPE objType;	//тип объекта
	LEX id;				//идентификатор объекта
	DATA_TYPE dataType;	//тип значения
};

class Tree			//элемент семантической таблицы
{
	Node* node;		//информация об объекте таблицы
	Tree* parent, * left, * right;		//родитель, левый и правый потомки

public:
	static Tree* cur;		//текущий элемент дерева

	Tree(Tree* p, Tree* l, Tree* r, Node* data);
	Tree();
	~Tree();

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
	Tree* SemInclude(LEX a, DATA_TYPE t);		//занесение идентификатора a в таблицу с типом t
	Tree* SemGetObj(LEX a);		//найти в таблице объект с именем a и вернуть ссылку на соответсвующий элемент дерева
	int DupControl(Tree* addr, LEX a);			//проверка идентификатора a на повторное описание внутри блока
};

