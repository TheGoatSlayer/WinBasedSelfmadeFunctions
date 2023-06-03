#include <iostream>
#include <Windows.h>
#include <memory.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;
#define MAP_WIDTH	10
#define MAP_LENGTH	20
#define BLOCK_TYPE_NUM 7

const int Block_I[4][4][4] = {	//Set CLOCKWISE
	{{0,0,0,0},	//Block_I[RotationNum::DEFAULT][][]
	{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0} },

	{{0,1,0,0},	//Block_I[RotationNum::ONCE][][]
	{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0}},

	{{0,0,0,0},	//Block_I[RotationNum::TWICE][][]
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}},

	
	{{0,0,1,0},	//Block_I[RotationNum::THREETIMES][][]
	{0,0,1,0},
	{0,0,1,0},
	{0,0,1,0}},
};
const int Block_O[4][4][4] = {  //CLOCKWISE
	{{0,0,0,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0}},
};
const int Block_T[4][4][4] = {  //CLOCKWISE
	{{0,0,0,0},
	{0,1,0,0},
	{1,1,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,0,0},
	{0,1,1,0},
	{0,1,0,0}},

	{{0,0,0,0},
	{0,0,0,0},
	{1,1,1,0},
	{0,1,0,0}},

	{{0,0,0,0},
	{0,1,0,0},
	{1,1,0,0},
	{0,1,0,0}},
};
const int Block_L[4][4][4] = {  //CLOCKWISE
	{{0,1,0,0},
	{0,1,0,0},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,1,1},
	{0,1,0,0},
	{0,0,0,0}},

	{{0,1,1,0},
	{0,0,1,0},
	{0,0,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,0,0,1},
	{0,1,1,1},
	{0,0,0,0}},
};
const int Block_J[4][4][4] = {  //CLOCKWISE
	{{0,0,1,0},
	{0,0,1,0},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,0,0},
	{0,1,1,1},
	{0,0,0,0}},

	{{0,1,1,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,1,1},
	{0,0,0,1},
	{0,0,0,0}},
};
const int Block_S[4][4][4] = {  //CLOCKWISE
	{{0,0,0,0},
	{0,0,1,1},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,0,1,0},
	{0,0,1,1},
	{0,0,0,1},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,0,1,1},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,0,1,0},
	{0,0,1,1},
	{0,0,0,1},
	{0,0,0,0}},
};
const int Block_Z[4][4][4] = {  //CLOCKWISE
	{{0,0,0,0},
	{0,1,1,0},
	{0,0,1,1},
	{0,0,0,0}},

	{{0,0,1,0},
	{0,1,1,0},
	{0,1,0,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,1,0},
	{0,0,1,1},
	{0,0,0,0}},

	{{0,0,1,0},
	{0,1,1,0},
	{0,1,0,0},
	{0,0,0,0}},
};

enum Flags { EMPTY = 0, BLOCK = 1, OBSTACLE = 2, WALL = 3 };
enum BlockType { I = 0, O, T, L, J, S, Z };
enum RotationNum { DEFAULT = 0, ONCE, TWICE, THREETIME };
enum Direction { LEFT = 75, RIGHT = 77, DOWN = 80 };

typedef const int(*BlockPtr)[4][4];
const BlockPtr& GetBlockByType(int BlockType)
{
	switch (BlockType)
	{
	case I:
		return Block_I;
	case O:
		return Block_O;
	case T:
		return Block_T;
	case L:
		return Block_L;
	case J:
		return Block_J;
	case S:
		return Block_S;
	case Z:
		return Block_Z;
	default:
		break;
	}
}

class MapCLASS
{
	friend class BlockCLASS;
	friend class TetrisGameCLASS;
public:
	MapCLASS()
	{
		for (int i = 0; i < MAP_LENGTH; i++)
			for (int j = 0; j < MAP_WIDTH; j++)
				if (i == (MAP_LENGTH - 1) || j == 0 || j == (MAP_WIDTH - 1))
					Map[i][j] = WALL;
				else Map[i][j] = EMPTY;
		SynchronizeObstacleMapToMap();
	}

