#pragma once
#include <iostream>
#include <fstream>
using namespace std;

#define MAX_TEXT 100000     //������������ ����� ������ ��
#define MAX_LEX 100         //������������ ����� �������
#define MAX_KEYW 8			//����� �������� ����
#define MAX_CONST 10		//����� ���������

typedef char IM[MAX_TEXT];   //����� ��
typedef char LEX[MAX_LEX];   //�������

//�������� �����
#define TInt       1		//int
#define TShort     2		//short
#define TLong      3		//long
#define TFloat     4		//float
#define TMain      5		//main
#define TReturn    6		//return
#define TWhile     7		//while
#define TBreak     8		//break

//��������������
#define TIdent      20		//�������������

//���������
#define TConstInt      30	//������������� ���������
#define TConstFloat    31	//��������� � ������������� ������

//����������� �����
#define TComma      40		// ,
#define TSemicolon  41		// ;
#define TLS         42		// (
#define TRS         43		// )
#define TFLS        44		// {
#define TFRS        45		// }

//����� ��������
#define TSave       50		// =
#define TEq         51		// ==
#define TNEq        52		// !=
#define TLT         53		// <
#define TGT         54		// >
#define TLE         55		// <=
#define TGE         56		// >=
#define TLShift     57		// <<
#define TRShift     58		// >>
#define TPlus       59		// +
#define TMinus      60		// -
#define TMult       61		// *
#define TDiv        62		// /
#define TMod        63		// %

//����� ��������� ������
#define TEnd        100

//��������� ������
#define TError      200