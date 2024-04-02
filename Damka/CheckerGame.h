#ifndef _CheckerGame
#define _CheckerGame

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ListHandler.h"


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
#define LAST_ROW 7

#define NO_MOVES_AT_ALL -1

#define ISWITHCAPTURE(toCapture) (((toCapture) == (MOVE_WITH_CAPTURE)) ? 1 : 0)
#define COL_MOVE(p, dir) ( (p) == FIRST_PLAYER ) ? ( ( (dir) == RIGHT ) ? (RIGHT) : (LEFT - 1) ) :\
												   ( ( (dir) == RIGHT ) ? (LEFT - 1) : (RIGHT) )
#define ROW_MOVE(p) ((p) == FIRST_PLAYER) ? DOWN : UP

#define ROW_CHAR_TO_INT(row) ((row) - 'A')
#define COL_CHAR_TO_INT(col) ((col) - '1')

#define ROW_INT_TO_CHAR(row) ((row) + 'A')
#define COL_INT_TO_CHAR(col) ((col) + '1')

#define BOARD_PRINT_SIZE 19
#define MAX_CAPTURES 12

#define SWITCH_PLAYER(pNum) ((pNum) == (0)) ? ((pNum) = (1)) : ((pNum) = (0))

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

typedef unsigned char Player;

typedef struct Stats {
	Player player;
	int total_moves;
	int total_captures;
	int best_capture;
} STATS;

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

// Tree Functions

// Q1
void makeEmptySingleSourceMovesTree(SingleSourceMovesTree* tr);
SingleSourceMovesTreeNode* createNewSingleSourceMovesTreeNode(Board board, checkersPos* pSrc,
	unsigned short numOfCaptures, SingleSourceMovesTreeNode* next_moves[]);
void freeSingleSourceMoveTree(SingleSourceMovesTree* tr);
void freeSingleSourceMoveTreeHelper(SingleSourceMovesTreeNode* source);

// Global Functions
void buildBoard(Board board);
Player getPlayerFromPos(Board board, checkersPos* pSrc);

// Questions Functions

// Q1
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* pSrc);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelper(Board board, checkersPos* pSrc,
	Player p, unsigned short numOfCaptures, bool isFirstMove);
void createNewBoard(Board res, Board oldBoard, checkersPos* pSrc, Player p, int dir, int steps);
checkersPos* createNewCheckersPos(char row, char col, Player p, int dir, int steps);
int checkDiagonal(Board board, checkersPos* pSrc, Player p, unsigned short numOfCaptures, int dir, bool isFirstMove);

// Q2
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree);
SingleSourceMovesList* FindSingleSourceOptimalMoveHelper(SingleSourceMovesTreeNode* root, Player p);
bool isRightListBetter(SingleSourceMovesList* right_list, SingleSourceMovesList* left_list, Player p);

// Q3
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);

// Q4
void Turn(Board board, Player player);
SingleSourceMovesList* getBestListForPlayer(Board board, Player player);
bool isCurrSourceTheBest(Player player, MultipleSourceMovesListCell* source_best_move,
	MultipleSourceMovesListCell* player_best_move, int max_captures, int curr_source_best_captures);
bool hasValidMove(MultipleSourceMovesListCell* source_best_move);
void updateBoard(Board board, SingleSourceMovesTreeNode* source, checkersPos* pSrc);

// Q5
void PlayGame(Board board, Player starting_player);
void addStats(STATS* players_stats, int pNum, SingleSourceMovesList* lst);
void printBoard(Board board);
bool checkWinner(Board board, STATS* players_stats);
void printStats(STATS* players_stats, int pNum);

#endif // !_CheckerGame
