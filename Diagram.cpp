#include "Diagram.h"

int TDiagram::LookForward(int k) 
{
	LEX lex;
	int  savedPointer = scan->GetUK();
	int  savedLine = scan->GetLine();
	int  savedPos = scan->GetPos();

	int  nextType;

	for (int i = 0; i < k; i++)
	{
		nextType = scan->Scanner(lex);
	}

	scan->SetLine(savedLine);
	scan->SetPos(savedPos);
	scan->PutUK(savedPointer);

	return nextType;
}



void TDiagram::PrintTree()
{
	root->Print();
}

void TDiagram::S()			//Программа
//               -----
//            ---| D |---
//		     |	 -----   |
//        ---|           |---
//        |  |   -----   |  |
//        |   ---| F |---   |
//       \|/     -----      |
//--------.----------------------->
//
{
	LEX lex;
	int type;

	type = LookForward(1);

	while (type == TShort || type == TLong || type == TInt || type == TFloat)
	{
		int k = 2;

		if (type == TShort || type == TLong)
		{
			type = LookForward(2);

			if (type == TInt)
			{
				k = 3;
			}
		}

		type = LookForward(k);

		if (type == TMain)
		{
			F();
		}
		else if (type == TIdent)
		{
			type = LookForward(k + 1);

			if (type == TLS)
			{
				F();
			}
			else 
			{
				D();
			}
		}
		else
		{
			for (int i = 0; i < k; i++)
			{
				type = scan->Scanner(lex);
			}
			scan->PrintError("Ожидался идентификатор");
		}

		type = LookForward(1);
	}
}



void TDiagram::D()			//Описание данных
// 
//  --- long ---                  ----------------------- , ----
//  |-----------|--- int ---      |                 -----      |
//	|-- short --           |      |         -- = ---| V |---   |
//  |                      |     \|/        |       -----  |   |
//------ float -------------------.--- a ------------------------ ; --->
//
{
	LEX lex;
	int type;

	type = scan->Scanner(lex);

	if (type != TLong && type != TShort && type != TInt && type != TFloat)
	{
		scan->PrintError("Ожидался тип");
	}

	DATA_TYPE semType = root->GetTypebyLex(type);

	if (type == TLong || type == TShort)
	{
		type = LookForward(1);

		if (type == TInt)
		{
			type = scan->Scanner(lex);
		}
	}

	do
	{
		type = scan->Scanner(lex);

		if (type != TIdent)
		{
			scan->PrintError("Ожидался идентификатор");
		}

		Tree* v = root->SemInclude(lex, ObjVar, semType);

		type = scan->Scanner(lex);

		if (type == TSave)
		{
			NData data;
			V(&data);

			root->TypeCastingAssign(semType, data.type);

			type = scan->Scanner(lex);

			if (v->GetType() == TYPE_SHORT)
			{
				v->GetValue()->DataAsShort = data.value.DataAsShort;
			}
			else if (v->GetType() == TYPE_INT)
			{
				v->GetValue()->DataAsInt = data.value.DataAsInt;
			}
			else
			{
				v->GetValue()->DataAsFloat = data.value.DataAsFloat;
			}
		}

	} while (type == TComma);

	if (type != TSemicolon)
	{
		scan->PrintError("Ожидался символ ';'");
	}
}



void TDiagram::F()			//Функция
// 
//  --- long ---             
//  |-----------|--- int ---  
//	|-- short --           |   
//  |                      | 	 ----- a -----                -----
// ------ float -----------------|			 |--- ( --- ) ----| Q |---->
//								 ---- main ---				  -----
//
{
	LEX lex;
	int type;

	type = scan->Scanner(lex);

	if (type != TLong && type != TShort && type != TInt && type != TFloat)
	{
		scan->PrintError("Ожидался тип");
	}

	DATA_TYPE semType = root->GetTypebyLex(type);

	if (type == TLong || type == TShort)
	{
		type = LookForward(1);

		if (type == TInt)
		{
			type = scan->Scanner(lex);
		}
	}

	type = scan->Scanner(lex);

	if (type != TMain && type != TIdent)
	{
		scan->PrintError("Ожидалось имя функции");
	}

	Tree* v = root->SemInclude(lex, ObjFunct, semType);

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ '('");
	}

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ ')'");
	}

	Q();

	root->SetCur(v);
}



