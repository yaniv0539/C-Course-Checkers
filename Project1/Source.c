#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BOARD_SIZE 8

#define FIRST_PLAYER 'T'
#define SECOND_PLAYER 'B'
#define EMPTY_PLACE ' '

#define LEFT 0
#define RIGHT 1
#define UP -1
#define DOWN 1

#define SINGLE_MOVE 1
#define DOUBLE_MOVE 2

#define STUCK 0
#define MOVE_WITHOUT_CAPTURE 1
#define MOVE_WITH_CAPTURE 2

#define FIRST_COL 0
#define SECOND_COL 1
#define FIRST_ROW 0
#define SECOND_ROW 1

#define NO_MOVES_AT_ALL -1

#define ISWITHCAPTURE(toCapture) (((toCapture) == (MOVE_WITH_CAPTURE)) ? 1 : 0)
#define COL_MOVE(p, dir) ( (p) == FIRST_PLAYER ) ? ( ( (dir) == RIGHT ) ? (RIGHT) : (LEFT - 1) ) :\
												   ( ( (dir) == RIGHT ) ? (LEFT - 1) : (RIGHT) )
#define ROW_MOVE(p) ((p) == FIRST_PLAYER) ? DOWN : UP

#define ROW_CHAR_TO_INT(row) ((row) - 'A')
#define COL_CHAR_TO_INT(col) ((col) - '1')

#define ROW_INT_TO_CHAR(row) ((row) + 'A')
#define COL_INT_TO_CHAR(col) ((col) + '1')

typedef struct checkersPos
{
	char row, col;
} checkersPos;

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

typedef unsigned char Player;

// Q1 Structs
typedef struct SingleSourceMovesTreeNode
{
	Board board;
	checkersPos* pos;
	unsigned short total_captures_so_far;
	struct SingleSourceMovesTreeNode* next_moves[2];
} SingleSourceMovesTreeNode;

typedef struct SingleSourceMovesTree
{
	SingleSourceMovesTreeNode* source;
} SingleSourceMovesTree;

// Global Functions
void buildBoard(Board board);
Player getPlayerFromPos(Board board, checkersPos* pSrc);

// Q1
void makeEmptySingleSourceMovesTree(SingleSourceMovesTree* tr);
SingleSourceMovesTreeNode* createNewSingleSourceMovesTreeNode(Board board, checkersPos* pSrc,
	unsigned short numOfCaptures, SingleSourceMovesTreeNode* next_moves[]);

// Q1
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* pSrc);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelper(Board board, checkersPos* pSrc,
	Player p, unsigned short numOfCaptures, bool isFirstMove);
void createNewBoard(Board res, Board oldBoard, checkersPos* pSrc, Player p, int dir, int steps);
checkersPos* createNewCheckersPos(int row, int col, Player p, int dir, int steps);
int checkDiagonal(Board board, checkersPos* pSrc, Player p, unsigned short numOfCaptures, int dir, bool isFirstMove);

void main()
{
	Board board;
	SingleSourceMovesTree* pTree;
	checkersPos pos = { 'G', '2' };

	buildBoard(board);

	pTree = FindSingleSourceMoves(board, &pos);
}

// Trees Functions

// Makes empty single source moves tree by initialized to NULL
void makeEmptySingleSourceMovesTree(SingleSourceMovesTree* tr)
{
	tr->source = NULL;
}

// Creates new single source move tree node by assimilate inside: The board, his source position,
// number of captures so far and array with his next possible moves.
SingleSourceMovesTreeNode* createNewSingleSourceMovesTreeNode(Board board, checkersPos* pSrc,
	unsigned short numOfCaptures, SingleSourceMovesTreeNode* next_moves[])
{
	SingleSourceMovesTreeNode* newNode = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));

	if (!newNode)
	{
		printf("Memory Allocation Failure!!!");
		exit(1);
	}

	memcpy(newNode->board, board, sizeof(char) * BOARD_SIZE * BOARD_SIZE);
	newNode->pos = pSrc;
	newNode->total_captures_so_far = numOfCaptures;
	newNode->next_moves[LEFT] = next_moves[LEFT];
	newNode->next_moves[RIGHT] = next_moves[RIGHT];

	return newNode;
}

// Q1