	int CheckFullRows()
	{
		int CountFullRows = 0;
		int FullRows[MAP_LENGTH - 1] = { 0, };	// 0 is not a full row, 1 is a full row
		for (int i = 0; i < MAP_LENGTH - 1; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				if (Map[i][j] == EMPTY)
					break;
				else if(j == MAP_WIDTH-1)
					FullRows[i] = 1;
			}
		}

		for (int i = 0; i < MAP_LENGTH - 1; i++)
		{
			if (FullRows[i] == 1)
			{
				CountFullRows++;
				memmove((Map + 1), Map, sizeof(int) * MAP_WIDTH * i);
				memset(&Map[0][1], 0, sizeof(int) * (MAP_WIDTH - 2));
			}
		}
		return CountFullRows;
	}
	void ChangeBlockToObstacle()
	{
		for (int i = 0; i < MAP_LENGTH; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				if (Map[i][j] == BLOCK)
					Map[i][j] = OBSTACLE;
			}
		}
	}
	
	bool bGameOver()
	{
		for (int i = 0; i < MAP_WIDTH; i++)
			if (Map[0][i] == OBSTACLE)
				return true;
		return false;
	}
	
	void SynchronizeObstacleMapToMap()
	{
		memcpy(ObstacleMap, Map, sizeof(int) * MAP_LENGTH * MAP_WIDTH);
	}
	void SynchronizeMapToObstacleMap()
	{
		memcpy(Map, ObstacleMap, sizeof(int) * MAP_LENGTH * MAP_WIDTH);
	}
private:
	int Map[MAP_LENGTH][MAP_WIDTH];
	int ObstacleMap[MAP_LENGTH][MAP_WIDTH];
};

class BlockCLASS
{
	friend class TetrisGameCLASS;
public:
	BlockCLASS(int typeinfo) : TypeInfo(typeinfo), CurX(3), CurY(0), RotationInfo(DEFAULT)
	{
		SetTopEnd();
		SetBottomEnd();
		CurY -= TopEnd;
	}

	void GoDown() { CurY++; }
	void Move(int Direction)
	{
		switch (Direction)
		{
		case LEFT:
			CurX--;
			break;
		case RIGHT:
			CurX++;
			break;
		default:
			break;
		}
	}
	void Rotate(int Direction)
	{
		switch (Direction)
		{
		case '<':
		case ',':
			RotationInfo = (RotationInfo + 1) % 4;
			break;
		case '>':
		case '.':
			 RotationInfo = (RotationInfo - 1 < 0) ? THREETIME : (RotationInfo - 1);
			break;
		default:
			break;
		}
	}

