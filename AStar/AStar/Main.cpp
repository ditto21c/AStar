#include<Windows.h>
#include<stdio.h>

INPUT_RECORD rec;
DWORD        dwNOER;

void CheckMouse()
{
	while (true)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER); // 콘솔창 입력을 받아들임.
		if (rec.EventType == MOUSE_EVENT) // 마우스 이벤트일 경우
		{
			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) // 좌측 버튼이 클릭되었을 경우
			{
				int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X값 받아옴
				int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y값 받아옴
				COORD Coor = { 0, 0 };
				DWORD dw;
				FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80 * 25, Coor, &dw); // 콘솔창 화면을 지운다.
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coor); // 커서를 0, 0으로 이동시킨다.
				printf("%d, %d", mouse_x, mouse_y); // 좌표를 출력한다.
				return;
			}
		}
	}
}

void main()
{
	HANDLE input_handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(input_handle, &dwNOER);
	SetConsoleMode(input_handle, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	while (true)
	{
		CheckMouse();
	}
}
