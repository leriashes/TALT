#include "Scanner.h" 

TScanner::TScanner(char* FileName)
{
    GetData(FileName);
    PutUK(0);
    SetLine(1);
    SetPos(1);
}

LEX Keyword[MAX_KEYW] = { "int", "short", "long", "float",
                            "main", "return", "while", "break" };

int IndexKeyword[MAX_KEYW]	 = { TInt, TShort, TLong, TFloat,
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

void TScanner::SetLine(int i)
{
    line = i;
}

void TScanner::NewLine()
{
    line += 1;
    SetPos(1);
}

int TScanner::GetLine()
{
    return line;
}

void TScanner::SetPos(int i)
{
    pos = i;
}

int TScanner::GetPos()
{
    return pos;
}

//������ ��������� �� ������
void TScanner::PrintError(const char* error, char* a)
{
    if (a[0] == '\0')
    {
        printf("--- ������: %s. ---   ������: %d  �������: %d\n", error, line, pos);
    }
    else
    {
        printf("--- ������: %s. \'%s\' ---  ������: %d  �������: %d\n", error, a, line, pos);
    }

    exit(0);
}

//������ ��������� �� ������
void TScanner::PrintError(const char* error)
{
    printf("--- ������: %s. ---    ������: %d  �������: %d\n", error, line, pos);

    exit(0);
}

int TScanner::Scanner(LEX lex)
{
    int i;          //������� ����� �������

    for (i = 0; i < MAX_LEX; i++) lex[i] = 0;    //������� ���� �������

    i = 0;
    

start:
    //��� ������������ ��������:
    while (text[uk] == ' ' || text[uk] == '\n' || text[uk] == '\t')       //������� ���������� ���������
    {
        if (text[uk] == ' ' || text[uk] == '\t')
        {
            pos++;
        }
        else
        {
            NewLine();
        }

        uk++;
    }


    if (text[uk] == '/' && text[uk + 1] == '/')			//������� ������������ �����������, ����� ���������� ����� �� '\n'
    {
        uk += 2;
        pos += 2;

        while (text[uk] != '\n' && text[uk] != '\0')
        {
            uk++;
            pos++;
        }

        NewLine();

        goto start;
    }

    if (text[uk] == '/' && text[uk + 1] == '*')			//������� ������������� �����������, ����� ���������� ����� �� "*/"
    {
        uk += 2;
        pos += 2;

        while ((text[uk] != '*' || text[uk + 1] != '/') && text[uk] != '\0')
        {
            if (text[uk] == '\n')
            {
                NewLine();
            }
            else
            {
                pos++;
            }

            uk++;
        }

        if (text[uk] == '*' && text[uk + 1] == '/')
        {
            uk += 2;
            pos += 2;
        }

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
        pos++;

        while (text[uk] >= '0' && text[uk] <= '9')
        {
            if (i < MAX_CONST + 1) lex[i++] = text[uk++];
            else uk++;

            pos++;
        }

        if (i == MAX_CONST + 1)
        {
            PrintError("������� ������� ���������", lex);
            return TError;
        }

        if (text[uk] == '.')
        {
            lex[i++] = text[uk++];
            pos++;
            goto N1;
        }

        return TConstInt;
    }
    else if (text[uk] >= 'a' && text[uk] <= 'z' || text[uk] >= 'A' && text[uk] <= 'Z')			//���������� ������������� 
    {
        lex[i++] = text[uk++];
        pos++;

        while (text[uk] >= '0' && text[uk] <= '9' || text[uk] >= 'a' && text[uk] <= 'z' ||
            text[uk] >= 'A' && text[uk] <= 'Z' || text[uk] == '_')
        {
            if (i < MAX_LEX - 1) lex[i++] = text[uk++];
            else uk++;

            pos++;
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
        pos += 2;
        goto N1;
    }
    else if (text[uk] == ',')
    {
        lex[i++] = text[uk++];
        pos++;
        return TComma;
    }
    else if (text[uk] == ';')
    {
        lex[i++] = text[uk++];
        pos++;
        return TSemicolon;
    }
    else if (text[uk] == '(')
    {
        lex[i++] = text[uk++];
        pos++;
        return TLS;
    }
    else if (text[uk] == ')')
    {
        lex[i++] = text[uk++];
        pos++;
        return TRS;
    }
    else if (text[uk] == '{')
    {
        lex[i++] = text[uk++];
        pos++;
        return TFLS;
    }
    else if (text[uk] == '}')
    {
        lex[i++] = text[uk++];
        pos++;
        return TFRS;
    }
    else if (text[uk] == '+')
    {
        lex[i++] = text[uk++];
        pos++;
        return TPlus;
    }
    else if (text[uk] == '-')
    {
        lex[i++] = text[uk++];
        pos++;
        return TMinus;
    }
    else if (text[uk] == '*')
    {
        lex[i++] = text[uk++];
        pos++;
        return TMult;
    }
    else if (text[uk] == '/')
    {
        lex[i++] = text[uk++];
        pos++;
        return TDiv;
    }
    else if (text[uk] == '%')
    {
        lex[i++] = text[uk++];
        pos++;
        return TMod;
    }
    else if (text[uk] == '=')
    {
        lex[i++] = text[uk++];
        pos++;

        if (text[uk] == '=')
        {
            lex[i++] = text[uk++];
            pos++;
            return TEq;
        }

        return TSave;
    }
    else if (text[uk] == '!' && text[uk + 1] == '=')
    {
        lex[i++] = text[uk++];
        lex[i++] = text[uk++];
        pos += 2;
        return TNEq;
    }
    else if (text[uk] == '<')
    {
        lex[i++] = text[uk++];
        pos++;

        if (text[uk] == '=')
        {
            lex[i++] = text[uk++];
            pos++;
            return TLE;
        }
        else if (text[uk] == '<')
        {
            lex[i++] = text[uk++];
            pos++;
            return TLShift;
        }

        return TLT;
    }
    else if (text[uk] == '>')
    {
        lex[i++] = text[uk++];
        pos++;

        if (text[uk] == '=')
        {
            lex[i++] = text[uk++];
            pos++;
            return TGE;
        }
        else if (text[uk] == '>')
        {
            lex[i++] = text[uk++];
            pos++;
            return TRShift;
        }

        return TGT;
    }
    else 
    {
        lex[i++] = text[uk++];
        pos++;
        PrintError("�������� ������", lex);
        return TError;
    }

N1:
    while (text[uk] >= '0' && text[uk] <= '9')
    {
        if (i < MAX_CONST + 2)
            lex[i++] = text[uk++];
        else
            uk++;

        pos++;
    }

    if (i == MAX_CONST + 2)
    {
        PrintError("������� ������� ���������", lex);
        return TError;
    }

    return TConstFloat;
}

void TScanner::GetData(char* FileName)
{
    ifstream file(FileName);
    char symb;

    if (!file.is_open())
    {
        PrintError("����������� ������� ����", nullptr);
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
                PrintError("������� ������� ������ ��������� ������", nullptr);
                break;
            }
        }

        text[i] = '\0';
        file.close();
    }
}
