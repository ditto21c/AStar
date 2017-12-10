#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Tile
{
	float Fitness = 0;
	float Goal = 0;
	float Heuristic = 0;
	float PosX = 0;
	float PosY = 0;
	bool bBlock = false;
	int ParentTile = 0;
};

vector<Tile> Tiles;
vector<int> OpenList;
vector<int> ClosedList;
vector<int> Paths;
int g_StartPos = 0, g_EndPos = 0;
int TileSize = 10;

float CalcuGoal(int StandardPos, int OpenPos)
{
	float X = abs(Tiles[StandardPos].PosX - Tiles[OpenPos].PosX);
	float Y = abs(Tiles[StandardPos].PosY - Tiles[OpenPos].PosY);
	
	float Dist = sqrt(X*X + Y*Y);
	return Tiles[StandardPos].Fitness + Dist;
}

float CalcuHeuristic(int OpenPos, int EndPos)
{
	return abs(Tiles[EndPos].PosX - Tiles[OpenPos].PosX) + abs(Tiles[EndPos].PosY - Tiles[OpenPos].PosY);
}

vector<int> PushOpenList(int StandardPos, int OpenPos)
{
	vector<int> PushedOpenList;
	vector<int>::iterator findClose = std::find(ClosedList.begin(), ClosedList.end(), OpenPos);
	if (findClose == ClosedList.end())
	{
		vector<int>::iterator findOpen = std::find(OpenList.begin(), OpenList.end(), OpenPos);
		if (findOpen == OpenList.end())
		{

			OpenList.push_back(OpenPos);
			Tiles[OpenPos].Goal = CalcuGoal(StandardPos, OpenPos);
			Tiles[OpenPos].Heuristic = CalcuHeuristic(OpenPos, g_EndPos);
			Tiles[OpenPos].Fitness = Tiles[OpenPos].Goal + Tiles[OpenPos].Heuristic;
			Tiles[OpenPos].ParentTile = StandardPos;
			PushedOpenList.push_back(OpenPos);
		}
	}

	return PushedOpenList;
}

