#include "Semant.h"
Tree* Tree::cur = (Tree*)NULL;
TScanner* Tree::scan = (TScanner*)NULL;

Tree::Tree(TScanner* scan)
{
	this->scan = scan;

	node = new Node();
	node->objType = Empty;
	node->dataType = NO_TYPE;

	parent = NULL;
	left = NULL;
	right = NULL;


	cur = this;
}

Tree::Tree(Tree* p, Tree* l, Tree* r, Node* data)
{
	node = new Node();

	parent = p;
	left = l;
	right = r;

	memcpy(node, data, sizeof(Node));
}

void Tree::SetLeft(Node* data)
{
	left = new Tree(this, NULL, NULL, data);
}

void Tree::SetRight(Node* data)
{
	right = new Tree(this, NULL, NULL, data);
}

Tree* Tree::FindUp(Tree* from, LEX id)
{
	Tree* i = from;

	while ((i != NULL) && memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) != 0)
		i = i->parent;

	return i;
}

Tree* Tree::FindRoot()
{
	Tree* i = cur;

	while ((i != NULL) && (i->parent != NULL))
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

	while ((i != NULL) && (i->parent == NULL || i->parent->right != i))
	{
		if (memcmp(id, i->node->id, max(strlen(i->node->id), strlen(id))) == 0)
			return i;	//найден совпадающий идентификатор

		i = i->parent;
	}

	return NULL;
}

void Tree::Print()
{
	if (node->objType != Empty)
		printf("Вершина с данными %s ------>", node->id);
	else
		printf("Вершина ПУСТАЯ ------>");

	if (left != NULL)
	{
		if (left->node->objType != Empty)
			printf("      слева данные %s", left->node->id);
		else
			printf("      слева ПУСТАЯ вершина");
	}
		

	if (right != NULL)
	{
		if (right->node->objType != Empty)
			printf("      справа данные %s", right->node->id);
		else
			printf("      справа ПУСТАЯ вершина");
	}

	printf("\n");

	if (left != NULL)
		left->Print();

	if (right != NULL)
	{
		printf("\n\nРОДИТЕЛЬ: ");
		if (node->objType != Empty)
			printf("Вершина %s\n", node->id);
		else
			printf("Вершина ПУСТАЯ\n");
		right->Print();
	}
		
}

void Tree::SetCur(Tree* a)
{
	cur = a;
}

Tree* Tree::GetCurrentFunct()
{
	if (parent == NULL)
		return NULL;
		
	if (node->objType == Empty && parent->node->objType == ObjFunct && parent->right == this)
		return parent;

	return parent->GetCurrentFunct();
}

Tree* Tree::GetCur()
{
	return cur;
}

Tree* Tree::SemInclude(LEX a, OBJ_TYPE ot, DATA_TYPE t)
{
	if (DupControl(cur, a))
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Повторное описание идентификатора", a);
	}

	Tree* v;
	Node n;

	memcpy(n.id, a, strlen(a) + 1);
	n.objType = ot;
	n.dataType = t;
	
	if (this->node->objType == Empty && this->parent == NULL && this->left == NULL && this->right == NULL)
		memcpy(node, &n, sizeof(Node));
	else
	{
		cur->SetLeft(&n);
		cur = cur->left;
	}

	if (ot == ObjFunct)
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
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Отсутствует описание идентификатора", a);
	}

	if (v->node->objType == ObjFunct)
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Неверное использование имени функции", a);
	}

	return v;
}

Tree* Tree::SemNewLevel()
{
	Node n;

	memcpy(&n.id, &"", 2);
	n.objType = Empty;

	cur->SetLeft(&n);
	cur = cur->left;
	Tree* v = cur;

	cur->SetRight(&n);
	cur = cur->right;

	return v;
}

Tree* Tree::SemGetFunct(LEX a)
{
	Tree* v = FindUp(cur, a);

	if (v == NULL)
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Отсутствует описание функции", a);
	}

	if (v->node->objType != ObjFunct)
	{
		printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
		Tree* root = FindRoot();
		root->Print();
		printf("\n\n");
		scan->PrintError("Идентификатор не является именем функции", a);
	}

	return v;
}

int Tree::DupControl(Tree* addr, LEX a)
{
	if (FindUpOneLevel(addr, a) == NULL) return 0;
	return 1;
}

DATA_TYPE Tree::GetType()
{
	return node->dataType;
}

DATA_TYPE Tree::TypeCasting(DATA_TYPE firstType, DATA_TYPE secondType, LEX operation)
{
	DATA_TYPE resType = firstType;

	if (firstType != secondType)
	{
		if (firstType == NO_TYPE || secondType == NO_TYPE)
			resType = NO_TYPE;
		else if (firstType == TYPE_FLOAT || secondType == TYPE_FLOAT)
			resType = TYPE_FLOAT;
		else if (firstType == TYPE_INT || secondType == TYPE_INT)
			resType = TYPE_INT;
		else
			resType = TYPE_SHORT;

	}


	printf("\nПриведение типов %s и %s (%s) --> %s ------ строка %d\n", DT_Name[firstType], DT_Name[secondType], operation, DT_Name[resType], scan->GetLine());

	return resType;
}

void Tree::TypeCastingAssign(DATA_TYPE firstType, DATA_TYPE secondType)
{
	printf("\nПриведение типа %s к типу %s (присваивание) --> %s ------ строка %d\n", DT_Name[secondType], DT_Name[firstType], DT_Name[firstType], scan->GetLine());
}

DATA_TYPE Tree::GetTypebyLex(int lexType)
{
	if (lexType == TShort)
		return TYPE_SHORT;

	if (lexType == TFloat)
		return TYPE_FLOAT;

	return TYPE_INT;
}