void TDiagram::Q()			//Составной оператор
//               -----
//            ---| D |---
//		     |	 -----   |
//        ---|           |---
//        |  |   -----   |  |
//        |   ---| A |---   |
//       \|/     -----      |
//-- { ---.------------------- } ---->
//
{
	LEX lex;
	int type;

	type = scan->Scanner(lex);

	if (type != TFLS)
	{
		scan->PrintError("Ожидался символ '{'");
	}

	Tree* v = root->SemNewLevel();

	type = LookForward(1);

	while (type == TShort || type == TLong || type == TInt || type == TFloat || type == TFLS || type == TWhile || type == TIdent || type == TReturn || type == TBreak || type == TSemicolon || type == TMain)
	{

		if (type == TShort || type == TLong || type == TInt || type == TFloat)
		{
			D();
		}
		else
		{
			A();
		}

		type = LookForward(1);
	}

	type = scan->Scanner(lex);

	if (type != TFRS)
	{
		scan->PrintError("Ожидался символ '}'");
	}

	root->SetCur(v);
}



void TDiagram::A()			//Оператор
//               -----
//        -------| Q |------------
//		  |	     -----           |
//        |      -----           |
//        |------| W |-----------|
//        |      -----           |
//        |      -----           |
//        |   ---| P |---        |
//		  |  |	 -----   |       |
//-----------|-----------|-- ; -------->
//           |   -----   |  
//           |---| K |---|  
//           |   -----   |  
//           |   -----   |
//           |---| R |---|
//		     |	 -----   |
//           |   -----   |   
//            ---| B |---    
//               -----       
//
{
	LEX lex;
	int type;

	NData res;

	type = LookForward(1);

	if (type == TFLS)
	{
		Q();
	}
	else if (type == TWhile)
	{
		W();
	}
	else
	{
		if (type == TReturn)
		{
			R();
		}
		else if (type == TBreak)
		{
			B();
		}
		else if (type == TMain)
		{
			K(&res);
		}
		else if (type == TIdent)
		{
			type = LookForward(2);

			if (type == TSave)
			{
				P();
			}
			else
			{
				K(&res);
			}
		}

		type = scan->Scanner(lex);

		if (type != TSemicolon)
		{
			scan->PrintError("Ожидался символ ';'");
		}
	}
}



void TDiagram::W()			//while
// 
//		               -----         -----
// ----- while -- ( ---| V |--- ) ---| A |--->
//					   -----         -----
//
{
	LEX lex;
	int type;

	type = scan->Scanner(lex);

	if (type != TWhile)
	{
		scan->PrintError("Ожидался оператор 'while'");
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ '('");
	}

	NData data;
	V(&data);

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ ')'");
	}

	A();
}



void TDiagram::P()			//Присваивание
// 
//		             ----- 
// ----- a --- = ----| V |----->
//					 ----- 
//
{
	LEX lex;
	int type;

	type = scan->Scanner(lex);

	if (type != TIdent)
	{
		scan->PrintError("Ожидался идентификатор");
	}

	Tree* ident = root->SemGetVar(lex);

	type = scan->Scanner(lex);

	if (type != TSave)
	{
		scan->PrintError("Ожидался знак =");
	}

	NData data;
	V(&data);

	root->TypeCastingAssign(ident->GetType(), data.type);
}



void TDiagram::R()			//return
// 
//		            ----- 
// ----- return ----| V |----->
//					----- 
//
{
	LEX lex;
	int type;

	type = scan->Scanner(lex);

	if (type != TReturn)
	{
		scan->PrintError("Ожидался оператор 'return'");
	}

	NData data;
	V(&data);

	root->TypeCastingAssign(root->GetCur()->GetCurrentFunct()->GetType(), data.type);
}



void TDiagram::B()			//break
// 
//------ break -------->
//
{
	LEX lex;
	int type;

	type = scan->Scanner(lex);

	if (type != TBreak)
	{
		scan->PrintError("Ожидался оператор 'break'");
	}
}



void TDiagram::V(NData* res)			//Выражение
//               
//                -----   ----- == ----
//             ---| Z |---|           |---
//             |  -----   ----- != ----  |
//   -----    \|/                        |
//---| Z |-----.------------------------------->
//   -----            
//
{
	LEX lex;
	int type;

	NData secondData;

	Z(res);

	type = LookForward(1);

	while (type == TEq || type == TNEq)
	{
		int znak = type - 51;

		type = scan->Scanner(lex);
		Z(&secondData);
		type = LookForward(1);

		root->TypeCasting(res->type, secondData.type, OP_Name[znak]);
		res->type = TYPE_INT;
	}
}



void TDiagram::Z(NData* res)			//Сравнение
//    
//					      ----- < -----   
//					      |           |
//					      ----- > -----   
//			              |           |
//                -----   ----- <= ----
//             ---| Y |---|           |---
//             |  -----   ----- >= ----  |
//   -----    \|/                        |
//---| Y |-----.------------------------------->
//   -----            
//
{
	LEX lex;
	int type;

	NData secondData;

	Y(res);

	type = LookForward(1);

	while (type == TLT || type == TGT || type == TLE || type == TGE)
	{
		int znak = type - 51;

		type = scan->Scanner(lex);
		Y(&secondData);
		type = LookForward(1);

		root->TypeCasting(res->type, secondData.type, OP_Name[znak]);
		res->type = TYPE_INT;
	}
}