vector<int> MakeOpenList(int StandardPos)
{
	vector<int> MakedOpenList;
	for (int i = 0; i < 3; ++i)
	{
		int OpenPos = StandardPos - (TileSize+1) + i;
		if (0 <= OpenPos && OpenPos < TileSize*TileSize)
		{
			float XGap = abs(Tiles[StandardPos].PosX - Tiles[OpenPos].PosX);
			float YGap = abs(Tiles[StandardPos].PosY - Tiles[OpenPos].PosY);
			if (XGap < 2.0f && YGap < 2.0f)
			{
				if (0 == i)
				{
					if(Tiles[OpenPos +1].bBlock || Tiles[OpenPos + TileSize].bBlock)
						continue;
				}
				else if (2 == i)
				{
					if (Tiles[OpenPos - 1].bBlock || Tiles[OpenPos + TileSize].bBlock)
						continue;
				}
				vector<int> OpenList = PushOpenList(StandardPos, OpenPos);
				for (int index : OpenList)
					MakedOpenList.push_back(index);
			}
		}
	}

	if (0 <= StandardPos - 1 && StandardPos - 1 < TileSize*TileSize)
	{
		int OpenPos = StandardPos - 1;
		float XGap = abs(Tiles[StandardPos].PosX - Tiles[OpenPos].PosX);
		float YGap = abs(Tiles[StandardPos].PosY - Tiles[OpenPos].PosY);
		if (XGap < 2.0f && YGap < 2.0f)
		{
			vector<int> OpenList = PushOpenList(StandardPos, OpenPos);
			for (int index : OpenList)
				MakedOpenList.push_back(index);
		}
	}
		

	if (0 <= StandardPos + 1 && StandardPos + 1 < TileSize*TileSize)
	{
		int OpenPos = StandardPos + 1;
		float XGap = abs(Tiles[StandardPos].PosX - Tiles[OpenPos].PosX);
		float YGap = abs(Tiles[StandardPos].PosY - Tiles[OpenPos].PosY);
		if (XGap < 2.0f && YGap < 2.0f)
		{
			vector<int> OpenList = PushOpenList(StandardPos, OpenPos);
			for (int index : OpenList)
				MakedOpenList.push_back(index);
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		int OpenPos = StandardPos + (TileSize-1) + i;
		if (0 <= OpenPos && OpenPos < TileSize*TileSize)
		{
			float XGap = abs(Tiles[StandardPos].PosX - Tiles[OpenPos].PosX);
			float YGap = abs(Tiles[StandardPos].PosY - Tiles[OpenPos].PosY);
			if (XGap < 2.0f && YGap < 2.0f)
			{
				if (0 == i)
				{
					if (Tiles[OpenPos + 1].bBlock || Tiles[OpenPos - TileSize].bBlock)
						continue;
				}
				else if (2 == i)
				{
					if (Tiles[OpenPos - 1].bBlock || Tiles[OpenPos - TileSize].bBlock)
						continue;
				}

				vector<int> OpenList = PushOpenList(StandardPos, OpenPos);
				for (int index : OpenList)
					MakedOpenList.push_back(index);
			}
		}
	}

	return MakedOpenList;
}

int FindNextPathIndexFromOpenList()
{
	int FindPath = 0;
	float FindFitness = 999999999.0f;
	for (int CurIndex : OpenList)
	{
		if (Tiles[CurIndex].Fitness < FindFitness)
		{
			FindPath = CurIndex;
			FindFitness = Tiles[CurIndex].Fitness;
		}
	}

	return FindPath;
}

int FindNextPathIndex(const vector<int>& CheckOpenList)
{
	int FindPath = 0;
	float FindFitness = 999999999.0f;
	for (int CurIndex : CheckOpenList)
	{
		if (Tiles[CurIndex].Fitness < FindFitness)
		{
			FindPath = CurIndex;
			FindFitness = Tiles[CurIndex].Fitness;
		}
	}

	return FindPath;
}

void ShowTile()
{
	for (int y = 0; y < TileSize; ++y)
	{
		for (int x = 0; x < TileSize; ++x)
		{
			std::vector<int>::iterator it_find = std::find(Paths.begin(), Paths.end(), y*TileSize + x);
			if (it_find != Paths.end())
			{
				cout << "@";
			}
			else if (Tiles[TileSize * y + x].bBlock)
			{
				cout << "#";
			}
			else
			{
				cout << "*";
			}
		}
		cout << endl;
	}
}


void main()
{
	for (int y = 0; y < TileSize; ++y)
	{
		for (int x = 0; x < TileSize; ++x)
		{
			Tile tile;
			tile.PosX = (float)x;
			tile.PosY = (float)y;

			if (y != 0 && y != 5 && y != 10 && x == 5)
				tile.bBlock = true;

			Tiles.push_back(tile);
		}
	}

	ShowTile();

	while (1)
	{
		ClosedList.clear();
		OpenList.clear();
		Paths.clear();

		for (Tile tile : Tiles)
		{
			if (tile.bBlock)
			{
				ClosedList.push_back(TileSize * (int)tile.PosY + (int)tile.PosX);
			}
		}
	
		std::cout << "Select Start Position : " << endl;
		cin >> g_StartPos;

		std::cout << "Select End Position : " << endl;
		cin >> g_EndPos;

		ClosedList.push_back(g_StartPos);
		int FindNextPath = g_StartPos;

		while (1)
		{
			vector<int> MakedOpenList = MakeOpenList(FindNextPath);
			if (0 < MakedOpenList.size())
			{
				FindNextPath = FindNextPathIndex(MakedOpenList);
			}
			else
			{
				FindNextPath = FindNextPathIndexFromOpenList();
			}

			if (FindNextPath == g_EndPos)
				break;

			std::remove(OpenList.begin(), OpenList.end(), FindNextPath);
			ClosedList.push_back(FindNextPath);
		}


		int path_index = g_EndPos;
		Paths.push_back(g_EndPos);
		while (1)
		{
			if (path_index == g_StartPos)
			{
				break;
			}

			Paths.push_back(Tiles[path_index].ParentTile);
			path_index = Tiles[path_index].ParentTile;
		}

		ShowTile();
	}
}