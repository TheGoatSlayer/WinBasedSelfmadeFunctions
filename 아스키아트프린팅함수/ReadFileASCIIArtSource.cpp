#include "ReadFileASCIIArtHeader.h"

static void gotoxy(int x, int y); //커서 위치 이동 함수

bool PrintASCIIArt(FILE* fp)
{
	CONSOLE_SCREEN_BUFFER_INFO presentCur; // 커서 위치 변수
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur); //커서 위치 저장
	while (true)
	{
		char c = fgetc(fp);

		if (c == EOF) //END OF THE FILE
			return false;
		else if (c == '\n')
		{
			putchar(c);
			c = fgetc(fp);

			if (c == '\n') // 그림을 엔터로 구분, 그림 출력 완료
				return true; // while(PrintASCIIArt())를 위해 true반환
			else
			{
				gotoxy(presentCur.dwCursorPosition.X, ++(presentCur.dwCursorPosition.Y)); //x값 고정으로 그림 깨짐 방지
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

