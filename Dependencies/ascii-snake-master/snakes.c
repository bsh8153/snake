#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
//#include "graphics.h"

#define BOUNDARY 100

/// <summary>
/// �ܼ�â �󿡼� Ŀ���� ��ġ ����
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(short x, short y)
{
	/* positions cursor to coodinate (x,y) */
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);	// �ܼ� Ŀ���� ��ġ�� ����
}

/// <summary>
/// ������ũ �׸��� �Լ�
/// </summary>
/// <param name="arr"></param>
/// <param name="le"></param>
/// <returns></returns>
int redraw(int arr[BOUNDARY][2], int le)
{
	gotoxy(arr[0][0], arr[0][1]);
	printf("%s", "+");	// ����

	for (int i = 1; i < le; i++)
	{
		gotoxy(arr[i][0], arr[i][1]);
		if (!(i == (le - 1)))
			printf("%s", "#");	// ����
		else
			printf("%s", "��");	// �Ӹ�
	}
}

/// <summary>
/// ��ġ ���� (x, y)
/// </summary>
/// <param name="x">x��ǥ ���� �ּ�</param>
/// <param name="y">y��ǥ ���� �ּ�</param>
/// <param name="move">�̵� ���� (-1 ����, 1: ������ ��0�̸� �ȵ�)</param> // => �̵� ������ define�Ͽ� �ҽ��ڵ带 �д� ����� ���⿡ ���� ����� ������ �� �ֵ��� �ϱ�
void movechange(int *x, int *y, int move)
{
	if (move == 1 || move == -1)
		*x = *x + move;
	else
		*y = *y + (move / 2);
}

/// <summary>
/// ���� �Ծ��� �� �۵�
/// (������ũ�� ���� ����,
/// ���ھ� ����)
/// </summary>
/// <param name="arr"></param>
/// <param name="le"></param>
/// <param name="fx"></param>
/// <param name="fy"></param>
/// <param name="s">����</param>
void foodgained(int arr[BOUNDARY][2], int *le, int *fx, int *fy, int *s)
{
	if (arr[*le - 1][0] == *fx&&arr[*le - 1][1] == *fy)
	{
		gotoxy(*fx = rand() % 30 + 6, *fy = rand() % 20 + 6);	// Ŀ���� x, y ��ġ�� ��ġ��Ű��
		printf("%s", "X");										// ����(X) ���
		(*s)++;
		gotoxy(50, 0);
		printf("Score: %d", *s);
		arr[*le][0] = arr[*le - 1][0];
		arr[*le][1] = arr[*le - 1][1];
		(*le)++;
	}
}

/// <summary>
/// (���� �̸� repaint) => maintLastDirection ���������� �Էµ� �������� ������ũ�� ����
/// </summary>
/// <param name="arr"></param>
/// <param name="move"></param>
/// <param name="le"></param>
void maintLastDirection(int arr[BOUNDARY][2], int move, int *le)
{
	int i = 0;
	gotoxy(arr[0][0], arr[0][1]);
	printf("%s", " ");
	for (i = 0; i < *le - 1; i++) {
		arr[i][0] = arr[i + 1][0];
		arr[i][1] = arr[i + 1][1];
	}
	arr[*le - 1][0] = arr[*le - 2][0];
	arr[*le - 1][1] = arr[*le - 2][1];
	movechange(&arr[*le - 1][0], &arr[*le - 1][1], move);
}

/// <summary>
/// �浹 (��ȯ���� �Ұ����� �����ϱ�)
/// </summary>
/// <param name="arr"></param>
/// <param name="le"></param>
/// <returns></returns>
short collision(int arr[BOUNDARY][2], int le)
{
	int i, j;
	for (i = 0; i < le - 2; i++)
	{
		if (arr[i][0] == arr[le - 1][0] && arr[le - 1][1] == arr[i][1])
		{
			return 1;	// �浹 ���� �� ��ȯ
		}

	}
	return 0;		// �浹 ���� �ʾ��� �� ��ȯ
}

/// <summary>
/// ��ġ �ʱ�ȭ (reinit -> init���� �̸� ����)
/// </summary>
/// <param name="arr"></param>
/// <param name="fx"></param>
/// <param name="fy"></param>
/// <param name="le"></param>
/// <param name="s"></param>
void init(int arr[BOUNDARY][2], int *fx, int *fy, int *le, int *s)
{
	gotoxy(*fx = rand() % 30 + 1, *fy = rand() % 30 + 1);
	printf("%s ", "X");
	int r = 0;
	*le = 3;
	for (r = 0; r < *le; r++)
	{
		arr[r][0] = r + 1;
		arr[r][1] = 0;
	}
	*s = 0;
}

/// <summary>
/// cmdâ���� ��ũ�ѹ� ����
/// </summary>
void remove_scrollbar()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
}




/// <summary>
/// ���� �Լ�
/// </summary>
/// <returns></returns>
int main()
{
	// �ܼ�â ��ũ�ѹ� ����
	remove_scrollbar();

	// textcolor(RED);
	int arr[BOUNDARY][2];
	int le = 0;
	int move = 1;
	int r;
	int t = 0;
	int fx;
	int fy;
	int over = 0;
	int s = 0;	// ����

	init(arr, &fx, &fy, &le, &s);
	while (1) {
		if (_kbhit()) {
			/*controls for the snake <ASDW> */
			r = _getch();
			if (r == 'w' && move != 2)	// ��
				move = -2;
			else if (r == 's' && move != -2)	// ��
				move = 2;
			else if (r == 'd' && move != -1)	// ��
				move = 1;
			else if (r == 'a' && move != 1)		// ��
				move = -1;
		}
		Sleep(10);	// �ӵ� ����
		t++;
		if (t == 10)
		{
			maintLastDirection(arr, move, &le);
			redraw(arr, le);
			foodgained(arr, &le, &fx, &fy, &s);
			if (collision(arr, le)) {
				_cprintf("Game Over-- X ---");
				_getch();
				_cprintf("Restart the game.....");
				_getch();
				break;
			}

			t = 0;
		}
	}

	return 0;
}