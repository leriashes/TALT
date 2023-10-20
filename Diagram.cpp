#include "Diagram.h"

void TDiagram::S()			//программа
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
	int uk1, uk2;

	uk1 = scan->GetUK();
	type = scan->Scanner(lex);

	while (type == TShort || type == TLong || type == TInt || type == TFloat)
	{
		if (type == TShort || type == TLong)
		{
			uk2 = scan->GetUK();
			type = scan->Scanner(lex);

			if (type != TInt)
			{
				scan->PutUK(uk2);
			}
		}

		type = scan->Scanner(lex);

		if (type == TMain)
		{
			scan->PutUK(uk1);
			F();
		}
		else if (type == TIdent)
		{
			type = scan->Scanner(lex);
			scan->PutUK(uk1);

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
			scan->PrintError("Ожидался идентификатор", lex);
		}

		uk1 = scan->GetUK();
		type = scan->Scanner(lex);
	}

	scan->PutUK(uk1);
}



void TDiagram::D()			//описание данных
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
	int uk1;

	type = scan->Scanner(lex);

	if (type != TLong && type != TShort && type != TInt && type != TFloat)
	{
		scan->PrintError("Ожидался тип", lex);
	}

	if (type == TLong || type == TShort)
	{
		uk1 = scan->GetUK();
		type = scan->Scanner(lex);

		if (type != TInt)
		{
			scan->PutUK(uk1);
		}
	}


	do
	{
		type = scan->Scanner(lex);

		if (type != TIdent)
		{
			scan->PrintError("Ожидался идентификатор", lex);
		}

		type = scan->Scanner(lex);

		if (type == TSave)
		{
			V();

			type = scan->Scanner(lex);
		}

	} while (type == TComma);

	if (type != TSemicolon)
	{
		scan->PrintError("Ожидался символ ';'", lex);
	}
}



void TDiagram::F()			//функция
// 
//		----- a -----                -----
// -----|			|--- ( --- ) ----| Q |---->
//		---- main ---				 -----
//
{
	LEX lex;
	int type;
	int uk1;

	type = scan->Scanner(lex);

	if (type != TLong && type != TShort && type != TInt && type != TFloat)
	{
		scan->PrintError("Ожидался тип", lex);
	}

	if (type == TLong || type == TShort)
	{
		uk1 = scan->GetUK();
		type = scan->Scanner(lex);

		if (type != TInt)
		{
			scan->PutUK(uk1);
		}
	}

	type = scan->Scanner(lex);

	if (type != TMain && type != TIdent)
	{
		scan->PrintError("Ожидалось имя функции", lex);
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ '('", lex);
	}

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ ')'", lex);
	}

	Q();
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
	int uk1;

	type = scan->Scanner(lex);

	if (type != TFLS)
	{
		scan->PrintError("Ожидался символ '{'", lex);
	}

	uk1 = scan->GetUK();
	type = scan->Scanner(lex);
	scan->PutUK(uk1);

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

		uk1 = scan->GetUK();
		type = scan->Scanner(lex);
		scan->PutUK(uk1);
	}

	type = scan->Scanner(lex);

	if (type != TFRS)
	{
		scan->PrintError("Ожидался символ '}'", lex);
	}
}



void TDiagram::A()			//оператор
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
	int uk1;

	uk1 = scan->GetUK();
	type = scan->Scanner(lex);

	if (type == TFLS)
	{
		scan->PutUK(uk1);
		Q();
	}
	else if (type == TWhile)
	{
		scan->PutUK(uk1);
		W();
	}
	else
	{
		if (type == TReturn)
		{
			scan->PutUK(uk1);
			R();
		}
		else if (type == TBreak)
		{
			scan->PutUK(uk1);
			B();
		}
		else if (type == TMain)
		{
			scan->PutUK(uk1);
			K();
		}
		else if (type == TIdent)
		{
			type = scan->Scanner(lex);
			scan->PutUK(uk1);

			if (type == TSave)
			{
				P();
			}
			else
			{
				K();
			}
		}

		type = scan->Scanner(lex);

		if (type != TSemicolon)
		{
			scan->PrintError("Ожидался символ ';'", lex);
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
		scan->PrintError("Ожидался оператор 'while'", lex);
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ '('", lex);
	}

	V();

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ ')'", lex);
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
		scan->PrintError("Ожидался идентификатор", lex);
	}

	type = scan->Scanner(lex);

	if (type != TSave)
	{
		scan->PrintError("Ожидался знак =", lex);
	}

	V();
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
		scan->PrintError("Ожидался оператор 'return'", lex);
	}

	V();
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
		scan->PrintError("Ожидался оператор 'break'", lex);
	}
}



