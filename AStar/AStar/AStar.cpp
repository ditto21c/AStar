#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

class CAStar
{
public:
	struct Tile
	{
		float Fitness = 0;
		float Goal = 0;
		float Heuristic = 0;
		float PosX = 0;
		float PosY = 0;
		bool bBlock = false;
		int ParentTile = 0;
		vector<int> OpenList;
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

	bool PushOpenList(int StandardPos, int OpenPos)
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
				Tiles[OpenPos].ParentTile = StandardPos;
				return true;
			}
		}

		return false;
	}


	vector<int> FindNearOpenList(int StandardPos)
	{
		vector<int> FindedNearOpenList;

		for (int i = 0; i < 3; ++i)
		{
			int OpenPos = StandardPos - (TileSize + 1) + i;
			if (0 <= OpenPos && OpenPos < TileSize*TileSize)
			{
				float XGap = abs(Tiles[StandardPos].PosX - Tiles[OpenPos].PosX);
				float YGap = abs(Tiles[StandardPos].PosY - Tiles[OpenPos].PosY);
				if (XGap < 2.0f && YGap < 2.0f)
				{
					if (0 == i)
					{
						if (Tiles[OpenPos + 1].bBlock || Tiles[OpenPos + TileSize].bBlock)
							continue;
					}
					else if (2 == i)
					{
						if (Tiles[OpenPos - 1].bBlock || Tiles[OpenPos + TileSize].bBlock)
							continue;
					}
					FindedNearOpenList.push_back(OpenPos);
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
				FindedNearOpenList.push_back(OpenPos);
			}
		}


		if (0 <= StandardPos + 1 && StandardPos + 1 < TileSize*TileSize)
		{
			int OpenPos = StandardPos + 1;
			float XGap = abs(Tiles[StandardPos].PosX - Tiles[OpenPos].PosX);
			float YGap = abs(Tiles[StandardPos].PosY - Tiles[OpenPos].PosY);
			if (XGap < 2.0f && YGap < 2.0f)
			{
				FindedNearOpenList.push_back(OpenPos);
			}
		}

		for (int i = 0; i < 3; ++i)
		{
			int OpenPos = StandardPos + (TileSize - 1) + i;
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

					FindedNearOpenList.push_back(OpenPos);
				}
			}
		}

		return FindedNearOpenList;

	}

	vector<int> MakeOpenList(int StandardPos)
	{
		vector<int> MakedOpenList;

		vector<int> finded_near_openlist = FindNearOpenList(StandardPos);
		for (int open_pos : finded_near_openlist)
		{
			if (PushOpenList(StandardPos, open_pos))
				MakedOpenList.push_back(open_pos);
		}

		return MakedOpenList;
	}

	int FindNextPathIndexFromOpenList()
	{
		int FindPath = -1;
		float FindFitness = 999999999.0f;
		for (int CurIndex : OpenList)
		{
			vector<int>::iterator itFind = std::find(ClosedList.begin(), ClosedList.end(), CurIndex);
			if (itFind == ClosedList.end())
			{
				if (Tiles[CurIndex].Fitness < FindFitness)
				{
					FindPath = CurIndex;
					FindFitness = Tiles[CurIndex].Fitness;
				}
			}
		}

		return FindPath;
	}

	int FindNextPathIndex(const vector<int>& CheckOpenList)
	{
		int FindPath = -1;
		float FindFitness = 999999999.0f;
		for (int CurIndex : CheckOpenList)
		{
			vector<int>::iterator itFind = std::find(ClosedList.begin(), ClosedList.end(), CurIndex);
			if (itFind == ClosedList.end())
			{
				if (Tiles[CurIndex].Fitness < FindFitness)
				{
					FindPath = CurIndex;
					FindFitness = Tiles[CurIndex].Fitness;
				}
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

	void MakeTiles(bool* pTile, int InTileSize)
	{
		TileSize = InTileSize;
		
		for (int y = 0; y < TileSize; ++y)
		{
			for (int x = 0; x < TileSize; ++x)
			{
				Tile tile;
				tile.PosX = (float)x;
				tile.PosY = (float)y;
				tile.bBlock = pTile[y*TileSize + x];
				Tiles.push_back(tile);
			}
		}
	}

	void StartAStar(bool* pTile, int InTileSize)
	{
		MakeTiles(pTile, InTileSize);

		for (Tile tile : Tiles)
		{
			if (tile.bBlock)
			{
				ClosedList.push_back(TileSize * (int)tile.PosY + (int)tile.PosX);
			}
		}

		g_StartPos = 32;
		g_EndPos = 26;

		ClosedList.push_back(g_StartPos);
		int FindNextPath = g_StartPos;

		while (1)
		{
			vector<int> MakedOpenList = MakeOpenList(FindNextPath);
			if (0 < MakedOpenList.size())
			{
				Tiles[FindNextPath].OpenList = MakedOpenList;
				FindNextPath = FindNextPathIndex(MakedOpenList);
			}
			else
			{
				int ParentTile = Tiles[FindNextPath].ParentTile;
				FindNextPath = FindNextPathIndex(Tiles[ParentTile].OpenList);
				if (-1 == FindNextPath)
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

		/*MakeTiles(pTile, InTileSize);

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
				tile.OpenList.clear();
			}

			

			while (1)
			{
				std::cout << "Select Start Position : ";
				cin >> g_StartPos;
				if (0 <= g_StartPos && g_StartPos < TileSize*TileSize && false == Tiles[g_StartPos].bBlock)
					break;
			}

			while (1)
			{
				std::cout << "Select End Position : ";
				cin >> g_EndPos;
				if (0 <= g_EndPos && g_EndPos < TileSize*TileSize && false == Tiles[g_EndPos].bBlock)
					break;
			}

			int bStart;
			std::cout << "Start(1) End(0) : " << endl;
			cin >> bStart;
			if (bStart != 1)
				break;


			ClosedList.push_back(g_StartPos);
			int FindNextPath = g_StartPos;

			while (1)
			{
				vector<int> MakedOpenList = MakeOpenList(FindNextPath);
				if (0 < MakedOpenList.size())
				{
					Tiles[FindNextPath].OpenList = MakedOpenList;
					FindNextPath = FindNextPathIndex(MakedOpenList);
				}
				else
				{
					int ParentTile = Tiles[FindNextPath].ParentTile;
					FindNextPath = FindNextPathIndex(Tiles[ParentTile].OpenList);
					if(-1 == FindNextPath)
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
		}*/
	}
};