	bool bValidMove(int Direction, MapCLASS& AMap)
	{
		SetLeftEnd(); SetRightEnd();
		const BlockPtr pBlock = GetBlockByType(TypeInfo);
		int Operand = (Direction == LEFT) ? -1 : 1;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if ((Direction == LEFT && LeftEnd == 1) 
				|| (Direction == RIGHT && RightEnd == (MAP_WIDTH - 2)))
					return false;
				else if(pBlock[RotationInfo][i][j] == 1
				&& AMap.ObstacleMap[CurY + i][CurX + Operand + j] == OBSTACLE)
				{
					return false;
				}
			}
		}
		return true;
	}
	bool bValidRotation(int Direction, MapCLASS& AMap)
	{
		Rotate(Direction);
		SetAllEnd();
		const BlockPtr pBlock = GetBlockByType(TypeInfo);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if(LeftEnd < 1 || RightEnd > MAP_WIDTH - 2
				|| TopEnd < 0 || BottomEnd > MAP_LENGTH - 2
				|| (pBlock[RotationInfo][i][j] == 1
				&& AMap.ObstacleMap[CurY + i][CurX + j] == OBSTACLE))
				{
					(Direction == '<' || Direction == ',') ? Rotate('>') : Rotate('<');
					return false;
				}
			}
		}
		(Direction == '<' || Direction == ',') ? Rotate('>') : Rotate('<');
		return true;
	}

	bool bMapEnd(MapCLASS& AMap)
	{
		SetBottomEnd();
		const BlockPtr pBlock = GetBlockByType(TypeInfo);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (BottomEnd == MAP_LENGTH - 2)
					return true;
				else if(pBlock[RotationInfo][i][j] == 1
					 && AMap.Map[(CurY + 1) + i][CurX + j] == OBSTACLE)
				{
					return true;
				}
			}
		}
		return false;
	}

	void PutBlockIntoMap(MapCLASS& AMap)
	{
		const BlockPtr pBlock = GetBlockByType(TypeInfo);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (pBlock[RotationInfo][i][j] == 1)
					AMap.Map[CurY + i][CurX + j] = BLOCK;
			}
		}
	}

	void SetLeftEnd()
	{
		const BlockPtr pBlock = GetBlockByType(TypeInfo);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (pBlock[RotationInfo][j][i] == 1)
				{
					LeftEnd = (CurX+i);
					return;
				}
	}
	void SetRightEnd()
	{
		const BlockPtr pBlock = GetBlockByType(TypeInfo);
		for (int i = 3; i >= 0; i--)
			for (int j = 0; j < 4; j++)
				if (pBlock[RotationInfo][j][i] == 1)
				{
					RightEnd = (CurX+i);
					return;
				}
	}
	void SetTopEnd()
	{
		const BlockPtr pBlock = GetBlockByType(TypeInfo);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (pBlock[RotationInfo][i][j] == 1)
				{
					TopEnd = (CurY+i);
					return;
				}
	}
	void SetBottomEnd()
	{
		const BlockPtr pBlock = GetBlockByType(TypeInfo);
		for (int i = 3; i >= 0; i--)
			for(int j = 0; j < 4; j++)
			if (pBlock[RotationInfo][i][j] == 1)
			{
				BottomEnd = (CurY+i);
				return;
			}
	}
	void SetAllEnd()
	{
		SetLeftEnd(); SetRightEnd(); SetTopEnd(); SetBottomEnd();
	}
private:
	int TypeInfo;
	int RotationInfo;
	int CurX, CurY;
	int LeftEnd, RightEnd, TopEnd, BottomEnd;
};

