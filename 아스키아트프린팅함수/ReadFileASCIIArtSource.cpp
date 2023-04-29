#include "ReadFileASCIIArtHeader.h"

static void gotoxy(int x, int y); //Ŀ�� ��ġ �̵� �Լ�

bool PrintASCIIArt(FILE* fp)
{
	CONSOLE_SCREEN_BUFFER_INFO presentCur; // Ŀ�� ��ġ ����
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur); //Ŀ�� ��ġ ����
	while (true)
	{
		char c = fgetc(fp);

		if (c == EOF) //END OF THE FILE
			return false;
		else if (c == '\n')
		{
			putchar(c);
			gotoxy(presentCur.dwCursorPosition.X, ++(presentCur.dwCursorPosition.Y)); //x�� �������� �׸� ���� ����
			c = fgetc(fp);

			if (c == '\n') // �׸��� ���ͷ� ����, �׸� ��� �Ϸ�
				return true; // while(PrintASCIIArt())�� ���� true��ȯ
			else
			{

				putchar(c);
			}
		}
		else
		{

			putchar(c);
		}
	}
}

static void gotoxy(int x, int y)
{
	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

