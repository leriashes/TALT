#include "Diagram.h"

void TDiagram::S()			//���������
//               -----
//            ---| D |---
//		     |	 _____   |
//        ---|           |---
//        |  |   -----   |  |
//        |   ---| F |---   |
//       \|/     -----      |
//--------.----------------------->
//
{
	LEX lex;
	int type;
	int uk1;

	uk1 = scan->GetUK();
	type = scan->Scanner(lex);

	while (type == TShort || type == TLong || type == TInt || type == TFloat)
	{
		if (type == TShort || type == TLong)
		{
			uk1 = scan->GetUK();
			type = scan->Scanner(lex);

			if (type != TInt)
			{
				scan->PutUK(uk1);
			}
		}

		uk1 = scan->GetUK();
		type = scan->Scanner(lex);

		scan->PutUK(uk1);

		if (type == TMain)
		{
			F();
		}
		else if (type == TIdent)
		{
			type = scan->Scanner(lex);

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
			scan->PrintError("�������� �������������", lex);
		}

		uk1 = scan->GetUK();
		type = scan->Scanner(lex);
	}

	scan->PutUK(uk1);
}



void TDiagram::D()			//�������� ������
// 
//        ----------------------- , ----
//        |                 -----      |
//	      |         -- = ---| V |---   |
//       \|/        |       -----  |   |
//--------.--- a ------------------------ ; --->
//
{
	LEX lex;
	int type;
	int uk1;

	do
	{
		type = scan->Scanner(lex);

		if (type != TIdent)
		{
			scan->PrintError("�������� �������������", lex);
		}

		uk1 = scan->GetUK();
		type = scan->Scanner(lex);

		if (type == TSave)
		{
			V();

			uk1 = scan->GetUK();
			type = scan->Scanner(lex);
		}

	} while (type == TComma);

	if (type != TSemicolon)
	{
		scan->PrintError("�������� ������ ';'", lex);
	}
}



void TDiagram::F()			//�������
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

	if (type != TMain || type != TIdent)
	{
		scan->PrintError("��������� ��� �������", lex);
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ '('", lex);
	}

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'", lex);
	}

	Q();
}



void TDiagram::Q()			//��������� ��������
//               -----
//            ---| D |---
//		     |	 _____   |
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
		scan->PrintError("�������� ������ '{'", lex);
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

	if (type != TFRS)
	{
		scan->PrintError("�������� ������ '}'", lex);
	}
}



void TDiagram::A()			//��������
//               -----
//        -------| Q |------------
//		  |	     _____           |
//        |      -----           |
//        |------| W |-----------|
//        |      -----           |
//        |      -----           |
//        |   ---| P |---        |
//		  |  |	 _____   |       |
//-----------|-----------|-- ; -------->
//           |   -----   |  
//           |---| K |---|  
//           |   -----   |  
//           |   -----   |
//           |---| R |---|
//		     |	 _____   |
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
	scan->PutUK(uk1);

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
			K();
		}
		else if (type == TIdent)
		{
			uk1 = scan->GetUK();
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

		uk1 = scan->GetUK();
		type = scan->Scanner(lex);

		if (type != TSemicolon)
		{
			scan->PrintError("�������� ������ ';'", lex);
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
	int uk1;

	type = scan->Scanner(lex);

	if (type != TWhile)
	{
		scan->PrintError("�������� �������� 'while'", lex);
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ '('", lex);
	}

	V();

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'", lex);
	}

	A();
}



void TDiagram::P()			//������������
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
		scan->PrintError("�������� �������������", lex);
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ���� =", lex);
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
		scan->PrintError("�������� �������� 'return'", lex);
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
		scan->PrintError("�������� �������� 'break'", lex);
	}
}



void TDiagram::V()			//���������
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
		type = scan->Scanner(lex);
		uk1 = scan->GetUK();
	} while (type == TEq || type == TNEq);

	scan->PutUK(uk1);
}



void TDiagram::Z()			//���������
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
		type = scan->Scanner(lex);
		uk1 = scan->GetUK();
	} while (type == TLT || type == TGT || type == TLE || type == TGE);

	scan->PutUK(uk1);
}



void TDiagram::M()			//���������
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
		type = scan->Scanner(lex);
		uk1 = scan->GetUK();
	} while (type == TMult || type == TDiv || type == TMod);

	scan->PutUK(uk1);
}



void TDiagram::Y()			//�����
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
		type = scan->Scanner(lex);
		uk1 = scan->GetUK();
	} while (type == TLShift || type == TRShift);

	scan->PutUK(uk1);
}



void TDiagram::L()			//���������
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



void TDiagram::N()			//�� ������
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
	int uk1;

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
			scan->PrintError("�������� ������ ')'", lex);
		}
	}
	else if (type == TMain)
	{
		scan->PutUK(uk1);
		K();
	}
	else if (type == TIdent)
	{
		int uk2;
		
		uk2 = scan->GetUK();
		type = scan->Scanner(lex);

		if (type == TLS)
		{
			scan->PutUK(uk1);
			K();
		}
	}
	else if (type != TConstInt && type != TConstFloat)
	{
		scan->PrintError("��������� ������������ ���������", lex);
	}
}



void TDiagram::K()			//����� �������
// 
//		----- a -----                
// -----|			|--- ( --- ) ----->
//		---- main ---				 
//
{
	LEX lex;
	int type;
	int uk1;

	type = scan->Scanner(lex);

	if (type != TMain || type != TIdent)
	{
		scan->PrintError("��������� ��� �������", lex);
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ '('", lex);
	}

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'", lex);
	}
}
