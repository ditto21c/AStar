#include<Windows.h>
#include<stdio.h>

INPUT_RECORD rec;
DWORD        dwNOER;

void CheckMouse()
{
	while (true)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER); // �ܼ�â �Է��� �޾Ƶ���.
		if (rec.EventType == MOUSE_EVENT) // ���콺 �̺�Ʈ�� ���
		{
			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) // ���� ��ư�� Ŭ���Ǿ��� ���
			{
				int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X�� �޾ƿ�
				int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y�� �޾ƿ�
				COORD Coor = { 0, 0 };
				DWORD dw;
				FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80 * 25, Coor, &dw); // �ܼ�â ȭ���� �����.
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coor); // Ŀ���� 0, 0���� �̵���Ų��.
				printf("%d, %d", mouse_x, mouse_y); // ��ǥ�� ����Ѵ�.
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
