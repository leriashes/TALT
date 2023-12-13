#include "Semant.h"
Tree* Tree::cur = (Tree*)NULL;
TScanner* Tree::scan = (TScanner*)NULL;

Tree::Tree(TScanner* scan)
{
	this->scan = scan;

	node = new Node();
	node->objType = Empty;
	node->data.type = NO_TYPE;

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
		printf("Вершина с данными %s (%s) ------>", node->id, DT_Name[node->data.type]);
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
	if (!flagInterpret) return NULL;

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
	n.data.type = t;
	
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
	if (!flagInterpret) return NULL;

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
	if (!flagInterpret) return NULL;

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
	if (!flagInterpret) return NULL;

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
	return node->data.type;
}

void Tree::TypeCasting(NData* firstData, NData secondData, int operation, LEX operationName)
{
	if (!flagInterpret) return;

	DATA_TYPE resType = firstData->type;

	if (resType != secondData.type)
	{
		if (resType == NO_TYPE || secondData.type == NO_TYPE)
			resType = NO_TYPE;
		else if (resType == TYPE_FLOAT || secondData.type == TYPE_FLOAT)
			resType = TYPE_FLOAT;
		else if (resType == TYPE_INT || secondData.type == TYPE_INT)
			resType = TYPE_INT;
		else
			resType = TYPE_SHORT;
	}

	printf("\nПриведение типов %s и %s (%s) --> %s ------ строка %d\n", DT_Name[firstData->type], DT_Name[secondData.type], operationName, DT_Name[resType], scan->GetLine());

	if (firstData->type != resType)
	{
		if (resType == TYPE_FLOAT)
		{
			if (firstData->type == TYPE_INT)
			{
				firstData->value.DataAsFloat = firstData->value.DataAsInt;
			}
			else
			{
				firstData->value.DataAsFloat = firstData->value.DataAsShort;
			}
		}
		else if (resType == TYPE_INT)
		{
			if (firstData->type == TYPE_FLOAT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat;
			}
			else
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort;
			}
		}
		else
		{
			if (firstData->type == TYPE_INT)
			{
				firstData->value.DataAsShort = firstData->value.DataAsInt;
			}
			else
			{
				firstData->value.DataAsShort = firstData->value.DataAsFloat;
			}
		}
	}

	firstData->type = resType;

	if (resType == TYPE_FLOAT)
	{
		if (secondData.type == TYPE_FLOAT)
		{
			//L - слагаемое
			if (operation == TPlus)
				firstData->value.DataAsFloat += secondData.value.DataAsFloat;
			else if (operation == TMinus)
				firstData->value.DataAsFloat -= secondData.value.DataAsFloat;

			//M - множитель
			else if (operation == TMult)
				firstData->value.DataAsFloat *= secondData.value.DataAsFloat;
			else if (operation == TDiv)
				firstData->value.DataAsFloat /= secondData.value.DataAsFloat;

			//Z - сравнение
			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat < secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat > secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat <= secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat >= secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}

			//V - выражение
			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat == secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat != secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
		}
		else if (secondData.type == TYPE_INT)
		{
			if (operation == TPlus)
				firstData->value.DataAsFloat += secondData.value.DataAsInt;
			else if (operation == TMinus)
				firstData->value.DataAsFloat -= secondData.value.DataAsInt;
			else if (operation == TMult)
				firstData->value.DataAsFloat *= secondData.value.DataAsInt;
			else if (operation == TDiv)
				firstData->value.DataAsFloat /= secondData.value.DataAsInt;

			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat < secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat > secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat <= secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat >= secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}

			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat == secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat != secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
		}
		else
		{
			if (operation == TPlus)
				firstData->value.DataAsFloat += secondData.value.DataAsShort;
			else if (operation == TMinus)
				firstData->value.DataAsFloat -= secondData.value.DataAsShort;
			else if (operation == TMult)
				firstData->value.DataAsFloat *= secondData.value.DataAsShort;
			else if (operation == TDiv)
				firstData->value.DataAsFloat /= secondData.value.DataAsShort;

			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat < secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat > secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat <= secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat >= secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}

			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat == secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsFloat != secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
		}
	}
	else if (resType == TYPE_INT)
	{
		if (secondData.type == TYPE_FLOAT)
		{
			if (operation == TPlus)
				firstData->value.DataAsInt += secondData.value.DataAsFloat;
			else if (operation == TMinus)
				firstData->value.DataAsInt -= secondData.value.DataAsFloat;
			else if (operation == TMult)
				firstData->value.DataAsInt *= secondData.value.DataAsFloat;
			else if (operation == TDiv)
				firstData->value.DataAsInt /= secondData.value.DataAsFloat;

			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt < secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt > secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt <= secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt >= secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}

			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt == secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt != secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
		}
		else if (secondData.type == TYPE_INT)
		{
			if (operation == TPlus)
				firstData->value.DataAsInt += secondData.value.DataAsInt;
			else if (operation == TMinus)
				firstData->value.DataAsInt -= secondData.value.DataAsInt;
			else if (operation == TMult)
				firstData->value.DataAsInt *= secondData.value.DataAsInt;
			else if (operation == TDiv)
				firstData->value.DataAsInt /= secondData.value.DataAsInt;
			else if (operation == TMod)
				firstData->value.DataAsInt %= secondData.value.DataAsInt;

			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt < secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt > secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt <= secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt >= secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}

			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt == secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt != secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
		}
		else
		{
			if (operation == TPlus)
				firstData->value.DataAsInt += secondData.value.DataAsShort;
			else if (operation == TMinus)
				firstData->value.DataAsInt -= secondData.value.DataAsShort;
			else if (operation == TMult)
				firstData->value.DataAsInt *= secondData.value.DataAsShort;
			else if (operation == TDiv)
				firstData->value.DataAsInt /= secondData.value.DataAsShort;
			else if (operation == TMod)
				firstData->value.DataAsInt %= secondData.value.DataAsShort;

			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt < secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt > secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt <= secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt >= secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}

			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt == secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsInt != secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
		}
	}
	else
	{
		if (secondData.type == TYPE_FLOAT)
		{
			if (operation == TPlus)
				firstData->value.DataAsShort += secondData.value.DataAsFloat;
			else if (operation == TMinus)
				firstData->value.DataAsShort -= secondData.value.DataAsFloat;
			else if (operation == TMult)
				firstData->value.DataAsShort *= secondData.value.DataAsFloat;
			else if (operation == TDiv)
				firstData->value.DataAsShort /= secondData.value.DataAsFloat;

			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort < secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort > secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort <= secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort >= secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}

			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort == secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort != secondData.value.DataAsFloat;
				firstData->type = TYPE_INT;
			}
		}
		else if (secondData.type == TYPE_INT)
		{
			if (operation == TPlus)
				firstData->value.DataAsShort += secondData.value.DataAsInt;
			else if (operation == TMinus)
				firstData->value.DataAsShort -= secondData.value.DataAsInt;
			else if (operation == TMult)
				firstData->value.DataAsShort *= secondData.value.DataAsInt;
			else if (operation == TDiv)
				firstData->value.DataAsShort /= secondData.value.DataAsInt;
			else if (operation == TMod)
				firstData->value.DataAsShort %= secondData.value.DataAsInt;

			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort < secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort > secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort <= secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort >= secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}

			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort == secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort != secondData.value.DataAsInt;
				firstData->type = TYPE_INT;
			}
		}
		else
		{
			if (operation == TPlus)
				firstData->value.DataAsShort += secondData.value.DataAsShort;
			else if (operation == TMinus)
				firstData->value.DataAsShort -= secondData.value.DataAsShort;
			else if (operation == TMult)
				firstData->value.DataAsShort *= secondData.value.DataAsShort;
			else if (operation == TDiv)
				firstData->value.DataAsShort /= secondData.value.DataAsShort;
			else if (operation == TMod)
				firstData->value.DataAsShort %= secondData.value.DataAsShort;

			else if (operation == TLT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort < secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGT)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort > secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TLE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort <= secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TGE)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort >= secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}

			else if (operation == TEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort == secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
			else if (operation == TNEq)
			{
				firstData->value.DataAsInt = firstData->value.DataAsShort != secondData.value.DataAsShort;
				firstData->type = TYPE_INT;
			}
		}
	}

}

