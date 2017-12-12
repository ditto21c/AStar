/*
	http://www.gamecodi.com/board/zboard-id-GAMECODI_Talkdev-no-3763-z-12.htm

	** Cellular Automata
*/

#include <iostream>
#include <list>
#include <Windows.h>
#include "AStar.cpp"

using namespace std;

void CreateTile(bool* Tile);
void UpdateTile(bool* Tile);
void ShowTile(bool* Tile);

int Size;
int TileCount;

void main()
{
	cout << "크기를 입력해주세요 : " << endl;
	cin >> Size;

	TileCount = Size * Size;

	bool* Tile = new bool[TileCount];

	CreateTile(Tile);

	cout << "기본 모양" << endl;
	ShowTile(Tile);

	int Temp;
	while (true)
	{
		cout << "갱신(1) 리셋(2) 시드변경(3) 종료(4) AStar(5)";
		cin >> Temp;
		if (1 == Temp)
		{
			UpdateTile(Tile);
			ShowTile(Tile);
		}
		else if (2 == Temp)
		{
			cout << "크기를 입력해주세요 : " << endl;
			cin >> Size;

			CreateTile(Tile);
			ShowTile(Tile);
		}
		else if (3 == Temp)
		{
			srand(GetTickCount());
		}
		else if (4 == Temp)
		{
			break;
		}
		else if (5 == Temp)
		{
			CAStar AStar;
			AStar.StartAStar(Tile, Size);
		}
	}
}

void CreateTile(bool* Tile)
{
	memset(Tile, 0, sizeof(bool)*TileCount);

	list<int> TempList;
	int BlockCount = (int)((float)TileCount * 0.45f);
	int RanBlock = 0;
	bool bAdd;
	for (int i = 0; i < BlockCount; ++i)
	{
		bAdd = false;
		do
		{
			RanBlock = rand() % TileCount;
			if (TempList.end() == std::find(TempList.begin(), TempList.end(), RanBlock))
			{
				TempList.push_back(RanBlock);
				bAdd = true;
			}
		} while (!bAdd);
	}

	for (auto iter = TempList.begin(); iter != TempList.end(); ++iter)
	{
		Tile[*iter] = true;
	}
}

/*
(i - Size - 1);
(i - Size);
(i - Size + 1);

(i - 1);
(i);
(i + 1);

(i + Size - 1);
(i + Size);
(i + Size + 1);
*/
void UpdateTile(bool* Tile)
{
	int BlockCount;
	int Temp;
	for (int i = 0; i < TileCount; ++i)
	{
		BlockCount = 0;
		for (int j = -1; j < 2; ++j)
		{
			for (int k = -1; k < 2; ++k)
			{
				Temp = i + Size*j + k;
				if (0 <= Temp && Temp < TileCount)
				{
					if (Tile[Temp])
						++BlockCount;
				}
				else
				{
					++BlockCount;
				}
			}
		}

		if (4 < BlockCount)
			Tile[i] = true;
		else
			Tile[i] = false;
	}
}

void ShowTile(bool* Tile)
{
	for (int i = 0; i < Size; ++i)
	{
		for (int j = 0; j < Size; ++j)
		{
			if (Tile[i*Size + j])
				cout << "#";
			else
				cout << "*";
		}
		cout << endl;
	}
}