// Temporary function to build board. 
void buildBoard(Board board)
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			board[i][j] = ' ';

	board[3][0] = FIRST_PLAYER;
	board[1][2] = FIRST_PLAYER;
	board[5][2] = FIRST_PLAYER;
	board[1][4] = FIRST_PLAYER;
	board[3][4] = FIRST_PLAYER;
	board[1][6] = FIRST_PLAYER;
	board[3][6] = FIRST_PLAYER;
	//board[1][6] = FIRST_PLAYER;
	//board[2][1] = FIRST_PLAYER;
	//board[2][3] = FIRST_PLAYER;
	//board[2][5] = FIRST_PLAYER;
	//board[2][7] = FIRST_PLAYER;

	board[6][1] = SECOND_PLAYER;
	//board[5][2] = SECOND_PLAYER;
	//board[5][4] = SECOND_PLAYER;
	//board[5][6] = SECOND_PLAYER;
	//board[6][1] = SECOND_PLAYER;
	//board[6][3] = SECOND_PLAYER;
	//board[6][5] = SECOND_PLAYER;
	//board[6][7] = SECOND_PLAYER;
	//board[7][0] = SECOND_PLAYER;
	//board[7][2] = SECOND_PLAYER;
	//board[7][4] = SECOND_PLAYER;
	//board[7][6] = SECOND_PLAYER;
}

// Gets the player name from position.
Player getPlayerFromPos(Board board, checkersPos* pSrc)
{
	int rowNum, colNum;

	rowNum = ROW_CHAR_TO_INT(pSrc->row);
	colNum = COL_CHAR_TO_INT(pSrc->col);

	return board[rowNum][colNum];
}

// Creates new single source moves tree. If position not valid returns NULL. Else calls helper function.
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* pSrc)
{
	SingleSourceMovesTree* pTree;
	Player p;

	p = getPlayerFromPos(board, pSrc);

	if (p == EMPTY_PLACE)
		return NULL;

	pTree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));

	if (!pTree)
	{
		printf("Memory Allocation Failure!!!");
		exit(1);
	}

	makeEmptySingleSourceMovesTree(pTree);

	pTree->source = FindSingleSourceMovesHelper(board, pSrc, p, 0, true);

	return pTree;
}

// Recursive helper function to 'FindSingleSourceMoves'. Creates all the possible routes for single source. 
SingleSourceMovesTreeNode* FindSingleSourceMovesHelper(Board board, checkersPos* pSrc,
	Player p, unsigned short numOfCaptures, bool isFirstMove)
{
	SingleSourceMovesTreeNode* next_moves[2] = { NULL };
	int rightRoute, leftRoute;
	checkersPos* rightSrc, * leftSrc;
	Board rightBoard, leftBoard;

	rightRoute = checkDiagonal(board, pSrc, p, numOfCaptures, RIGHT, isFirstMove);
	leftRoute = checkDiagonal(board, pSrc, p, numOfCaptures, LEFT, isFirstMove);

	if (rightRoute != STUCK)
	{
		rightSrc = createNewCheckersPos(pSrc->row, pSrc->col, p, RIGHT, rightRoute);
		createNewBoard(rightBoard, board, pSrc, p, RIGHT, rightRoute);
		next_moves[RIGHT] = FindSingleSourceMovesHelper(rightBoard, rightSrc, p, numOfCaptures + ISWITHCAPTURE(rightRoute), false);
	}

	if (leftRoute != STUCK)
	{
		leftSrc = createNewCheckersPos(pSrc->row, pSrc->col, p, LEFT, leftRoute);
		createNewBoard(leftBoard, board, pSrc, p, LEFT, leftRoute);
		next_moves[LEFT] = FindSingleSourceMovesHelper(leftBoard, leftSrc, p, numOfCaptures + ISWITHCAPTURE(leftRoute), false);
	}

	return createNewSingleSourceMovesTreeNode(board, pSrc, numOfCaptures, next_moves);
}

// Creates new board by the data that was given - Old borad, new Board, player, his direction and how many steps he moved.
void createNewBoard(Board res, Board oldBoard, checkersPos* pSrc, Player p, int dir, int steps)
{
	int rowNum, colNum;

	rowNum = ROW_CHAR_TO_INT(pSrc->row);
	colNum = COL_CHAR_TO_INT(pSrc->col);

	memcpy(res, oldBoard, sizeof(char) * BOARD_SIZE * BOARD_SIZE);

	res[rowNum][colNum] = ' ';
	res[rowNum + (ROW_MOVE(p) * steps)][colNum + (COL_MOVE(p, dir) * steps)] = p;
	if (steps == DOUBLE_MOVE)
		res[rowNum + (ROW_MOVE(p))][colNum + (COL_MOVE(p, dir))] = ' ';
}