NData Tree::TypeCastingAssign(DATA_TYPE firstType, NData secondData)
{
	printf("\nПриведение типа %s к типу %s (присваивание) --> %s ------ строка %d\n", DT_Name[secondData.type], DT_Name[firstType], DT_Name[firstType], scan->GetLine());
	if (firstType == TYPE_SHORT)
	{
		if (secondData.type != TYPE_SHORT)
		{
			if (secondData.type == TYPE_INT)
			{
				secondData.value.DataAsShort = secondData.value.DataAsInt;
			}
			else
			{
				secondData.value.DataAsShort = secondData.value.DataAsFloat;
			}

			secondData.type = TYPE_SHORT;
		}

		printf("\tТип: short\tЗначение: %d", secondData.value.DataAsShort);
	}
	else if (firstType == TYPE_INT)
	{
		if (secondData.type != TYPE_INT)
		{
			int res;

			if (secondData.type == TYPE_SHORT)
			{
				res = secondData.value.DataAsShort;
			}
			else
			{
				res = secondData.value.DataAsFloat;
			}

			secondData.type = TYPE_INT;
			secondData.value.DataAsInt = res;
		}

		printf("\tТип: int\tЗначение: %d", secondData.value.DataAsInt);
	}
	else
	{
		if (secondData.type != TYPE_FLOAT)
		{
			if (secondData.type == TYPE_SHORT)
			{
				secondData.value.DataAsFloat = secondData.value.DataAsShort;
			}
			else
			{
				secondData.value.DataAsFloat = secondData.value.DataAsInt;
			}

			secondData.type = TYPE_FLOAT;
		}

		printf("\tТип: float\tЗначение: %f", secondData.value.DataAsFloat);
	}

	return secondData;
}

void Tree::CleanChild()
{
	if (right != NULL)
	{
		right->CleanTree();
		delete right;
		right = NULL;
	}
}

void Tree::CleanTree()
{
	if (left != NULL)
	{
		left->CleanTree();
		delete left;
		left = NULL;
	}

	CleanChild();
}

DATA_TYPE Tree::GetTypebyLex(int lexType)
{
	if (lexType == TShort)
		return TYPE_SHORT;

	if (lexType == TFloat)
		return TYPE_FLOAT;

	return TYPE_INT;
}

void Tree::CheckTypeInt(DATA_TYPE type)
{
	if (type != TYPE_INT && type != TYPE_SHORT)
	{
		scan->PrintError("Выражение должно относится к целочисленному типу");
	}
}

DATA_VALUE* Tree::GetValue()
{
	return &(node->data.value);
}


