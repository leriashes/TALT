#include "Semant.h"


Tree::Tree(TScanner* scan)
{
	cur = NULL;
	this->scan = scan;
}

Tree::Tree(Tree* p, Tree* l, Tree* r, Node* data)
{
	node = new Node();

	parent = p;
	left = l;
	right = r;

	memcpy(node, data, sizeof(Node));
}

Tree::Tree()
{
	node = new Node();

	parent = NULL;
	left = NULL;
	right = NULL;

	memcpy(node, &("------"), sizeof(Node));
}

void Tree::SetLeft(Node* data)
{
	Tree* a = new Tree(NULL, NULL, this, data);
	left = a;
}

void Tree::SetRight(Node* data)
{
	Tree* a = new Tree(NULL, NULL, this, data);
	right = a;
}

Tree* Tree::FindUp(Tree* from, LEX id)
{
	Tree* i = from;

	while ((i != NULL) && memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) != 0)
		i = i->parent;

	return i;
}

Tree* Tree::FindUp(LEX id)
{
	return FindUp(this, id);
}

Tree* Tree::FindUpOneLevel(Tree* from, LEX id)
{
	Tree* i = from;		//текущая вершина поиска

	while ((i != NULL) && (i->parent->right != i))
	{
		if (memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) == 0)
			return i;	//найден совпадающий идентификатор

		i = i->parent;
	}

	return NULL;
}

void Tree::Print()
{
	printf("Вершина с данными %s ------>", node->id);

	if (left != NULL)
		printf("      слева данные %s", left->node->id);

	if (right != NULL)
		printf("      справа данные %s", right->node->id);
	printf("\n");

	if (left != NULL)
		left->Print();

	if (right != NULL)
		right->Print();
}

void Tree::SetCur(Tree* a)
{
	cur = a;
}

Tree* Tree::GetCur()
{
	return cur;
}

Tree* Tree::SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t)
{
	if (DupControl(cur, a))
		scan->PrintError("Повторное описание идентификатора", a);

	Tree* v;
	Node n;

	memcpy(n.id, a, strlen(a) + 1);
	n.objType = ot;
	n.dataType = t;
	
	cur->SetLeft(&n);
	cur = cur->left;

	if (t == ObjFunct)
	{
		v = cur;
		memcpy(&n.id, &"", 2);
		n.objType = Empty;

		cur->SetRight(&n);
		cur = cur->right;
		return v;
	}

	return cur;
}

Tree* Tree::SemGetVar(LEX a)
{
	Tree* v = FindUp(cur, a);

	if (v == NULL)
		scan->PrintError("Отсутствует описание идентификатора", a);

	if (v->node->objType == ObjFunct)
		scan->PrintError("Неверное использование имени функции", a);

	return v;
}

Tree* Tree::SemGetFunct(LEX a)
{
	Tree* v = FindUp(cur, a);

	if (v == NULL)
		scan->PrintError("Отсутствует описание функции", a);

	if (v->node->objType != ObjFunct)
		scan->PrintError("Идентификатор не является именем функции", a);

	return v;
}

int Tree::DupControl(Tree* addr, LEX a)
{
	if (FindUpOneLevel(addr, a) == NULL) return 0;
	return 1;
}


