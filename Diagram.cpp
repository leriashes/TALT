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

void TDiagram::S()			//���������
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
			scan->PrintError("�������� �������������");
		}

		type = LookForward(1);
	}
}



void TDiagram::D()			//�������� ������
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
		scan->PrintError("�������� ���");
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
			scan->PrintError("�������� �������������");
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
		scan->PrintError("�������� ������ ';'");
	}
}



void TDiagram::F()			//�������
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
		scan->PrintError("�������� ���");
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
		scan->PrintError("��������� ��� �������");
	}

	Tree* v = root->SemInclude(lex, ObjFunct, semType);

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ '('");
	}

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'");
	}

	Q();

	root->SetCur(v);
}



void TDiagram::Q()			//��������� ��������
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
		scan->PrintError("�������� ������ '{'");
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
		scan->PrintError("�������� ������ '}'");
	}

	root->SetCur(v);
}



void TDiagram::A()			//��������
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
			scan->PrintError("�������� ������ ';'");
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
		scan->PrintError("�������� �������� 'while'");
	}

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ '('");
	}

	DATA_TYPE valType;
	V(&valType);

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'");
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
		scan->PrintError("�������� �������������");
	}

	Tree* ident = root->SemGetVar(lex);

	type = scan->Scanner(lex);

	if (type != TSave)
	{
		scan->PrintError("�������� ���� =");
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
		scan->PrintError("�������� �������� 'return'");
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
		scan->PrintError("�������� �������� 'break'");
	}
}



void TDiagram::V(DATA_TYPE* resType)			//���������
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



void TDiagram::Z(DATA_TYPE* resType)			//���������
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



void TDiagram::M(DATA_TYPE* resType)			//���������
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



void TDiagram::Y(DATA_TYPE* resType)			//�����
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



void TDiagram::L(DATA_TYPE* resType)			//���������
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



void TDiagram::N(DATA_TYPE* resType)			//�� ������
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
			scan->PrintError("�������� ������ ')'");
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
			scan->PrintError("��������� ���������");
		}
	}
}



void TDiagram::K(DATA_TYPE* resType)			//����� �������
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
		scan->PrintError("��������� ��� �������");
	}

	Tree* funct = root->SemGetFunct(lex);

	type = scan->Scanner(lex);

	if (type != TLS)
	{
		scan->PrintError("�������� ������ '('");
	}

	type = scan->Scanner(lex);

	if (type != TRS)
	{
		scan->PrintError("�������� ������ ')'");
	}
}