void TDiagram::M(NData* res)			//Множитель
// 
//					      ----- * -----   
//			              |           |
//                -----   ----- / -----
//             ---| N |---|           |---
//             |  -----   ----- % -----  |
//   -----    \|/                        |
//---| N |-----.------------------------------->
//   -----            
//
{
	LEX lex;
	int type;

	NData secondData;

	N(res);

	type = LookForward(1);

	if (type == TMod)
	{
		root->CheckTypeInt(res->type);
	}

	while (type == TMult || type == TDiv || type == TMod)
	{
		int znak = type - 51;

		type = scan->Scanner(lex);
		N(&secondData);

		if (type == TMod)
		{
			root->CheckTypeInt(secondData.type);
		}

		type = LookForward(1);

		res->type = root->TypeCasting(res->type, secondData.type, OP_Name[znak]);
	}
}



void TDiagram::Y(NData* res)			//Сдвиг
//               
//                -----   ----- << ----
//             ---| L |---|           |---
//             |  -----   ----- >> ----  |
//   -----    \|/                        |
//---| L |-----.------------------------------->
//   -----            
//
{
	LEX lex;
	int type;

	NData secondData;

	L(res);

	type = LookForward(1);

	if (type == TLShift || type == TRShift)
	{
		root->CheckTypeInt(res->type);
	}

	while (type == TLShift || type == TRShift)
	{

		type = scan->Scanner(lex);
		L(&secondData);

		root->CheckTypeInt(secondData.type);

		type = LookForward(1);

		res->type = TYPE_INT;
	}
}



void TDiagram::L(NData* res)			//Слагаемое
//               
//                -----   ----- + ----
//             ---| M |---|           |---
//             |  -----   ----- - ----   |
//   -----    \|/                        |
//---| M |-----.------------------------------->
//   -----            
//
{
	LEX lex;
	int type;

	NData secondData;

	M(res);

	type = LookForward(1);

	while (type == TPlus || type == TMinus)
	{
		int znak = type - 51;

		type = scan->Scanner(lex);
		M(&secondData);
		type = LookForward(1);

		res->type = root->TypeCasting(res->type, secondData.type, OP_Name[znak]);
	}
}



void TDiagram::N(NData* res)			//Со знаком
//					 --- a -----------------
//                   |                     |
//		--- + ---    |-- c1 ---------------|            
// -----|-------|----|-- c2 ---------------|----->
//		--- - ---	 |   -----             |
//					 |---| K |-------------|
//					 |   -----             |
//					 |		  -----	       |
//					 --- ( ---| V |--- ) ---
//							  -----			 
//
{
	LEX lex;
	int type;

	type = LookForward(1);

	if (type == TPlus || type == TMinus)
	{
		type = scan->Scanner(lex);
	}

	type = LookForward(1);

	if (type == TLS)
	{
		type = scan->Scanner(lex);

		V(res);

		type = scan->Scanner(lex);

		if (type != TRS)
		{
			scan->PrintError("Ожидался символ ')'");
		}
	}
	else if (type == TMain)
	{
		K(res);
	}
	else if (type == TIdent)
	{
		type = LookForward(2);

		if (type == TLS)
		{
			K(res);
		}
		else
		{
			type = scan->Scanner(lex);
			res->type = root->SemGetVar(lex)->GetType();
			res->value = *root->SemGetVar(lex)->GetValue();
		}

	}
	else
	{
		type = scan->Scanner(lex);

		if (type == TConstInt)
		{
			res->type = TYPE_INT;
			res->value.DataAsInt = atoi(lex);
		}
		else if (type == TConstFloat)
		{
			res->type = TYPE_FLOAT;
			res->value.DataAsInt = atof(lex);
		}
		else
		{
			scan->PrintError("Ожидалось выражение");
		}
	}
}



void TDiagram::K(NData* res)			//Вызов функции
// 
//		----- a -----                
// -----|			|--- ( --- ) ----->
//		---- main ---				 
//
{
	LEX lex;
	int type;

	type = scan->Scanner(lex);

	if (type != TMain && type != TIdent)
	{
		scan->PrintError("Ожидалось имя функции");
	}

	Tree* funct = root->SemGetFunct(lex);
	res->type = funct->GetType();
	
	if (res->type == TYPE_SHORT)
	{
		res->value.DataAsShort = funct->GetValue()->DataAsShort;
	}
	else if (res->type == TYPE_INT)
	{
		res->value.DataAsInt = funct->GetValue()->DataAsInt;
	}
	else
	{
		res->value.DataAsFloat = funct->GetValue()->DataAsFloat;
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ '('");
	}

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ ')'");
	}
}
