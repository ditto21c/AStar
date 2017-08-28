#include <iostream>
#include <vector>

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
int g_StartPos = 0, g_EndPos = 0;

int CalcuGoal(int StandardPos, int GoalPos)
{
	return Tiles[StandardPos].Fitness + (abs(Tiles[StandardPos].PosX - Tiles[GoalPos].PosX) + abs(Tiles[StandardPos].PosX - Tiles[GoalPos].PosX));
}

int CalcuHeuristic(int CurPos, int EndPos)
{
	return (Tiles[EndPos].PosX - Tiles[CurPos].PosX) + (Tiles[EndPos].PosY - Tiles[CurPos].PosY);
}

void PushOpenList(int OpenPos, int StandardPos)
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
		int OpenPos = StandardPos - 11 + i;
		PushOpenList(OpenPos, StandardPos);
	}

	PushOpenList(StandardPos - 1, StandardPos);
	PushOpenList(StandardPos + 1, StandardPos);

	for (int i = 0; i < 3; ++i)
	{
		int OpenPos = StandardPos + 9 + i;
		PushOpenList(OpenPos, StandardPos);
	}
}

void main()
{
	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 100; ++x)
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
	Tiles[g_StartPos].Goal = CalcuGoal(g_StartPos, g_StartPos);
	Tiles[g_StartPos].Heuristic = CalcuHeuristic(g_StartPos, g_EndPos);
	Tiles[g_StartPos].Fitness = Tiles[g_StartPos].Goal + Tiles[g_StartPos].Heuristic;



	

}