class TetrisGameCLASS
{
public:
	TetrisGameCLASS() : AMap() {}
	void StartGame()
	{
		SetRandomTypeArr(NextRandomTypeArr);
		do
		{
			if ((blockCount % BLOCK_TYPE_NUM) == 0)
			{
				memcpy(RandomTypeArr, NextRandomTypeArr, sizeof(int) * BLOCK_TYPE_NUM);
				SetRandomTypeArr(NextRandomTypeArr);
			}
			BlockCLASS block(RandomTypeArr[blockCount % BLOCK_TYPE_NUM]);
			blockCount++;
			DrawMap(block);
			while (true)
			{
				clock_t start, finish;
				start = clock();
				do
				{
					block.SetAllEnd();
					if (_kbhit())
					{
						int input = _getch();
						if (input == 224)
						{
							int dir = _getch();
							if (dir == DOWN)
								break;
							else if (block.bValidMove(dir, AMap))
								block.Move(dir);
						}
						else if (input == 'q' || input == 'Q')
						{
							return;
						}
						else if (input == '<' || input == '>' || input == ',' || input == '.')
						{
							if (block.bValidRotation(input, AMap))
								block.Rotate(input);
						}
						block.PutBlockIntoMap(AMap);
						DrawMap(block);
					}
					finish = clock();
				} while ((double)(finish - start) / CLOCKS_PER_SEC <= 1.0);
				if (block.bMapEnd(AMap))
					break;
				block.GoDown();
				DrawMap(block);
			}
			block.PutBlockIntoMap(AMap);
			AMap.ChangeBlockToObstacle();
			Score += 100 * AMap.CheckFullRows();
			AMap.SynchronizeObstacleMapToMap();
			DrawMap(block);
		} while (!(AMap.bGameOver()));
	}
	void DrawMap(BlockCLASS& ABlock)
	{
		system("cls");
		cout << "YOUR SCORE IS [ " << Score << " ]" << endl;
		DrawThisTimeBlocks();
		DrawNextBlockType();
		ABlock.PutBlockIntoMap(AMap);
		for (int i = 0; i < MAP_LENGTH; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				switch (AMap.Map[i][j])
				{
				case EMPTY:
					cout << "..";
					break;
				case BLOCK:
					cout << "¡á";
					break;
				case OBSTACLE:
					cout << "¡à";
					break;
				case WALL:
					cout << "¢È";
					break;
				default:
					break;
				}

			}
			cout << endl;
		}
		AMap.SynchronizeMapToObstacleMap();
	}
	void DrawNextBlockType()
	{
		const BlockPtr pBlock = (blockCount % BLOCK_TYPE_NUM == 0)
								? GetBlockByType(NextRandomTypeArr[0])
								: GetBlockByType(RandomTypeArr[blockCount % BLOCK_TYPE_NUM]);
		cout << "NEXT BLOCK IS.." << endl;
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cout << ((pBlock[DEFAULT][i][j] == 1) ? "¡á" : "  ");
			}
			cout << endl;
		}
	}
	void DrawThisTimeBlocks()
	{
		cout << "THIS TIME BLOCKS ARE.." << endl;
		cout << "<< ";
		for (int i = 0; i < BLOCK_TYPE_NUM; i++)
		{
			switch (RandomTypeArr[i])
			{
			case 0:
				cout << "I ";
				break;
			case 1:
				cout << "O ";
				break;
			case 2:
				cout << "T ";
				break;
			case 3:
				cout << "L ";
				break;
			case 4:
				cout << "J ";
				break;
			case 5:
				cout << "S ";
				break;
			case 6:
				cout << "Z ";
				break;
			default:
				break;
			}
		}
		cout << ">>";
		cout << endl << endl;
	}
	void SetRandomTypeArr(int TypeArr[])
	{
		int DummyArr[BLOCK_TYPE_NUM];
		for (int i = 0; i < BLOCK_TYPE_NUM; i++)
		{
			DummyArr[i] = i;
		}
		for (int i = 0; i < BLOCK_TYPE_NUM; i++)
		{
			int RandNum = rand() % (BLOCK_TYPE_NUM - i);
			TypeArr[i] = DummyArr[RandNum];
			for (int j = 0; j < (BLOCK_TYPE_NUM -i)-RandNum; j++)
			{
				DummyArr[RandNum + j] = DummyArr[RandNum + j + 1];
			}
		}
	}
	int GetScore() { return Score; }
private:
	MapCLASS AMap;
	int RandomTypeArr[BLOCK_TYPE_NUM];
	int NextRandomTypeArr[BLOCK_TYPE_NUM];
	int blockCount = 0;
	int Score = 0;
};

void main()
{
	string name;
	srand((unsigned)time(NULL));
	cout << "YOU CAN USE.." << endl << endl;
	cout << "  ¡ç  ¡æ  ¡é  <(,)  >(.) Q(q)=Quit " << endl << endl;
	cout << "PRESS TO START..";
	_getch();
	TetrisGameCLASS playTetris;
	playTetris.StartGame();
	cout << "-------------GAMEOVER------------" << endl;
	cout << "-------------GAMEOVER------------" << endl;
	cout << "-------------GAMEOVER------------" << endl;
	cout << "-------------GAMEOVER------------" << endl;
	cout << "-------------GAMEOVER------------" << endl << endl << endl;
	cout << "FINAL SCORE = " << playTetris.GetScore() << endl << endl;
	cout << "ENTER YOUR NAME.. "; getline(cin, name);
	cout << "SCORE WILL BE SAVED IN \"TetrisScores.txt\"";

	ofstream file("TetrisScores.txt", ios_base::out | ios_base::app);
	if (file.is_open()) {
		file << playTetris.GetScore() << " / ";
		file << name << endl;
		file.close();
		_getch();
	}
}