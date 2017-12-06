#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Tile
{
	int Fitness = 0;
	int Goal = 0;
	int Heuristic = 0;
	int PosX = 0;
	int PosY = 0;
};

vector<Tile> Tiles;
vector<int> OpenList;
vector<int> ClosedList;
vector<int> Paths;
int g_StartPos = 0, g_EndPos = 0;
int TileSize = 10;

int CalcuGoal(int StandardPos, int OpenPos)
{
	return Tiles[StandardPos].Fitness + (abs(Tiles[StandardPos].PosX - Tiles[OpenPos].PosX) + abs(Tiles[StandardPos].PosY - Tiles[OpenPos].PosY));
}

int CalcuHeuristic(int OpenPos, int EndPos)
{
	return (Tiles[EndPos].PosX - Tiles[OpenPos].PosX) + (Tiles[EndPos].PosY - Tiles[OpenPos].PosY);
}

void PushOpenList(int StandardPos, int OpenPos)
{
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
		}
	}
}

void MakeOpenList(int StandardPos)
{
	for (int i = 0; i < 3; ++i)
	{
		int OpenPos = StandardPos - (TileSize+1) + i;
		PushOpenList(StandardPos, OpenPos);
	}

	PushOpenList(StandardPos, StandardPos - 1);
	PushOpenList(StandardPos, StandardPos + 1);

	for (int i = 0; i < 3; ++i)
	{
		int OpenPos = StandardPos + (TileSize-1) + i;
		PushOpenList(StandardPos, OpenPos);
	}
}

int FindNextPathFromOpenList()
{
	int FindPath = 0;
	int FindFitness = 0;
	for (int CurIndex : OpenList)
	{
		if (Tiles[CurIndex].Fitness < FindFitness)
		{

		}
	}
		
}

void main()
{
	for (int y = 0; y < TileSize; ++y)
	{
		for (int x = 0; x < TileSize; ++x)
		{
			Tile tile;
			tile.PosX = x;
			tile.PosY = y;
			Tiles.push_back(tile);
		}
	}
	
	std::cout << "Select Start Position : " << endl;
	cin >> g_StartPos;

	std::cout << "Select End Position : " << endl;
	cin >> g_EndPos;

	ClosedList.push_back(g_StartPos);
	Tiles[g_StartPos].Goal = 0;
	Tiles[g_StartPos].Heuristic = CalcuHeuristic(g_StartPos, g_EndPos);
	Tiles[g_StartPos].Fitness = Tiles[g_StartPos].Heuristic;

	while (1)
	{

	}



	

}