// Creates new position by the data that was given - row, column, player, his direction and how many steps he wants to move. 
checkersPos* createNewCheckersPos(int row, int col, Player p, int dir, int steps)
{
	checkersPos* res;

	res = (checkersPos*)malloc(sizeof(checkersPos));

	if (!res)
	{
		printf("Memory Allocation Failure!!!");
		exit(1);
	}

	res->row = row + (ROW_MOVE(p) * steps);
	res->col = col + (COL_MOVE(p, dir) * steps);

	return res;
}

// Checks the all possible routes for specific source on board. 
int checkDiagonal(Board board, checkersPos* pSrc, Player p, unsigned short numOfCaptures, int dir, bool isFirstMove)
{
	int rowNum, colNum;

	rowNum = ROW_CHAR_TO_INT(pSrc->row);
	colNum = COL_CHAR_TO_INT(pSrc->col);

	// Checks if not first move and didn't capture yet.
	if (!numOfCaptures && !isFirstMove)
		return STUCK;

	// Checks if stuck after single move.
	if ((p == FIRST_PLAYER && dir == LEFT && colNum == FIRST_COL) ||
		(p == FIRST_PLAYER && dir == RIGHT && colNum == BOARD_SIZE - 1) ||
		(p == SECOND_PLAYER && dir == LEFT && colNum == BOARD_SIZE - 1) ||
		(p == SECOND_PLAYER && dir == RIGHT && colNum == FIRST_COL))
		return STUCK;

	// Checks if should move without capture.
	else if ((p == FIRST_PLAYER && dir == LEFT && board[rowNum + SINGLE_MOVE][colNum - SINGLE_MOVE] == EMPTY_PLACE && !numOfCaptures) ||
		(p == FIRST_PLAYER && dir == RIGHT && board[rowNum + SINGLE_MOVE][colNum + SINGLE_MOVE] == EMPTY_PLACE && !numOfCaptures) ||
		(p == SECOND_PLAYER && dir == LEFT && board[rowNum - SINGLE_MOVE][colNum + SINGLE_MOVE] == EMPTY_PLACE && !numOfCaptures) ||
		(p == SECOND_PLAYER && dir == RIGHT && board[rowNum - SINGLE_MOVE][colNum - SINGLE_MOVE] == EMPTY_PLACE && !numOfCaptures))
		return MOVE_WITHOUT_CAPTURE;

	// Checks if stuck after double move.
	else if ((p == FIRST_PLAYER && dir == LEFT && colNum == SECOND_COL) ||
		(p == FIRST_PLAYER && dir == RIGHT && colNum == BOARD_SIZE - 2) ||
		(p == SECOND_PLAYER && dir == LEFT && colNum == BOARD_SIZE - 2) ||
		(p == SECOND_PLAYER && dir == RIGHT && colNum == SECOND_COL))
		return STUCK;

	// Checks if should move and capture!
	else if ((p == FIRST_PLAYER && dir == LEFT && board[rowNum + SINGLE_MOVE][colNum - SINGLE_MOVE] == SECOND_PLAYER && board[rowNum + DOUBLE_MOVE][colNum - DOUBLE_MOVE] == ' ') ||
		(p == FIRST_PLAYER && dir == RIGHT && board[rowNum + SINGLE_MOVE][colNum + SINGLE_MOVE] == SECOND_PLAYER && board[rowNum + DOUBLE_MOVE][colNum + DOUBLE_MOVE] == ' ') ||
		(p == SECOND_PLAYER && dir == LEFT && board[rowNum - SINGLE_MOVE][colNum + SINGLE_MOVE] == FIRST_PLAYER && board[rowNum - DOUBLE_MOVE][colNum + DOUBLE_MOVE] == ' ') ||
		(p == SECOND_PLAYER && dir == RIGHT && board[rowNum - SINGLE_MOVE][colNum - SINGLE_MOVE] == FIRST_PLAYER && board[rowNum - DOUBLE_MOVE][colNum - DOUBLE_MOVE] == ' '))
		return MOVE_WITH_CAPTURE;

	return STUCK;
}
