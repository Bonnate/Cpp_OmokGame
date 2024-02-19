////////////////////////////////////////////////////////////////
//
// ���α׷� �ۼ���:	2021.03.26
// �ֱ� ������:		2021.04.02
// �ۼ���:			�ڽ±� 20180952
// ����:				���� ����
// 
// MEMO //	
// 19*19 �ٵ��� �԰��̳�, 15*15 ������ �԰� ��� ����մϴ�.
// �浹�� 3*3 �ݼ� ó����.
// 4*4�� ��, �� ��� �� �� �ִ� ������.
// ���(6���̻�) ��, �� ��� �� ���� �ֵ�, ���п� ������ ����.
////////////////////////////////////////////////////////////////


//����� ��Ʈ�� ����
#include <iostream>
//key��, gotoxy�� �������� ��� ����
#include <Windows.h>
//kbhit func
#include <conio.h>
//vector structure
#include <vector>
//pow func
#include <math.h>

using namespace std;

//���� ũ��(��)
constexpr int MAX_ROW_OMOK_SIZE = 15;
//���� ũ��(��)
constexpr int MAX_COL_OMOK_SIZE = 15;

//gotoxy
void gotoxy(int x, int y)
{
	//pos struct�� x�� y(�ܼ��� ����)�� ���� �����ϴ°�.
	//gotoxy�� x,y ���ڸ� �̿��Ͽ� pos��ü�� �ӽ÷� �����ϰ�
	//SetConsoleCursorPosition�Լ��� �̿��Ͽ� Ŀ���� �̵���Ŵ
	COORD pos = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//textcolor
void textcolor(int foreground, int background)
{
	//color�� �ܼ��� �����ڵ带 ��Ʈȭ���Ѽ� ���� �Ѱ��� ��ȯ�����ش�.
	//*16�� ����Ͽ� ��Ʈ�� �̵���Ŵ (^4)
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Ŀ�������
void CursorView(char show)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

//���α׷� ���� �� ������ �迭�� ���Ŀ� �°� �Ҵ�����
void InitOmok(char*** arr);

//���� ���� �� �����ǰ� �ٸ� ��ҵ� �׸���
void PaintGame(char*** arr);

//33��Ģ�� Ž��
bool Check33_1111(int row, int col, const char* STONE, char*** arr);
bool Check33_112(int row, int col, const char* STONE, char*** arr);
bool Check33_22(int row, int col, const char* STONE, char*** arr);

//33��Ģ ���ݽ� �˸�
void AnnounceRule(int row, int col, char*** arr);

//���� ���� �������� 4����*2 üũ [����: ��, ��, �����, ������ �迭]
bool CheckFinish(int row, int col, const char* STONE, char*** arr);

//������ ������
void RunGame(char*** arr, int& row, int& col, bool& isBlackTurn);

//�Ҵ�� �޸𸮸� ��ȯ
void ReleaseMemory(char*** arr);

int main()
{
	

	//�浹, �鵹�� ������� ���ư��� boolean�� ����, �浹�� ���� ���� ����
	bool isBlackTurn = TRUE;

	//���� Ŀ���� ��ġ�� ���� ��ǥ�� ��Ÿ��, ��� ��
	int row, col;

	//���� ������ ������ ũ���� �߽ɿ��� ����
	row = MAX_ROW_OMOK_SIZE / 2;
	col = MAX_COL_OMOK_SIZE / 2;

	//2���� ������ char���� �����Ҵ����� ����
	char*** arr = new char** [MAX_ROW_OMOK_SIZE];
	for (int i = 0; i < MAX_ROW_OMOK_SIZE; ++i)
		arr[i] = new char* [MAX_COL_OMOK_SIZE];

	//������ �ʱ�ȭ -> ȭ�� ��� -> �Է��� �ݺ��ϴ� RunGame -> ����� �޸� ��ȯ
	InitOmok(arr);
	PaintGame(arr);
	RunGame(arr, row, col, isBlackTurn);
	ReleaseMemory(arr);

	return 0;
}

bool Check33_1111(int row, int col, const char* STONE, char*** arr)
{
	//+4���� �������� �ݼ��� �ƴ�
	const char* ENEMY_STONE = STONE == "��" ? "��" : "��";

	//3: 2 2		blank = 2
	//2: 1 1 2		�� ���⿡ ��ĭ 2���� ��ȿ, �� ��ĭ�� 3�� �̻��̸� ��ȿ
	//1: 1 1 1 1	blank = 0

	unsigned char _FIRST_PER_Stack = 0;
	unsigned char _SECOND_PER_Stack = 0;

	unsigned char _FIRST_blankPerCase = 0;
	unsigned char _SECOND_blankPerCase = 0;

	unsigned char _case_1_blankCount = 0;
	unsigned char _case_2_blankCount = 0;
	unsigned char _case_3_blankCount = 0;
	unsigned char _case_4_blankCount = 0;

	unsigned char checkStone = 0;

	bool is_StoneAvailable = FALSE;



	//case 1 �� ��
	for (int i = 1; i <= 2 && col - i >= 0; ++i)
	{
		if (arr[row][col - i] == ENEMY_STONE) return FALSE;
		if (arr[row][col - i] == STONE)
		{
			if (col - i - 1 < 0 || arr[row][col - i - 1] == ENEMY_STONE) return FALSE;
			is_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!is_StoneAvailable && arr[row][col - i] != ENEMY_STONE && arr[row][col - i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!is_StoneAvailable) _FIRST_blankPerCase = 0;
	is_StoneAvailable = FALSE;

	for (int i = 1; i <= 2 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row][col + i] == ENEMY_STONE) return FALSE;
		if (arr[row][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || arr[row][col + i + 1] == ENEMY_STONE) return FALSE;
			is_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!is_StoneAvailable && arr[row][col + i] != ENEMY_STONE && arr[row][col + i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!is_StoneAvailable) _SECOND_blankPerCase = 0;
	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	if (_FIRST_PER_Stack == 1 && _SECOND_PER_Stack == 1 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_case_1_blankCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		++checkStone;
	}

	is_StoneAvailable = _FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;



	//case 2 �� ��
	for (int i = 1; i <= 2 && row - i >= 0; ++i)
	{
		if (arr[row - i][col] == ENEMY_STONE) return FALSE;
		if (arr[row - i][col] == STONE)
		{
			if (row - i - 1 < 0 || arr[row - i - 1][col] == ENEMY_STONE) return FALSE;
			is_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!is_StoneAvailable && arr[row - i][col] != ENEMY_STONE && arr[row - i][col] != STONE) ++_FIRST_blankPerCase;
	}
	if (!is_StoneAvailable) _FIRST_blankPerCase = 0;
	is_StoneAvailable = FALSE;

	for (int i = 1; i <= 2 && row + i <= MAX_ROW_OMOK_SIZE - 1; ++i)
	{
		if (arr[row + i][col] == ENEMY_STONE) return FALSE;
		if (arr[row + i][col] == STONE)
		{
			if (row + i + 1 >= MAX_ROW_OMOK_SIZE || arr[row + i + 1][col] == ENEMY_STONE) return FALSE;
			is_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!is_StoneAvailable && arr[row + i][col] != ENEMY_STONE && arr[row + i][col] != STONE) ++_SECOND_blankPerCase;
	}
	if (!is_StoneAvailable) _SECOND_blankPerCase = 0;
	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	if (_FIRST_PER_Stack == 1 && _SECOND_PER_Stack == 1 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_case_2_blankCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		++checkStone;
	}

	is_StoneAvailable = _FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;



	//case 3 �آ�
	for (int i = 1; i <= 2 && row - i >= 0 && col - i >= 0; ++i)
	{
		if (arr[row - 1][col - i] == ENEMY_STONE) return FALSE;
		if (arr[row - i][col - i] == STONE)
		{
			if (col - i - 1 < 0 || row - i - 1 < 0 || arr[row - i - 1][col - i - 1] == ENEMY_STONE) return FALSE;
			is_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!is_StoneAvailable && arr[row - i][col - i] != ENEMY_STONE && arr[row - i][col - i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!is_StoneAvailable) _FIRST_blankPerCase = 0;
	is_StoneAvailable = FALSE;

	for (int i = 1; i <= 2 && row + i <= MAX_ROW_OMOK_SIZE - 1 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row + i][col + i] == ENEMY_STONE) return FALSE;
		if (arr[row + i][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || row + i + 1 >= MAX_ROW_OMOK_SIZE || arr[row + i + 1][col + i + 1] == ENEMY_STONE) return FALSE;
			is_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!is_StoneAvailable && arr[row + i][col + i] != ENEMY_STONE && arr[row + i][col + i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!is_StoneAvailable) _SECOND_blankPerCase = 0;

	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;
	if (_FIRST_PER_Stack == 1 && _SECOND_PER_Stack == 1 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_case_3_blankCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		++checkStone;
	}

	is_StoneAvailable = _FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;



	//case 4 �֢�
	for (int i = 1; i <= 2 && row - i >= 0 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row - i][col + i] == ENEMY_STONE) return FALSE;
		if (arr[row - i][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || row - i - 1 < 0 || arr[row - i - 1][col + i + 1] == ENEMY_STONE) return FALSE;
			is_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!is_StoneAvailable && arr[row - i][col + i] != ENEMY_STONE && arr[row - i][col + i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!is_StoneAvailable) _FIRST_blankPerCase = 0;
	is_StoneAvailable = FALSE;

	for (int i = 1; i <= 2 && row + i <= MAX_ROW_OMOK_SIZE - 1 && col - i >= 0; ++i)
	{
		if (arr[row + i][col - i] == ENEMY_STONE) return FALSE;
		if (arr[row + i][col - i] == STONE)
		{
			if (col - i - 1 < 0 || row + i + 1 >= MAX_ROW_OMOK_SIZE || arr[row + i + 1][col - i - 1] == ENEMY_STONE) return FALSE;
			is_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!is_StoneAvailable && arr[row + i][col - i] != ENEMY_STONE && arr[row + i][col - i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!is_StoneAvailable) _SECOND_blankPerCase = 0;
	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	if (_FIRST_PER_Stack == 1 && _SECOND_PER_Stack == 1 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_case_4_blankCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		++checkStone;
	}

	if (checkStone >= 2 && (_case_1_blankCount + _case_2_blankCount + _case_3_blankCount + _case_4_blankCount) < 3) return TRUE;
	return FALSE;
}

bool Check33_112(int row, int col, const char* STONE, char*** arr)
{
	//+4���� �������� �ݼ��� �ƴ�
	const char* ENEMY_STONE = STONE == "��" ? "��" : "��";

	//3: 2 2		blank = 2
	//2: 1 1 2		�� ���⿡ ��ĭ 2���� ��ȿ, �� ��ĭ�� 3�� �̻��̸� ��ȿ
	//1: 1 1 1 1	blank = 0

	unsigned char _FIRST_PER_Stack = 0;
	unsigned char _SECOND_PER_Stack = 0;

	unsigned char _FIRST_blankPerCase = 0;
	unsigned char _SECOND_blankPerCase = 0;

	unsigned char _check11StoneCount = 0;
	unsigned char _check2StoneCount = 0;

	bool check11Stone = FALSE;
	bool check2Stone = FALSE;

	bool ischeck1_FIRST_StoneAvailable = FALSE;
	bool ischeck2_FIRST_StoneAvailable = FALSE;

	bool ischeck1_LAST_StoneAvailable = FALSE;
	bool ischeck2_LAST_StoneAvailable = FALSE;

	int i;

	//case 1 �� ��
	for (i = 1; i <= 3 && col - i >= 0; ++i)
	{
		if (arr[row][col - i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row][col - i] == STONE) ischeck1_LAST_StoneAvailable = TRUE;
		if (arr[row][col - i] == STONE)
		{
			if (col - i - 1 < 0 || arr[row][col - i - 1] == ENEMY_STONE) return FALSE;
			ischeck1_FIRST_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!ischeck1_FIRST_StoneAvailable && arr[row][col - i] != ENEMY_STONE && arr[row][col - i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!ischeck1_FIRST_StoneAvailable) _FIRST_blankPerCase = 0;
	if (ischeck1_LAST_StoneAvailable && ischeck1_FIRST_StoneAvailable && _FIRST_blankPerCase == 0) ++_FIRST_blankPerCase;

	for (i = 1; i <= 3 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row][col + i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row][col + i] == STONE) ischeck2_LAST_StoneAvailable = TRUE;
		if (arr[row][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || arr[row][col + i + 1] == ENEMY_STONE) return FALSE;
			ischeck2_FIRST_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!ischeck2_FIRST_StoneAvailable && arr[row][col + i] != ENEMY_STONE && arr[row][col + i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!ischeck2_FIRST_StoneAvailable) _SECOND_blankPerCase = 0;
	if (ischeck2_LAST_StoneAvailable && ischeck2_FIRST_StoneAvailable && _SECOND_blankPerCase == 0) ++_SECOND_blankPerCase;

	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	if (_FIRST_PER_Stack == 1 && _SECOND_PER_Stack == 1 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_check11StoneCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		check11Stone = TRUE;
	}
	if (_FIRST_PER_Stack == 2 || _SECOND_PER_Stack == 2 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_check2StoneCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		check2Stone = TRUE;
	}
	ischeck1_LAST_StoneAvailable = ischeck2_LAST_StoneAvailable = ischeck1_FIRST_StoneAvailable = ischeck2_FIRST_StoneAvailable = 
		_FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;



	//case 2 �� ��
	for (i = 1; i <= 3 && row - i >= 0; ++i)
	{
		if (arr[row - i][col] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row - i][col] == STONE) ischeck1_LAST_StoneAvailable = TRUE;
		if (arr[row - i][col] == STONE)
		{
			if (row - i - 1 < 0 || arr[row - i - 1][col] == ENEMY_STONE) return FALSE;
			ischeck1_FIRST_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!ischeck1_FIRST_StoneAvailable && arr[row - i][col] != ENEMY_STONE && arr[row - i][col] != STONE) ++_FIRST_blankPerCase;
	}
	if (!ischeck1_FIRST_StoneAvailable) _FIRST_blankPerCase = 0;
	if (ischeck1_LAST_StoneAvailable && ischeck1_FIRST_StoneAvailable && _FIRST_blankPerCase == 0) ++_FIRST_blankPerCase;

	for (i = 1; i <= 3 && row + i <= MAX_ROW_OMOK_SIZE - 1; ++i)
	{
		if (arr[row + i][col] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row + i][col] == STONE) ischeck2_LAST_StoneAvailable = TRUE;
		if (arr[row + i][col] == STONE)
		{
			if (row + i + 1 >= MAX_ROW_OMOK_SIZE || arr[row + i + 1][col] == ENEMY_STONE) return FALSE;
			ischeck2_FIRST_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!ischeck2_FIRST_StoneAvailable && arr[row + i][col] != ENEMY_STONE && arr[row + i][col] != STONE) ++_SECOND_blankPerCase;
	}
	if (!ischeck2_FIRST_StoneAvailable) _SECOND_blankPerCase = 0;
	if (ischeck2_LAST_StoneAvailable && ischeck2_FIRST_StoneAvailable && _SECOND_blankPerCase == 0) ++_SECOND_blankPerCase;

	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	if (_FIRST_PER_Stack == 1 && _SECOND_PER_Stack == 1 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_check11StoneCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		check11Stone = TRUE;
	}
	if (_FIRST_PER_Stack == 2 || _SECOND_PER_Stack == 2 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_check2StoneCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		check2Stone = TRUE;
	}
	ischeck1_LAST_StoneAvailable = ischeck2_LAST_StoneAvailable = ischeck1_FIRST_StoneAvailable = ischeck2_FIRST_StoneAvailable = 
		_FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;



	//case 3 �آ�
	for (i = 1; i <= 3 && row - i >= 0 && col - i >= 0; ++i)
	{
		if (arr[row - i][col - i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row - i][col - i] == STONE) ischeck1_LAST_StoneAvailable = TRUE;
		if (arr[row - i][col - i] == STONE)
		{
			if (col - i - 1 < 0 || row - i - 1 < 0 || arr[row - i - 1][col - i - 1] == ENEMY_STONE) return FALSE;
			ischeck1_FIRST_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!ischeck1_FIRST_StoneAvailable && arr[row - i][col - i] != ENEMY_STONE && arr[row - i][col - i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!ischeck1_FIRST_StoneAvailable) _FIRST_blankPerCase = 0;
	if (ischeck1_LAST_StoneAvailable && ischeck1_FIRST_StoneAvailable && _FIRST_blankPerCase == 0) ++_FIRST_blankPerCase;

	for (i = 1; i <= 3 && row + i <= MAX_ROW_OMOK_SIZE - 1 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row + i][col + i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row + i][col + i] == STONE) ischeck2_LAST_StoneAvailable = TRUE;
		if (arr[row + i][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || row + i + 1 >= MAX_ROW_OMOK_SIZE || arr[row + i + 1][col + i + 1] == ENEMY_STONE) return FALSE;
			ischeck2_FIRST_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!ischeck2_FIRST_StoneAvailable && arr[row + i][col + i] != ENEMY_STONE && arr[row + i][col + i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!ischeck2_FIRST_StoneAvailable) _SECOND_blankPerCase = 0;
	if (ischeck2_LAST_StoneAvailable && ischeck2_FIRST_StoneAvailable && _SECOND_blankPerCase == 0) ++_SECOND_blankPerCase;

	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	if (_FIRST_PER_Stack == 1 && _SECOND_PER_Stack == 1 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_check11StoneCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		check11Stone = TRUE;
	}
	if (_FIRST_PER_Stack == 2 || _SECOND_PER_Stack == 2 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_check2StoneCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		check2Stone = TRUE;
	}
	ischeck1_LAST_StoneAvailable = ischeck2_LAST_StoneAvailable = ischeck1_FIRST_StoneAvailable = ischeck2_FIRST_StoneAvailable = 
		_FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;



	//case 4 �֢�
	for (i = 1; i <= 3 && row - i >= 0 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row - i][col + i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row - i][col + i] == STONE) ischeck1_LAST_StoneAvailable = TRUE;
		if (arr[row - i][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || row - i - 1 < 0 || arr[row - i - 1][col + i + 1] == ENEMY_STONE) return FALSE;
			ischeck1_FIRST_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!ischeck1_FIRST_StoneAvailable && arr[row - i][col + i] != ENEMY_STONE && arr[row - i][col + i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!ischeck1_FIRST_StoneAvailable) _FIRST_blankPerCase = 0;
	if (ischeck1_LAST_StoneAvailable && ischeck1_FIRST_StoneAvailable && _FIRST_blankPerCase == 0) ++_FIRST_blankPerCase;

	for (i = 1; i <= 3 && row + i <= MAX_ROW_OMOK_SIZE - 1 && col - i >= 0; ++i)
	{
		if (arr[row + i][col - i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row + i][col - i] == STONE) ischeck2_LAST_StoneAvailable = TRUE;
		if (arr[row + i][col - i] == STONE)
		{
			if (col - i - 1 < 0 || row + i + 1 >= MAX_ROW_OMOK_SIZE || arr[row + i + 1][col - i - 1] == ENEMY_STONE) return FALSE;
			ischeck2_FIRST_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!ischeck2_FIRST_StoneAvailable && arr[row + i][col - i] != ENEMY_STONE && arr[row + i][col - i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!ischeck2_FIRST_StoneAvailable) _SECOND_blankPerCase = 0;
	if (ischeck2_LAST_StoneAvailable && ischeck2_FIRST_StoneAvailable && _SECOND_blankPerCase == 0) ++_SECOND_blankPerCase;

	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;
	if (_FIRST_PER_Stack == 1 && _SECOND_PER_Stack == 1 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_check11StoneCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		check11Stone = TRUE;
	}
	if (_FIRST_PER_Stack == 2 || _SECOND_PER_Stack == 2 && _FIRST_blankPerCase + _SECOND_blankPerCase < 2)
	{
		_check2StoneCount = _FIRST_blankPerCase + _SECOND_blankPerCase;
		check2Stone = TRUE;
	}


	if (check11Stone && check2Stone && _check11StoneCount + _check2StoneCount < 4) return TRUE;
	return FALSE;
}

bool Check33_22(int row, int col, const char* STONE, char*** arr)
{
	//+4���� �������� �ݼ��� �ƴ�
	const char* ENEMY_STONE = STONE == "��" ? "��" : "��";

	//3: 2 2		blank = 2
	//2: 1 1 2		�� ���⿡ ��ĭ 2���� ��ȿ, �� ��ĭ�� 3�� �̻��̸� ��ȿ
	//1: 1 1 1 1	blank = 0

	unsigned char _FIRST_PER_Stack = 0;
	unsigned char _SECOND_PER_Stack = 0;

	unsigned char _FIRST_blankPerCase = 0;
	unsigned char _SECOND_blankPerCase = 0;

	unsigned char checkTotal = 0;
	unsigned char checkTotalBlank = 0;

	bool ischeck1_FIRST_StoneAvailable = FALSE;
	bool ischeck2_FIRST_StoneAvailable = FALSE;
	bool ischeck1_LAST_StoneAvailable = FALSE;
	bool ischeck2_LAST_StoneAvailable = FALSE;

	bool isTruePerCase[8] = { 0, };
	int blankCountPerCase[8] = { 0, };




	//case 1 �� ��
	for (int i = 1; i <= 3 && col - i >= 0; ++i)
	{
		if (arr[row][col - i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row][col - i] == STONE) ischeck1_LAST_StoneAvailable = TRUE;
		if (arr[row][col - i] == STONE)
		{
			if (col - i - 1 < 0 || arr[row][col - i - 1] == ENEMY_STONE) return FALSE;
			ischeck1_FIRST_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!ischeck1_FIRST_StoneAvailable && arr[row][col - i] != ENEMY_STONE && arr[row][col - i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!ischeck1_FIRST_StoneAvailable) _FIRST_blankPerCase = 0;
	if (ischeck1_LAST_StoneAvailable && ischeck1_FIRST_StoneAvailable && _FIRST_blankPerCase == 0) ++_FIRST_blankPerCase;
	isTruePerCase[0] = _FIRST_PER_Stack == 2 && _FIRST_blankPerCase < 2;
	blankCountPerCase[0] = _FIRST_blankPerCase;

	for (int i = 1; i <= 3 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row][col + i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row][col + i] == STONE) ischeck2_LAST_StoneAvailable = TRUE;
		if (arr[row][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || arr[row][col + i + 1] == ENEMY_STONE) return FALSE;
			ischeck2_FIRST_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!ischeck2_FIRST_StoneAvailable && arr[row][col + i] != ENEMY_STONE && arr[row][col + i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!ischeck2_FIRST_StoneAvailable) _SECOND_blankPerCase = 0;
	if (ischeck2_LAST_StoneAvailable && ischeck2_FIRST_StoneAvailable && _SECOND_blankPerCase == 0) ++_SECOND_blankPerCase;

	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;
	isTruePerCase[1] = _SECOND_PER_Stack == 2 && _SECOND_blankPerCase < 2;
	blankCountPerCase[1] = _SECOND_blankPerCase;

	ischeck1_LAST_StoneAvailable = ischeck2_LAST_StoneAvailable = ischeck1_FIRST_StoneAvailable = ischeck2_FIRST_StoneAvailable = 
		_FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;




	//case 2 �� ��
	for (int i = 1; i <= 3 && row - i >= 0; ++i)
	{
		if (arr[row - i][col] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row - i][col] == STONE) ischeck1_LAST_StoneAvailable = TRUE;
		if (arr[row - i][col] == STONE)
		{
			if (row - i - 1 < 0 || arr[row - i - 1][col] == ENEMY_STONE) return FALSE;
			ischeck1_FIRST_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!ischeck1_FIRST_StoneAvailable && arr[row - i][col] != ENEMY_STONE && arr[row - i][col] != STONE) ++_FIRST_blankPerCase;
	}
	if (!ischeck1_FIRST_StoneAvailable) _FIRST_blankPerCase = 0;
	if (ischeck1_LAST_StoneAvailable && ischeck1_FIRST_StoneAvailable && _FIRST_blankPerCase == 0) ++_FIRST_blankPerCase;
	isTruePerCase[2] = _FIRST_PER_Stack == 2 && _FIRST_blankPerCase < 2;
	blankCountPerCase[2] = _FIRST_blankPerCase;

	for (int i = 1; i <= 3 && row + i <= MAX_ROW_OMOK_SIZE - 1; ++i)
	{
		if (arr[row + i][col] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row + i][col] == STONE) ischeck2_LAST_StoneAvailable = TRUE;
		if (arr[row + i][col] == STONE)
		{
			if (row + i + 1 >= MAX_ROW_OMOK_SIZE || arr[row + i + 1][col] == ENEMY_STONE) return FALSE;
			ischeck2_FIRST_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!ischeck2_FIRST_StoneAvailable && arr[row + i][col] != ENEMY_STONE && arr[row + i][col] != STONE) ++_SECOND_blankPerCase;
	}
	if (!ischeck2_FIRST_StoneAvailable) _SECOND_blankPerCase = 0;
	if (ischeck2_LAST_StoneAvailable && ischeck2_FIRST_StoneAvailable && _SECOND_blankPerCase == 0) ++_SECOND_blankPerCase;
	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	isTruePerCase[3] = _SECOND_PER_Stack == 2 && _SECOND_blankPerCase < 2;
	blankCountPerCase[3] = _SECOND_blankPerCase;

	ischeck1_LAST_StoneAvailable = ischeck2_LAST_StoneAvailable = ischeck1_FIRST_StoneAvailable = ischeck2_FIRST_StoneAvailable = 
		_FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;




	//case 3 �آ�
	for (int i = 1; i <= 3 && row - i >= 0 && col - i >= 0; ++i)
	{
		if (arr[row - i][col - i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row - i][col - i] == STONE) ischeck1_LAST_StoneAvailable = TRUE;
		if (arr[row - i][col - i] == STONE)
		{
			if (col - i - 1 < 0 || row - i - 1 < 0 || arr[row - i - 1][col - i - 1] == ENEMY_STONE) return FALSE;
			ischeck1_FIRST_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!ischeck1_FIRST_StoneAvailable && arr[row - i][col - i] != ENEMY_STONE && arr[row - i][col - i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!ischeck1_FIRST_StoneAvailable) _FIRST_blankPerCase = 0;
	if (ischeck1_LAST_StoneAvailable && ischeck1_FIRST_StoneAvailable && _FIRST_blankPerCase == 0) ++_FIRST_blankPerCase;
	isTruePerCase[4] = _FIRST_PER_Stack == 2 && _FIRST_blankPerCase < 2;
	blankCountPerCase[4] = _FIRST_blankPerCase;

	for (int i = 1; i <= 3 && row + i <= MAX_ROW_OMOK_SIZE - 1 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row + i][col + i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row + i][col + i] == STONE) ischeck2_LAST_StoneAvailable = TRUE;
		if (arr[row + i][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || row + i + 1 >= MAX_COL_OMOK_SIZE || arr[row + i + 1][col + i + 1] == ENEMY_STONE) return FALSE;
			ischeck2_FIRST_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!ischeck2_FIRST_StoneAvailable && arr[row + i][col + i] != ENEMY_STONE && arr[row + i][col + i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!ischeck2_FIRST_StoneAvailable) _SECOND_blankPerCase = 0;
	if (ischeck2_LAST_StoneAvailable && ischeck2_FIRST_StoneAvailable && _SECOND_blankPerCase == 0) ++_SECOND_blankPerCase;
	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	isTruePerCase[5] = _SECOND_PER_Stack == 2 && _SECOND_blankPerCase < 2;
	blankCountPerCase[5] = _SECOND_blankPerCase;

	ischeck1_LAST_StoneAvailable = ischeck2_LAST_StoneAvailable = ischeck1_FIRST_StoneAvailable = ischeck2_FIRST_StoneAvailable = 
		_FIRST_PER_Stack = _SECOND_PER_Stack = _FIRST_blankPerCase = _SECOND_blankPerCase = 0;






	//case 4 �֢�
	for (int i = 1; i <= 3 && row - i >= 0 && col + i <= MAX_COL_OMOK_SIZE - 1; ++i)
	{
		if (arr[row - i][col + i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row - i][col + i] == STONE) ischeck1_LAST_StoneAvailable = TRUE;
		if (arr[row - i][col + i] == STONE)
		{
			if (col + i + 1 >= MAX_COL_OMOK_SIZE || row - i - 1 < 0 || arr[row - i - 1][col + i + 1] == ENEMY_STONE) return FALSE;
			ischeck1_FIRST_StoneAvailable = TRUE;
			++_FIRST_PER_Stack;
		}
		if (!ischeck1_FIRST_StoneAvailable && arr[row - i][col + i] != ENEMY_STONE && arr[row - i][col + i] != STONE) ++_FIRST_blankPerCase;
	}
	if (!ischeck1_FIRST_StoneAvailable) _FIRST_blankPerCase = 0;
	if (ischeck1_LAST_StoneAvailable && ischeck1_FIRST_StoneAvailable && _FIRST_blankPerCase == 0) ++_FIRST_blankPerCase;
	isTruePerCase[6] = _FIRST_PER_Stack == 2 && _FIRST_blankPerCase < 2;
	blankCountPerCase[6] = _FIRST_blankPerCase;

	for (int i = 1; i <= 3 && row + i <= MAX_ROW_OMOK_SIZE - 1 && col - i >= 0; ++i)
	{
		if (arr[row + i][col - i] == ENEMY_STONE) return FALSE;
		if (i == 3 && arr[row + i][col - i] == STONE) ischeck2_LAST_StoneAvailable = TRUE;
		if (arr[row + i][col - i] == STONE)
		{
			if (col - i - 1 < 0 || row + i + 1 >= MAX_COL_OMOK_SIZE || arr[row + i + 1][col - i - 1] == ENEMY_STONE) return FALSE;
			ischeck2_FIRST_StoneAvailable = TRUE;
			++_SECOND_PER_Stack;
		}
		if (!ischeck2_FIRST_StoneAvailable && arr[row + i][col - i] != ENEMY_STONE && arr[row + i][col - i] != STONE) ++_SECOND_blankPerCase;
	}
	if (!ischeck2_FIRST_StoneAvailable) _SECOND_blankPerCase = 0;
	if (ischeck2_LAST_StoneAvailable && ischeck2_FIRST_StoneAvailable && _SECOND_blankPerCase == 0) ++_SECOND_blankPerCase;
	if (_FIRST_PER_Stack + _SECOND_PER_Stack >= 3) return FALSE;

	isTruePerCase[7] = _SECOND_PER_Stack == 2 && _SECOND_blankPerCase < 2;
	blankCountPerCase[7] = _SECOND_blankPerCase;


	for (int i = 0; i < 8; ++i)
	{
		if (isTruePerCase[i])
		{
			++checkTotal;
			checkTotalBlank += blankCountPerCase[i];
		}
	}

	if (checkTotal >= 2 && checkTotalBlank < 3) return TRUE;
	return FALSE;

}

void AnnounceRule(int row, int col, char*** arr)
{
	textcolor(5, 0);
	gotoxy(MAX_COL_OMOK_SIZE, MAX_ROW_OMOK_SIZE + 3);
	printf("�ݼ�!!");

	Sleep(1000);
	gotoxy(MAX_COL_OMOK_SIZE, MAX_ROW_OMOK_SIZE + 3);
	printf("������");

	textcolor(15, 0);
}


bool CheckFinish(int row, int col, const char* STONE, char*** arr)
{
	//////////	�ش� �Լ��� ���������� Ž���ϴ� �Լ�			//////////
	//README//	�� 8������ ������ case���� 4���� ����			//README//
	//////////	case���� ȭ��ǥ�� üũ ������ ��Ÿ���� ����		//////////

	//STACK�� 5�� ������ (���ӵ� �� ������ 5�� �̻��̸�) TRUE�� �����Ͽ� ���� ����
	unsigned char OMOK_Stack = 1;


	//�� case���� STACK��ġ�� �ʱ�ȭ����
	OMOK_Stack = 1;
	//���� 1: �ִ� 4�� �ݺ�(�� ���⿡ 4������ ���̸� ���� 5���� �����̱⶧��)
	//���� 2: �˻� ������ ��ǥ�� �������� ũ�⸦ ����� FALSE�� �Ͽ� �ش� �ݺ��� ����
	//���� 3: �˻��ϴ� ��ǥ���� �ش� ���� ���̸� TRUE�̸鼭 ī��Ʈ�� ������Ų��.
	//���� 2�� ���� 3���� ���� ��ġ�Ͽ� �޸� ���� ������ ������ ���ش�.(���� �ݴ�� �� ��� �޸����� ������)
	//case 1 �� ��
	for (int i = 1; i <= 4 && col - i >= 0 && arr[row][col - i] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	for (int i = 1; i <= 4 && col + i <= MAX_COL_OMOK_SIZE - 1 && arr[row][col + i] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	if (OMOK_Stack >= 5) return TRUE;


	//case 2 �� ��
	OMOK_Stack = 1;
	for (int i = 1; i <= 4 && row - i >= 0 && arr[row - i][col] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	for (int i = 1; i <= 4 && row + i <= MAX_ROW_OMOK_SIZE - 1 && arr[row + i][col] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	if (OMOK_Stack >= 5) return TRUE;


	//case 3 �آ�
	OMOK_Stack = 1;
	for (int i = 1; i <= 4 && row - i >= 0 && col - i >= 0 && arr[row - i][col - i] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	for (int i = 1; i <= 4 && row + i <= MAX_ROW_OMOK_SIZE - 1 && col + i <= MAX_COL_OMOK_SIZE - 1 && arr[row + i][col + i] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	if (OMOK_Stack >= 5) return TRUE;


	//case 4 �֢�
	OMOK_Stack = 1;
	for (int i = 1; i <= 4 && row - i >= 0 && col + i <= MAX_COL_OMOK_SIZE - 1 && arr[row - i][col + i] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	for (int i = 1; i <= 4 && row + i <= MAX_ROW_OMOK_SIZE - 1 && col - i >= 0 && arr[row + i][col - i] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	if (OMOK_Stack >= 5) return TRUE;

	return FALSE;
}

void InitOmok(char*** arr)
{
	CursorView(NULL);

	//2�� for���� ����ؼ� �� ��ǥ�� �˸´� ��ȣ�� �Ҵ�
	for (int i = 0; i < MAX_ROW_OMOK_SIZE; ++i)
	{
		for (int j = 0; j < MAX_COL_OMOK_SIZE; ++j)
		{
			if (i == 0 && j == 0) {
				arr[i][j] = (char*)"��";
			}
			else if (i == 0 && j != MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"��";
			}
			else if (i == 0 && j == MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"��";
			}
			else if (i != MAX_ROW_OMOK_SIZE - 1 && j == 0) {
				arr[i][j] = (char*)"��";
			}
			else if (i != MAX_ROW_OMOK_SIZE - 1 && j != MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"��";
			}
			else if (i != MAX_ROW_OMOK_SIZE - 1 && j == MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"��";
			}
			else if (i == MAX_ROW_OMOK_SIZE - 1 && j == 0) {
				arr[i][j] = (char*)"��";
			}
			else if (i == MAX_ROW_OMOK_SIZE - 1 && j != MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"��";
			}
			else {
				arr[i][j] = (char*)"��";
			}
		}
	}
}

void PaintGame(char*** arr)
{
	//Let's Play Omok
	if (MAX_COL_OMOK_SIZE < 6)
	{
		gotoxy(MAX_COL_OMOK_SIZE - 4, 0);
		printf("Let's Play");
		gotoxy(2, 1);
		printf("Omok!!\n   ");
	}
	else if (MAX_COL_OMOK_SIZE <= 14)
	{
		gotoxy(MAX_COL_OMOK_SIZE - 6, 0);
		printf("= Let's Play Omok =\n\n   ");
	}
	else
	{
		gotoxy(MAX_COL_OMOK_SIZE - 12, 0);
		printf("====== Let's Play Omok ======\n\n   ");
	}

	//������ �� ��ȣ ���
	for (int i = 0; i < MAX_COL_OMOK_SIZE; ++i)
	{
		printf("%2d", 1 + i);
	}

	//�ʱ�ȭ�� arr(������)�� �� ��ȣ ���
	printf("\n");
	for (int i = 0; i < MAX_ROW_OMOK_SIZE; ++i)
	{
		printf("%2d ", 1 + i);
		for (int j = 0; j < MAX_COL_OMOK_SIZE; ++j)
		{
			if(arr[i][j] == "��" || arr[i][j] == "��")
				printf("%s", arr[i][j]);
			else
				printf("%s ", arr[i][j]);
		}
		printf("\n");
	}
}

void RunGame(char*** arr, int& row, int& col, bool& isBlackTurn)
{
	//����Ű, �����Է� ����� toggle�� flag
	bool isArrowMove = TRUE;

	//�����Է� ��Ŀ��� ���� �Է°��� �����ϴ� charBuffer
	vector<char> inputCharBuff;

	//charBuffer�� Ž���� iterator
	vector<char>::iterator buffIt;

	//������ ������ ���� ���� �ݺ�
	while (TRUE)
	{
		//����Ű ����ϰ��
		if (isArrowMove)
		{
			//����Ű ��Ŀ��� [Tt]oggle�� ������ ���� Aim�� ���ְ� isArrowMove flag���� ������Ű�� �����Է� ������� ��ȯ
			if (_kbhit())
			{
				switch(_getch())
				{
				case 'T': 
					[[fallthrough]];
				case 't':
					isArrowMove = false;
					gotoxy(3 + col * 2, row + 3);
					textcolor(15, 0);
					if (arr[row][col] == "��" || arr[row][col] == "��")
						printf("%s", arr[row][col]);
					else printf("%s ", arr[row][col]);

					system("cls");
					PaintGame(arr);
					
					//charBuffer�� ���� �����ش�
					inputCharBuff.clear();

					continue;
				}
			}
			//SPACE BAR�� ������ ������� ���´�
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				//�ش� ��ǥ�� �̹� ������� �ִ� ��� ����
				if (arr[row][col] == "��" || arr[row][col] == "��") {}
				else//�ƴϸ�
				{
					//������ �� �����ϰ��
					if (isBlackTurn)
					{
						if (Check33_1111(row, col, "��", arr) || Check33_112(row, col, "��", arr) || Check33_22(row, col, "��", arr))
						{
							AnnounceRule(row, col, arr);
							continue;
						}
						//�ش� ��ǥ�� ���� ���� ��ü
						arr[row][col] = (char*)"��";
						//���� ���ǿ� �ش��ϴ��� üũ.. TRUE�ϰ�� return�Ͽ� ���� ����
						if (CheckFinish(row, col, "��", arr)) return;

						//BlackTurn�� FALSE�� �Ͽ� ��� ���� �ǹ�
						isBlackTurn = FALSE;
					}
					else
					{
						arr[row][col] = (char*)"��";
						if (CheckFinish(row, col, "��", arr)) return;
						isBlackTurn = TRUE;
					}
				}
			}

			//���� AIM�� ��ǥ�� Ŀ���� �̵��ϰ� ������ ����(����)���� �����ش�
			//������ AIM�� �����϶����� AIM���� ��� ��ǥ�� ������ �ʱ�ȭ �������
			gotoxy(3 + col * 2, row + 3);

			//�ش� if���� �׷��ȱ�ȣ(��  �� ��)���� ũ�� ������ ����:���� = 1:2 �̱⶧���� ����� �� ��ĭ�� �������Ѵ�.
			//������ ������� ������ 1:1�̱� ������ ��ĭ�� ������� �ʴ´�.
			if (arr[row][col] == "��" || arr[row][col] == "��")
				printf("%s", arr[row][col]);
			else printf("%s ", arr[row][col]);

			//����Ű �Է¿� ���� ��� ���� ��ġ�� �ٲ��ش�. elseif�� �ǵ��� ��.
			if (GetAsyncKeyState(VK_UP) & 0x8000)			--row;	//��
			else if (GetAsyncKeyState(VK_DOWN) & 0x8000)	++row;	//��
			else if (GetAsyncKeyState(VK_LEFT) & 0x8000)	--col;	//��
			else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	++col;	//��

			//��� ���� ��ǥ�� �������� ����� �޸� ���� ������ �߻��ϱ� ������ �ٷ�����ش�.
			if (row < 0) row = 0;
			if (row > MAX_ROW_OMOK_SIZE - 1) row = MAX_ROW_OMOK_SIZE - 1;
			if (col < 0) col = 0;
			if (col > MAX_COL_OMOK_SIZE - 1) col = MAX_COL_OMOK_SIZE - 1;

			//����Ű �Է¿� ���� Ŀ���� AIM���� �ű�
			gotoxy(3 + col * 2, row + 3);

			//���� �Ͽ� ���� ������ ����
			if (isBlackTurn)	textcolor(15, 2);
			else				textcolor(15, 4);

			//�ش� AIM�� �ٵϾ��� ������� ��ĭ ���⸦ ���� ���� (����� ��������, ���� ���� ����)
			if (arr[row][col] == "��" || arr[row][col] == "��")	printf("%s", arr[row][col]);
			else												printf("%s ", arr[row][col]);

			//AIM�� ������ ���� ������ ������
			textcolor(15, 0);

			//���� ��ǥ�� ��ġȭ�Ͽ� ����ϱ�����
			gotoxy(0, MAX_ROW_OMOK_SIZE + 4);
			if (isBlackTurn) printf("��'s X,Y: %2d %2d", col + 1, row + 1);
			else			printf("��'s X,Y: %2d %2d", col + 1, row + 1);

			//Sleep�� �Ͽ� while���� �ݺ��� �ٿ� ��� ���ҽ��� ���ҽ�Ű�� Ű �Է��� �ݺ��� ������Ų��
			Sleep(100);
		}	
		else
		{
			//system clear, paintGame ���� �� �Է�â ǥ��
			gotoxy(0, MAX_ROW_OMOK_SIZE + 4);
			if (isBlackTurn) printf("��'s X,Y:  ");
			else			 printf("��'s X,Y:  ");

			//�� �ڵ��� �Է�â ǥ�� �Ŀ� ���ݱ��� charBuffer���Է��� ���� ���
			for (buffIt = inputCharBuff.begin(); buffIt != inputCharBuff.end(); ++buffIt)
				printf("%c", *buffIt);

			//�迭�� �����ּҴ� 0���� ����, �Է°��� -1�� �̸� ����
			int row = -1, col = -1;

			//charBuffer�� ��ĭ�� ������ �����ϴ� ����
			int blankCount = 0;

			//charBuffer�� ��ĭ�� ������ ��
			for (buffIt = inputCharBuff.begin(); buffIt != inputCharBuff.end(); ++buffIt)
			{
				if (*buffIt == ' ') ++blankCount;
			}

			//�Է� ���´� "5 4" ó�� �� �� ���̿� ���Ⱑ �ϳ��� �ֱ⿡ 1���϶��� ����
			if (blankCount == 1)
			{
				//sizeCap�� char�� int������ �ٲ� �� �ڸ����� ������ ���ֱ� ���� ����
				int sizeCap;
				//charBuffer�� �� �ڿ������� Ž���ϸ� �ش� �ڸ����� ���� * sizeCap^n��ŭ �ڸ����� �����Ͽ� row, col�� �����ش�
				for (sizeCap = 0, buffIt = inputCharBuff.end() - 1; *buffIt != ' '; --buffIt)
					row += (*buffIt - '0') * (int)pow(10, sizeCap++);

				//col�� ���������� char�� int������ ��ȯ�Ѵ�
				for (--buffIt, sizeCap = 0; /*NULL*/ ; --buffIt)
				{
					col += (*buffIt - '0') * (int)pow(10, sizeCap++);
					if (buffIt == inputCharBuff.begin()) break;
				}
			}

			//charBuffer�� �Է°��� ���� �� �ϰ��, �ش� ��ġ�� Aim ǥ��
			if (row >= 0 && col >= 0 && row < MAX_ROW_OMOK_SIZE && col < MAX_COL_OMOK_SIZE)
			{	
				//��ġ�� ǥ���� Aim�� ��ġ�� �����ϰ� �Ͽ����� ���� ����
				gotoxy(3 + col * 2, row + 3);
				if (isBlackTurn)	textcolor(15, 2);
				else				textcolor(15, 4);

				//�ش� AIM�� �ٵϾ��� ������� ��ĭ ���⸦ ���� ���� (����� ��������, ���� ���� ����)
				if (arr[row][col] == "��" || arr[row][col] == "��")	printf("%s", arr[row][col]);
				else												printf("%s ", arr[row][col]);

				//AIM�� ������ ���� ������ ������
				textcolor(15, 0);

			}

			//Ű���� �Է��� ���� �� ���� �� ��� ����
			//scanf�� cin���� �Է��� ������ ���๮�� �Է±��� �ý����� pause�ϱ⶧����
			//kbhit�� getch�� �Լ��� �ǽð����� �Է¶����� �̿� ���� ������ �ϱ� ���ؼ� ����
			//�ǽð����� Aimǥ��, TŰ�� ���� Toggle���� ��� �������� ��� �߰� ����
			if (_kbhit())
			{
				//ȭ�� �ʱ�ȭ
				system("cls");
				PaintGame(arr);

				//�Է��Ҷ����� charBuffer�� ũ�⿡ Ŀ���� ��ġ��Ű�� �ڿ������� �Է��� ����
				gotoxy(10 + inputCharBuff.size() , MAX_ROW_OMOK_SIZE + 4);

				//kbhit�Լ����� ���ۿ� ����ִ� char�� �������� �� ��� ����
				switch(_getch())
				{
				
				//�Է°��� [Tt] �ϰ�� isArrowMove�� true��Ű�� continue
				case 'T':
					[[fallthrough]];
				case 't':

					system("cls");
					PaintGame(arr);

					row = MAX_ROW_OMOK_SIZE / 2;
					col = MAX_COL_OMOK_SIZE / 2;

					isArrowMove = true;

					continue;

				//�Է°��� [0~9] (����)�ϰ�� charBuffer�� push�Ѵ�
				case '0':
					inputCharBuff.push_back('0');
					continue;
				case '1':
					inputCharBuff.push_back('1');
					continue;
				case '2':
					inputCharBuff.push_back('2');
					continue;
				case '3':
					inputCharBuff.push_back('3');
					continue;
				case '4':
					inputCharBuff.push_back('4');
					continue;
				case '5':
					inputCharBuff.push_back('5');
					continue;
				case '6':
					inputCharBuff.push_back('6');
					continue;
				case '7':
					inputCharBuff.push_back('7');
					continue;
				case '8':
					inputCharBuff.push_back('8');
					continue;
				case '9':
					inputCharBuff.push_back('9');
					continue;

				//Backspace(�����)�ϰ�� �� �� ��Ҹ� pop�ϸ� ����ִ°��� ���õȴ�
				case 8:		//BACKSPACE
					if(!inputCharBuff.empty())
					inputCharBuff.pop_back();

					continue;

				//���� �ϰ�� ' '(blank)�� �߰��ϸ� ����ִ°�쿡�� ��ĭ�� �߰����� �ʴ´�
				case 32:	//SPACEBAR
					if (inputCharBuff.empty()) continue;
					inputCharBuff.push_back(' ');
					continue;

				//�����Է½� �ش� ��ġ�� ������� �д� ������ ��İ� ����
				case 13:	//ENTER
					{
					//99,99 �Է½� ����
					if (row == 98 && col == 98) return;
						inputCharBuff.clear();
						
						//�Է°��� �������� ũ�⸦ ����°�� continue, ��ġ ������
						if (row < 0 || col < 0 || row >= MAX_ROW_OMOK_SIZE || col >= MAX_COL_OMOK_SIZE)
						{
							row = MAX_ROW_OMOK_SIZE / 2;
							col = MAX_COL_OMOK_SIZE / 2;
							continue;
						}
						
						//�̹� ���� �ִ°�� �Ѿ
						if (arr[row][col] == "��" || arr[row][col] == "��") {}
						//�ƴϸ�
						else
						{
							//������ �� �����ϰ��
							if (isBlackTurn)
							{
								if (Check33_1111(row, col, "��", arr) || Check33_112(row, col, "��", arr) || Check33_22(row, col, "��", arr))
								{
									AnnounceRule(row, col, arr);
									continue;
								}
								//�ش� ��ǥ�� ���� ���� ��ü
								arr[row][col] = (char*)"��";
								//���� ���ǿ� �ش��ϴ��� üũ.. TRUE�ϰ�� return�Ͽ� ���� ����
								if (CheckFinish(row, col, "��", arr)) return;

								//BlackTurn�� FALSE�� �Ͽ� ��� ���� �ǹ�
								isBlackTurn = FALSE;
							}
							else
							{
								//�� �� �����ΰ�� �� ���� ���´�
								arr[row][col] = (char*)"��";
								if (CheckFinish(row, col, "��", arr)) return;
								isBlackTurn = TRUE;
							}

							//ȭ�� �ʱ�ȭ
							system("cls");
							PaintGame(arr);
						}
					}
				}
			}
		}
	}
}

void ReleaseMemory(char*** arr)
{
	//2���� �迭 ��ȯ
	for (int i = 0; i < MAX_ROW_OMOK_SIZE; ++i)
		delete[] arr[i];
	delete arr;
}