void TDiagram::V()			//Выражение
//               
//           ----- == ----
//        ---|           |---
//        |  ----- != ----  |
//       \|/     -----      |
//--------.------| Z |---------->
//               -----
//
{
	LEX lex;
	int type;
	int uk1;

	do
	{
		Z();
		uk1 = scan->GetUK();
		type = scan->Scanner(lex);
	} while (type == TEq || type == TNEq);

	scan->PutUK(uk1);
}



void TDiagram::Z()			//Сравнение
//          
//           ----- < -----   
//           |           |
//           ----- > -----   
//           |           |
//           ----- <= ----
//        ---|           |---
//        |  ----- >= ----  |
//       \|/     -----      |
//--------.------| Y |---------->
//               -----
//
{
	LEX lex;
	int type;
	int uk1;

	do
	{
		Y();
		uk1 = scan->GetUK();
		type = scan->Scanner(lex);
	} while (type == TLT || type == TGT || type == TLE || type == TGE);

	scan->PutUK(uk1);
}



void TDiagram::M()			//Множитель
//          
//           ----- * ----   
//           |          |
//           ----- / ----
//        ---|          |---
//        |  ----- % ----  |
//       \|/     -----     |
//--------.------| N |---------->
//               -----
//
{
	LEX lex;
	int type;
	int uk1;

	do
	{
		N();
		uk1 = scan->GetUK();
		type = scan->Scanner(lex);
	} while (type == TMult || type == TDiv || type == TMod);

	scan->PutUK(uk1);
}



void TDiagram::Y()			//Сдвиг
//          
//           ----- << ----
//        ---|           |---
//        |  ----- >> ----  |
//       \|/     -----      |
//--------.------| L |---------->
//               -----
//
{
	LEX lex;
	int type;
	int uk1;

	do
	{
		L();
		uk1 = scan->GetUK();
		type = scan->Scanner(lex);
	} while (type == TLShift || type == TRShift);

	scan->PutUK(uk1);
}



void TDiagram::L()			//Слагаемое
//          
//           ----- + ----
//        ---|          |---
//        |  ----- - ----  |
//       \|/     -----     |
//--------.------| M |---------->
//               -----
//
{
	LEX lex;
	int type;
	int uk1;

	do
	{
		M();
		uk1 = scan->GetUK();
		type = scan->Scanner(lex);
	} while (type == TPlus || type == TMinus);

	scan->PutUK(uk1);
}



void TDiagram::N()			//Со знаком
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
	int uk1, uk2;

	uk1 = scan->GetUK();
	type = scan->Scanner(lex);

	if (type != TPlus && type != TMinus)
	{
		scan->PutUK(uk1);
	}

	uk1 = scan->GetUK();
	type = scan->Scanner(lex);

	if (type == TLS)
	{
		V();

		type = scan->Scanner(lex);

		if (type != TRS)
		{
			scan->PrintError("Ожидался символ ')'", lex);
		}
	}
	else if (type == TMain)
	{
		scan->PutUK(uk1);
		K();
	}
	else if (type == TIdent)
	{
		uk2 = scan->GetUK();
		type = scan->Scanner(lex);

		if (type == TLS)
		{
			scan->PutUK(uk1);
			K();
		}

		scan->PutUK(uk2);
	}
	else if (type != TConstInt && type != TConstFloat)
	{
		scan->PrintError("Ожидалось элементарное выражение", lex);
	}
}



void TDiagram::K()			//Вызов функции
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
		scan->PrintError("Ожидалось имя функции", lex);
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("Ожидался символ '('", lex);
	}

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("Ожидался символ ')'", lex);
	}
}
