////////////////////////////////////////////////////////////////
//
// 프로그램 작성일:	2021.03.26
// 최근 수정일:		2021.04.02
// 작성자:			박승근 20180952
// 목적:				오목 게임
// 
// MEMO //	
// 19*19 바둑판 규격이나, 15*15 오목판 규격 모두 사용합니다.
// 흑돌은 3*3 금수 처리함.
// 4*4는 흑, 백 모두 둘 수 있는 형태임.
// 장목(6목이상) 흑, 백 모두 둘 수는 있되, 승패에 영향은 없음.
////////////////////////////////////////////////////////////////


//입출력 스트림 지원
#include <iostream>
//key값, gotoxy등 여러가지 기능 지원
#include <Windows.h>

//세로 크기(행)
constexpr int MAX_ROW_OMOK_SIZE = 10;
//가로 크기(열)
constexpr int MAX_COL_OMOK_SIZE = 10;

//gotoxy
void gotoxy(int x, int y)
{
	//pos struct는 x와 y(단순히 숫자)의 값을 저장하는것.
	//gotoxy의 x,y 인자를 이용하여 pos객체를 임시로 생성하고
	//SetConsoleCursorPosition함수를 이용하여 커서를 이동시킴
	COORD pos = { (SHORT)x,(SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//textcolor
void textcolor(int foreground, int background)
{
	//color은 단순히 색상코드를 비트화시켜서 인자 한개로 변환시켜준다.
	//*16을 사용하여 비트를 이동시킴 (^4)
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//프로그램 실행 시 오목판 배열을 형식에 맞게 할당해줌
void InitOmok(char*** arr);

//게임 시작 시 오목판과 다른 요소들 그리기
void PaintGame(char*** arr);

//For TEST
bool Check33_1111(int row, int col, const char* STONE, char*** arr);
bool Check33_112(int row, int col, const char* STONE, char*** arr);
bool Check33_22(int row, int col, const char* STONE, char*** arr);
void AnnounceRule(int row, int col, char*** arr);

//돌을 찍은 기점에서 4방향*2 체크 [인자: 행, 열, 돌모양, 오목판 배열]
bool CheckFinish(int row, int col, const char* STONE, char*** arr);

//게임을 실행함
void RunGame(char*** arr, char& row, char& col, bool& isBlackTurn);

//할당된 메모리를 반환
void ReleaseMemory(char*** arr);

int main()
{
	//흑돌, 백돌이 순서대로 돌아가게 boolean형 선언, 흑돌이 게임 먼저 시작
	bool isBlackTurn = TRUE;

	//현재 커서가 위치한 곳의 좌표를 나타냄, 행과 열
	char row, col;

	//게임 시작은 오목판 크기의 중심에서 시작
	row = MAX_ROW_OMOK_SIZE / 2;
	col = MAX_COL_OMOK_SIZE / 2;

	//2차원 포인터 char형을 동적할당으로 선언
	char*** arr = new char** [MAX_ROW_OMOK_SIZE];
	for (int i = 0; i < MAX_ROW_OMOK_SIZE; ++i)
		arr[i] = new char* [MAX_COL_OMOK_SIZE];

	InitOmok(arr);
	PaintGame(arr);
	RunGame(arr, row, col, isBlackTurn);
	ReleaseMemory(arr);

	return 0;
}

bool Check33_1111(int row, int col, const char* STONE, char*** arr)
{
	//+4까지 막혀있음 금수가 아님
	const char* ENEMY_STONE = STONE == "●" ? "○" : "●";

	//3: 2 2		blank = 2
	//2: 1 1 2		한 방향에 빈칸 2개면 무효, 총 빈칸이 3개 이상이면 무효
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



	//case 1 ← →
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



	//case 2 ↑ ↓
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



	//case 3 ↖↘
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



	//case 4 ↗↙
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
	//+4까지 막혀있음 금수가 아님
	const char* ENEMY_STONE = STONE == "●" ? "○" : "●";

	//3: 2 2		blank = 2
	//2: 1 1 2		한 방향에 빈칸 2개면 무효, 총 빈칸이 3개 이상이면 무효
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

	//case 1 ← →
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



	//case 2 ↑ ↓
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



	//case 3 ↖↘
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



	//case 4 ↗↙
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
	//+4까지 막혀있음 금수가 아님
	const char* ENEMY_STONE = STONE == "●" ? "○" : "●";

	//3: 2 2		blank = 2
	//2: 1 1 2		한 방향에 빈칸 2개면 무효, 총 빈칸이 3개 이상이면 무효
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




	//case 1 ← →
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




	//case 2 ↑ ↓
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




	//case 3 ↖↘
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






	//case 4 ↗↙
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
	printf("금수!!");

	Sleep(1000);
	gotoxy(MAX_COL_OMOK_SIZE, MAX_ROW_OMOK_SIZE + 3);
	printf("　　　");

	textcolor(15, 0);
}


bool CheckFinish(int row, int col, const char* STONE, char*** arr)
{
	//////////	해당 함수는 종료조건을 탐색하는 함수			//////////
	//README//	총 8가지의 방향을 case별로 4개로 나눔			//README//
	//////////	case마다 화살표로 체크 방향을 나타내고 있음		//////////

	//STACK이 5를 넘으면 (연속된 돌 개수가 5개 이상이면) TRUE를 리턴하여 게임 종료
	unsigned char OMOK_Stack = 1;


	//각 case마다 STACK수치를 초기화해줌
	OMOK_Stack = 1;
	//조건 1: 최대 4번 반복(한 방향에 4번까지 참이면 돌이 5개가 연속이기때문)
	//조건 2: 검사 방향의 좌표가 오목판의 크기를 벗어나면 FALSE를 하여 해당 반복문 종료
	//조건 3: 검사하는 좌표값이 해당 턴의 돌이면 TRUE이면서 카운트를 증가시킨다.
	//조건 2가 조건 3보다 먼저 위치하여 메모리 참조 오류의 위험을 없앤다.(순서 반대로 할 경우 메모리참조 오류남)
	//case 1 ← →
	for (int i = 1; i <= 4 && col - i >= 0 && arr[row][col - i] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	for (int i = 1; i <= 4 && col + i <= MAX_COL_OMOK_SIZE - 1 && arr[row][col + i] == STONE; ++i)
	{
		++OMOK_Stack;
	}
	if (OMOK_Stack >= 5) return TRUE;


	//case 2 ↑ ↓
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


	//case 3 ↖↘
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


	//case 4 ↗↙
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
	//2중 for문을 사용해서 각 좌표에 알맞는 기호를 할당
	for (int i = 0; i < MAX_ROW_OMOK_SIZE; ++i)
	{
		for (int j = 0; j < MAX_COL_OMOK_SIZE; ++j)
		{
			if (i == 0 && j == 0) {
				arr[i][j] = (char*)"┌";
			}
			else if (i == 0 && j != MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"┬";
			}
			else if (i == 0 && j == MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"┐";
			}
			else if (i != MAX_ROW_OMOK_SIZE - 1 && j == 0) {
				arr[i][j] = (char*)"├";
			}
			else if (i != MAX_ROW_OMOK_SIZE - 1 && j != MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"┼";
			}
			else if (i != MAX_ROW_OMOK_SIZE - 1 && j == MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"┤";
			}
			else if (i == MAX_ROW_OMOK_SIZE - 1 && j == 0) {
				arr[i][j] = (char*)"└";
			}
			else if (i == MAX_ROW_OMOK_SIZE - 1 && j != MAX_COL_OMOK_SIZE - 1) {
				arr[i][j] = (char*)"┴";
			}
			else {
				arr[i][j] = (char*)"┘";
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

	//오목판 열 번호 출력
	for (int i = 0; i < MAX_COL_OMOK_SIZE; ++i)
	{
		printf("%2d", 1 + i);
	}

	//초기화된 arr(오목판)과 행 번호 출력
	printf("\n");
	for (int i = 0; i < MAX_ROW_OMOK_SIZE; ++i)
	{
		printf("%2d ", 1 + i);
		for (int j = 0; j < MAX_COL_OMOK_SIZE; ++j)
		{
			printf("%s ", arr[i][j]);
		}
		printf("\n");
	}
}

void RunGame(char*** arr, char& row, char& col, bool& isBlackTurn)
{
	//게임이 끝날때 까지 무한 반복
	while (TRUE)
	{
		//SPACE BAR를 누르면 오목알을 놓는다
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			//해당 좌표에 이미 오목알이 있는 경우 무시
			if (arr[row][col] == "○" || arr[row][col] == "●") {}
			else//아니면
			{
				//검은색 돌 차례일경우
				if (isBlackTurn)
				{
					if (Check33_1111(row, col, "○", arr) || Check33_112(row, col, "○", arr) || Check33_22(row, col, "○", arr))
					{
						AnnounceRule(row, col, arr);
						continue;
					}
					//해당 좌표를 검은 돌로 교체
					arr[row][col] = (char*)"○";
					//종료 조건에 해당하는지 체크.. TRUE일경우 return하여 게임 종료
					if (CheckFinish(row, col, "○", arr)) return;

					//BlackTurn을 FALSE로 하여 흰색 턴을 의미
					isBlackTurn = FALSE;
				}
				else
				{
					arr[row][col] = (char*)"●";
					if (CheckFinish(row, col, "●", arr)) return;
					isBlackTurn = TRUE;
				}
			}
		}

		
		//현재 AIM의 좌표로 커서를 이동하고 배경색이 없는(검정)으로 덮어준다
		//이유는 AIM이 움직일때마다 AIM에서 벗어난 좌표의 배경색을 초기화 해줘야함
		gotoxy(3 + col * 2, row + 3);

		//해당 if문은 그래픽기호(┌  ┐ ┘)들의 크기 비율이 가로:세로 = 1:2 이기때문에 출력할 때 한칸씩 띄워줘야한다.
		//하지만 오목알은 비율이 1:1이기 때문에 한칸을 띄워주지 않는다.
		if (arr[row][col] == "○" || arr[row][col] == "●")
			printf("%s", arr[row][col]);
		else printf("%s ", arr[row][col]);
		

		//방향키 입력에 따라 행과 열의 위치를 바꿔준다. elseif는 의도한 것.
		if (GetAsyncKeyState(VK_UP) & 0x8000)			--row;	//↑
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)	++row;	//↓
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000)	--col;	//←
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	++col;	//→

		//행과 열의 좌표가 오목판을 벗어나면 메모리 참조 오류가 발생하기 때문에 바로잡아준다.
		if (row < 0) row = 0;
		if (row > MAX_ROW_OMOK_SIZE - 1) row = MAX_ROW_OMOK_SIZE - 1;
		if (col < 0) col = 0;
		if (col > MAX_COL_OMOK_SIZE - 1) col = MAX_COL_OMOK_SIZE - 1;

		//방향키 입력에 따른 커서를 AIM으로 옮김
		gotoxy(3 + col * 2, row + 3);

		//현재 턴에 따라 배경색을 변경
		if (isBlackTurn)	textcolor(15, 2);
		else				textcolor(15, 4);

		//해당 AIM에 바둑알이 있을경우 한칸 띄우기를 하지 않음 (오목알 비율때문, 위에 설명 있음)
		if (arr[row][col] == "○" || arr[row][col] == "●")	printf("%s", arr[row][col]);
		else												printf("%s ", arr[row][col]);

		//AIM을 제외한 곳은 배경색이 검정임
		textcolor(15, 0);


		//현재 좌표를 수치화하여 출력하기위함
		gotoxy(0, MAX_ROW_OMOK_SIZE + 4);
		if (isBlackTurn) printf("○'s X,Y: %2d %2d", row + 1, col + 1);
		else			printf("●'s X,Y: %2d %2d", row + 1, col + 1);


		//Sleep를 하여 while문의 반복을 줄여 사용 리소스를 감소시키고 키 입력의 반복을 억제시킨다
		Sleep(100);
	}
}

void ReleaseMemory(char*** arr)
{
	//2차원 배열 반환
	for (int i = 0; i < MAX_ROW_OMOK_SIZE; ++i)
		delete[] arr[i];
	delete arr;
}