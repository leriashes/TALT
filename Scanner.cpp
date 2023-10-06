#include "Scanner.h"

TScanner::TScanner(string FileName)
{
	GetData(FileName);
	PutUK(0);
}

LEX Keyword[MAX_KEYW] = { "int", "short", "long", "float",
							"main", "return", "while", "break" };

int IndexKeyword[MAX_KEYW] = { TInt, TShort, TLong, TFloat,
								TMain, TReturn, TWhile, TBreak };

//������������ ���������
void TScanner::PutUK(int i)
{
	uk = i;
}

//��������� ���������
int TScanner::GetUK()
{
	return uk;
}

//������ ��������� �� ������
void TScanner::PrintError(string error, string a)
{
	if (a[0] == '\0')
	{
		cout << "--- ������: " << error << " ---  ";
	}
	else
	{
		cout << "--- ������: " << error << ". �������� ������: \'" << a << "\' ---  ";
	}

	//exit(0);
}

int TScanner::Scanner(LEX lex)
{
	int i;          //������� ����� �������

	for (i = 0; i < MAX_LEX; i++) lex[i] = 0;    //������� ���� �������

	i = 0;


start:
	//��� ������������ ��������:
	while (text[uk] == ' ' || text[uk] == '\n' || text[uk] == '\t') uk++;      //������� ���������� ���������

	if (text[uk] == '/' && text[uk + 1] == '/')			//������� ������������ �����������, ����� ���������� ����� �� '\n'
	{
		uk += 2;
		while (text[uk] != '\n' && text[uk] != '\0') uk++;
		goto start;
	}

	if (text[uk] == '/' && text[uk + 1] == '*')			//������� ������������� �����������, ����� ���������� ����� �� "*/"
	{
		uk += 2;
		while ((text[uk] != '*' || text[uk + 1] != '/') && text[uk] != '\0') uk++;

		if (text[uk] == '*' && text[uk + 1] == '/')
			uk += 2;

		goto start;
	}

	if (text[uk] == '\0')
	{
		lex[0] = '#';
		return TEnd;
	}

	if (text[uk] >= '0' && text[uk] <= '9')
	{
		lex[i++] = text[uk++];

		while (text[uk] >= '0' && text[uk] <= '9')
		{
			if (i < MAX_CONST - 1) lex[i++] = text[uk++];
			else uk++;
		}

		if (i == MAX_CONST - 1)
		{
			PrintError("������� ������� ���������", lex);
			return TError;
		}

		if (text[uk] == '.')
		{
			lex[i++] = text[uk++];
			goto N1;
		}

		return TConstInt;
	}
	else if (text[uk] >= 'a' && text[uk] <= 'z' || text[uk] >= 'A' && text[uk] <= 'Z')			//���������� ������������� 
	{
		lex[i++] = text[uk++];

		while (text[uk] >= '0' && text[uk] <= '9' || text[uk] >= 'a' && text[uk] <= 'z' ||
			text[uk] >= 'A' && text[uk] <= 'Z' || text[uk] == '_')
		{
			if (i < MAX_LEX - 1) lex[i++] = text[uk++];
			else uk++;
		}

		int j;
		for (j = 0; j < MAX_KEYW; j++)
		{
			if (strcmp(lex, Keyword[j]) == 0)
				return IndexKeyword[j];
		}

		return TIdent;
	}
	else if (text[uk] == '.' && text[uk + 1] >= '0' && text[uk + 1] <= '9')
	{
		lex[i++] = text[uk++];
		lex[i++] = text[uk++];
		goto N1;
	}
	else if (text[uk] == ',')
	{
		lex[i++] = text[uk++];
		return TComma;
	}
	else if (text[uk] == ';')
	{
		lex[i++] = text[uk++];
		return TSemicolon;
	}
	else if (text[uk] == '(')
	{
		lex[i++] = text[uk++];
		return TLS;
	}
	else if (text[uk] == ')')
	{
		lex[i++] = text[uk++];
		return TRS;
	}
	else if (text[uk] == '{')
	{
		lex[i++] = text[uk++];
		return TFLS;
	}
	else if (text[uk] == '}')
	{
		lex[i++] = text[uk++];
		return TFRS;
	}
	else if (text[uk] == '+')
	{
		lex[i++] = text[uk++];
		return TPlus;
	}
	else if (text[uk] == '-')
	{
		lex[i++] = text[uk++];
		return TMinus;
	}
	else if (text[uk] == '*')
	{
		lex[i++] = text[uk++];
		return TMult;
	}
	else if (text[uk] == '/')
	{
		lex[i++] = text[uk++];
		return TDiv;
	}
	else if (text[uk] == '%')
	{
		lex[i++] = text[uk++];
		return TMod;
	}
	else if (text[uk] == '=')
	{
		lex[i++] = text[uk++];
		if (text[uk] == '=')
		{
			lex[i++] = text[uk++];
			return TEq;
		}

		return TSave;
	}
	else if (text[uk] == '!' && text[uk + 1] == '=')
	{
		lex[i++] = text[uk++];
		lex[i++] = text[uk++];
		return TNEq;
	}
	else if (text[uk] == '<')
	{
		lex[i++] = text[uk++];
		if (text[uk] == '=')
		{
			lex[i++] = text[uk++];
			return TLE;
		}
		else if (text[uk] == '<')
		{
			lex[i++] = text[uk++];
			return TLShift;
		}

		return TLT;
	}
	else if (text[uk] == '>')
	{
		lex[i++] = text[uk++];
		if (text[uk] == '=')
		{
			lex[i++] = text[uk++];
			return TGE;
		}
		else if (text[uk] == '>')
		{
			lex[i++] = text[uk++];
			return TRShift;
		}

		return TGT;
	}
	else 
	{
		lex[i++] = text[uk++];	//????
		PrintError("�������� ������", lex);
		//uk++;
		return TError;
	}

N1:
	while (text[uk] >= '0' && text[uk] <= '9')
	{
		if (i < MAX_CONST - 1)
			lex[i++] = text[uk++];
		else
			uk++;
	}

	if (i == MAX_CONST - 1)
	{
		PrintError("������� ������� ���������", lex);
		return TError;
	}

	return TConstFloat;
}

void TScanner::GetData(string FileName)
{
	ifstream file(FileName);
	char symb;

	if (!file.is_open())
	{
		PrintError("����������� ������� ����", "");
		exit(1);
	}
	else
	{
		int i = 0;

		while (!file.eof()) 
		{
			file.get(symb);
			if (!file.eof()) text[i++] = symb;
			if (i >= MAX_TEXT - 1)
			{
				PrintError("������� ������� ������ ��������� ������", "");
				break;
			}
		}

		text[i] = '\0';
		file.close();
	}
}
