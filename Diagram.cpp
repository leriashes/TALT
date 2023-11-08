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
			DATA_TYPE valType;
			V(&valType);

			root->TypeCastingAssign(semType, valType);

			type = scan->Scanner(lex);
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

	DATA_TYPE resType;

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
			K(&resType);
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
				K(&resType);
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

	DATA_TYPE valType;
	V(&valType);

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

	DATA_TYPE valType;
	V(&valType);

	root->TypeCastingAssign(ident->GetType(), valType);
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

	DATA_TYPE valType;
	V(&valType);

	root->TypeCastingAssign(root->GetCur()->GetCurrentFunct()->GetType(), valType);
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



void TDiagram::V(DATA_TYPE* resType)			//Выражение
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

	DATA_TYPE secondType;

	Z(resType);

	type = LookForward(1);

	while (type == TEq || type == TNEq)
	{
		int znak = type - 51;

		type = scan->Scanner(lex);
		Z(&secondType);
		type = LookForward(1);

		root->TypeCasting(*resType, secondType, OP_Name[znak]);
		*resType = TYPE_INT;
	}
}



void TDiagram::Z(DATA_TYPE* resType)			//Сравнение
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

	DATA_TYPE secondType;

	Y(resType);

	type = LookForward(1);

	while (type == TLT || type == TGT || type == TLE || type == TGE)
	{
		int znak = type - 51;

		type = scan->Scanner(lex);
		Y(&secondType);
		type = LookForward(1);

		root->TypeCasting(*resType, secondType, OP_Name[znak]);
		*resType = TYPE_INT;
	}
}



void TDiagram::M(DATA_TYPE* resType)			//Множитель
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

	DATA_TYPE secondType;

	N(resType);

	type = LookForward(1);

	while (type == TMult || type == TDiv || type == TMod)
	{
		int znak = type - 51;

		type = scan->Scanner(lex);
		N(&secondType);
		type = LookForward(1);

		if (type == TMod)
		{
			;
		}

		*resType = root->TypeCasting(*resType, secondType, OP_Name[znak]);
	}
}



void TDiagram::Y(DATA_TYPE* resType)			//Сдвиг
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

	DATA_TYPE secondType;

	L(resType);

	type = LookForward(1);

	while (type == TLShift || type == TRShift)
	{

		type = scan->Scanner(lex);
		L(&secondType);
		type = LookForward(1);

		*resType = TYPE_INT;
	}
}



void TDiagram::L(DATA_TYPE* resType)			//Слагаемое
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

	DATA_TYPE secondType;

	M(resType);

	type = LookForward(1);

	while (type == TPlus || type == TMinus)
	{
		int znak = type - 51;

		type = scan->Scanner(lex);
		M(&secondType);
		type = LookForward(1);

		*resType = root->TypeCasting(*resType, secondType, OP_Name[znak]);
	}
}



void TDiagram::N(DATA_TYPE* resType)			//Со знаком
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

		V(resType);

		type = scan->Scanner(lex);

		if (type != TRS)
		{
			scan->PrintError("Ожидался символ ')'");
		}
	}
	else if (type == TMain)
	{
		K(resType);
	}
	else if (type == TIdent)
	{
		type = LookForward(2);

		if (type == TLS)
		{
			K(resType);
		}
		else
		{
			type = scan->Scanner(lex);
			*resType = root->SemGetVar(lex)->GetType();
		}

	}
	else
	{
		type = scan->Scanner(lex);

		if (type == TConstInt)
		{
			*resType = TYPE_INT;
		}
		else if (type == TConstFloat)
		{
			*resType = TYPE_FLOAT;
		}
		else
		{
			scan->PrintError("Ожидалось выражение");
		}
	}
}



void TDiagram::K(DATA_TYPE* resType)			//Вызов функции
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
