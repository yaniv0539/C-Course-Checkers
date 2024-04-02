#define _CRT_SECURE_NO_WARNINGS
#include <string.h>

#include "CheckerGame.h"

// Global Functions

// This function builds the starting board.
void buildBoard(Board board)
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			board[i][j] = ' ';

	// Fisrt player arrangement
	board[0][1] = FIRST_PLAYER;
	board[0][3] = FIRST_PLAYER;
	board[0][5] = FIRST_PLAYER;
	board[0][7] = FIRST_PLAYER;
	board[1][0] = FIRST_PLAYER;
	board[1][2] = FIRST_PLAYER;
	board[1][4] = FIRST_PLAYER;
	board[1][6] = FIRST_PLAYER;
	board[2][1] = FIRST_PLAYER;
	board[2][3] = FIRST_PLAYER;
	board[2][5] = FIRST_PLAYER;
	board[2][7] = FIRST_PLAYER;

	// Second player arrangement
	board[5][0] = SECOND_PLAYER;
	board[5][2] = SECOND_PLAYER;
	board[5][4] = SECOND_PLAYER;
	board[5][6] = SECOND_PLAYER;
	board[6][1] = SECOND_PLAYER;
	board[6][3] = SECOND_PLAYER;
	board[6][5] = SECOND_PLAYER;
	board[6][7] = SECOND_PLAYER;
	board[7][0] = SECOND_PLAYER;
	board[7][2] = SECOND_PLAYER;
	board[7][4] = SECOND_PLAYER;
	board[7][6] = SECOND_PLAYER;
}

// This function gets the player name from position.
Player getPlayerFromPos(Board board, checkersPos* pSrc)
{
	int rowNum, colNum;

	rowNum = ROW_CHAR_TO_INT(pSrc->row);
	colNum = COL_CHAR_TO_INT(pSrc->col);

	return board[rowNum][colNum];
}


// Q1

// This function makes empty single source moves tree by initialized to NULL
void makeEmptySingleSourceMovesTree(SingleSourceMovesTree* tr)
{
	tr->source = NULL;
}

// This function creates new single source move tree node by assimilate inside: The board, his source position,
// number of captures so far and array with his next possible moves.
SingleSourceMovesTreeNode* createNewSingleSourceMovesTreeNode(Board board, checkersPos* pSrc,
	unsigned short numOfCaptures, SingleSourceMovesTreeNode* next_moves[])
{
	//SingleSourceMovesTreeNode* newNode = (SingleSourceMovesTreeNode*)myMalloc(sizeof(SingleSourceMovesTreeNode));
	SingleSourceMovesTreeNode* newNode = (SingleSourceMovesTreeNode*)myMalloc(sizeof(SingleSourceMovesTreeNode), "SingleSourceMovesTreeNode");

	memcpy(newNode->board, board, sizeof(char) * BOARD_SIZE * BOARD_SIZE);
	newNode->pos = pSrc;
	newNode->total_captures_so_far = numOfCaptures;
	newNode->next_moves[LEFT] = next_moves[LEFT];
	newNode->next_moves[RIGHT] = next_moves[RIGHT];

	return newNode;
}

// This function creates new single source moves tree. If position not valid returns NULL. Else calls helper function.
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* pSrc)
{
	SingleSourceMovesTree* pTree;
	Player p;
	
	p = getPlayerFromPos(board, pSrc);

	if (p == EMPTY_PLACE)
		return NULL;

	pTree = (SingleSourceMovesTree*)myMalloc(sizeof(SingleSourceMovesTree), "SingleSourceMovesTree");

	makeEmptySingleSourceMovesTree(pTree);

	pTree->source = FindSingleSourceMovesHelper(board, pSrc, p, 0, true);

	return pTree;
}

// This function is a recursive helper function to 'FindSingleSourceMoves'. Creates all the possible routes for single source. 
SingleSourceMovesTreeNode* FindSingleSourceMovesHelper(Board board, checkersPos* pSrc,
	Player p, unsigned short numOfCaptures, bool isFirstMove)
{
	SingleSourceMovesTreeNode* next_moves[2] = {NULL};
	int rightRoute, leftRoute;
	checkersPos* rightSrc, *leftSrc;
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

// This function creates new board by the data that was given - Old borad, new Board, player, his direction and how many steps he moved.
void createNewBoard(Board res, Board oldBoard, checkersPos* pSrc, Player p, int dir, int steps)
{
	int rowNum, colNum;

	rowNum = ROW_CHAR_TO_INT(pSrc->row);
	colNum = COL_CHAR_TO_INT(pSrc->col);
	
	memcpy(res, oldBoard, sizeof(char) * BOARD_SIZE * BOARD_SIZE);
	
	res[rowNum][colNum] = ' ';
	res[rowNum + ((ROW_MOVE(p)) * steps)][colNum + ((COL_MOVE(p, dir)) * steps)] = p;
	if (steps == DOUBLE_MOVE)
		res[rowNum + (ROW_MOVE(p))][colNum + (COL_MOVE(p, dir))] = ' ';
}

// This function creates new position by the data that was given - row, column, player, his direction and how many steps he wants to move. 
checkersPos* createNewCheckersPos(char row, char col, Player p, int dir, int steps)
{
	checkersPos* res;

	res = (checkersPos*)myMalloc(sizeof(checkersPos), "checkersPos");

	res->row = row + ((ROW_MOVE(p)) * steps);
	res->col = col + ((COL_MOVE(p, dir)) * steps);

	return res;
}

// This function checks the all possible routes for specific source on board. 
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

// This function calls helper function 'freeTreeHelper' to free the single source moves nodes and afterwards free the single source moves tree.
void freeSingleSourceMoveTree(SingleSourceMovesTree* tr)
{
	freeSingleSourceMoveTreeHelper(tr->source);
	myFree(tr, "SingleSourceMovesTree");
}

// This function free the nodes inside the single source moves tree.
void freeSingleSourceMoveTreeHelper(SingleSourceMovesTreeNode* source)
{
	if (source == NULL)
		return;
	freeSingleSourceMoveTreeHelper(source->next_moves[LEFT]);
	freeSingleSourceMoveTreeHelper(source->next_moves[RIGHT]);
	myFree(source->pos, "checkersPos");
	myFree(source, "SingleSourceMovesTreeNode");
}


// Q2

// This function gets a tree that represents optianl moves for a piece, and returns the optimal move as a SingleSourceMovesList.
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree)
{
	Player p;
	p = getPlayerFromPos(moves_tree->source->board, moves_tree->source->pos);
	return FindSingleSourceOptimalMoveHelper(moves_tree->source, p);
}

// This function is a helper to the previous. It gets the piece type an returns the optimal move as a SingleSourceMovesList.
SingleSourceMovesList* FindSingleSourceOptimalMoveHelper(SingleSourceMovesTreeNode* source, Player p)
{
	SingleSourceMovesList* res, * left_list, * right_list;
	SingleSourceMovesListCell* newCell;

	if (source == NULL)
		return NULL;

	// Recursive assumption
	right_list = FindSingleSourceOptimalMoveHelper(source->next_moves[RIGHT], p);
	left_list = FindSingleSourceOptimalMoveHelper(source->next_moves[LEFT], p);

	// 5 steps
	if (right_list == NULL && left_list == NULL)
	{
		res = (SingleSourceMovesList*)myMalloc(sizeof(SingleSourceMovesList), "SingleSourceMovesList");
		makeEmptySSMList(res);
	}
	else if (right_list != NULL && left_list == NULL)
		res = right_list;
	else if (right_list == NULL && left_list != NULL)
		res = left_list;
	else if (isRightListBetter(right_list, left_list, p))
	{
		res = right_list;
		freeSingleSourceMovesList(left_list);
	}
	else
	{
		res = left_list;
		freeSingleSourceMovesList(right_list);
	}

	// Insert the current source to res
	newCell = createNewSSMListCell(source->pos, source->total_captures_so_far, NULL);
	insertSSMListCellToStartList(res, newCell);

	return res;
}

// This function gets R and L SingleSourceMovesLists(moves) from the helper, and returns the true if R is better, else returns L.
bool isRightListBetter(SingleSourceMovesList* right_list, SingleSourceMovesList* left_list, Player p)
{
	int right_captures, left_captures;

	if (left_list->tail == NULL)
		return true;

	if (right_list->tail == NULL)
		return false;

	right_captures = right_list->tail->captures;
	left_captures = left_list->tail->captures;
	
	if (right_captures == left_captures)
		return true;
	return right_captures > left_captures;
}


// Q3

// This function finds all possible player moves and returns a list of each source best possible move.
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player)
{
	int i, j;
	SingleSourceMovesTree* source_moves_tree;
	SingleSourceMovesList* source_optimal_move;
	MultipleSourceMovesList* res;
	checkersPos* pSrc;

	res = (MultipleSourceMovesList*)myMalloc(sizeof(MultipleSourceMovesList), "MultipleSourceMovesList");

	makeEmptyMultipleSourceMovesList(res);

	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			if (board[i][j] == player)
			{
				pSrc = (checkersPos*)myMalloc(sizeof(checkersPos), "checkersPos");

				pSrc->row = ROW_INT_TO_CHAR(i);
				pSrc->col = COL_INT_TO_CHAR(j);
				source_moves_tree = FindSingleSourceMoves(board, pSrc);
				source_optimal_move = FindSingleSourceOptimalMove(source_moves_tree);
				insertDataToEndOfMultipleSourceMovesList(res, source_optimal_move);
				freeSingleSourceMoveTree(source_moves_tree);
			}

	return res;
}


// Q4

// This function makes a player turn by the instrations that was given.
// It changes the board according to the move that has made.
void Turn(Board board, Player player)
{
	SingleSourceMovesList* player_best_move;
	SingleSourceMovesTree* player_best_move_tree;
	checkersPos* pSrc;

	player_best_move = getBestListForPlayer(board, player);

	if (player_best_move != NULL)
	{
		pSrc = (checkersPos*)myMalloc(sizeof(checkersPos), "checkersPos");

		pSrc->col = player_best_move->head->position->col;
		pSrc->row = player_best_move->head->position->row;

		player_best_move_tree = FindSingleSourceMoves(board, pSrc);
		updateBoard(board, player_best_move_tree->source, player_best_move->tail->position);
		freeSingleSourceMoveTree(player_best_move_tree);
	}
	freeSingleSourceMovesList(player_best_move);
}

// This function finds the best turn that a player can do with a single source.
SingleSourceMovesList* getBestListForPlayer(Board board, Player player)
{
	int max_captures, curr_source_best_captures;

	MultipleSourceMovesList* player_best_moves_list;
	MultipleSourceMovesListCell* source_best_move, * player_best_move;
	SingleSourceMovesList* res;

	player_best_moves_list = FindAllPossiblePlayerMoves(board, player);
	max_captures = NO_MOVES_AT_ALL;
	source_best_move = player_best_moves_list->head;
	player_best_move = NULL;

	while (source_best_move != NULL)
	{
		curr_source_best_captures = source_best_move->single_source_moves_list->tail->captures;
		if (isCurrSourceTheBest(player, source_best_move, player_best_move, max_captures, curr_source_best_captures))
		{
			max_captures = curr_source_best_captures;
			player_best_move = source_best_move;
		}
		source_best_move = source_best_move->next;
	}

	if (player_best_move == NULL)
		return NULL;

	res = player_best_move->single_source_moves_list;

	freeWithoutBestMovesList(player_best_moves_list, player_best_move);

	return res;
}

// This function checks if the current source has better move than the best source so far.
// If is does, it returns true.
bool isCurrSourceTheBest(Player player, MultipleSourceMovesListCell* source_best_move,
	MultipleSourceMovesListCell* player_best_move, int max_captures, int curr_source_best_captures)
{
	char source_col, source_row, player_col, player_row;

	if (!hasValidMove(source_best_move) || curr_source_best_captures < max_captures)
		return false;

	else if (player_best_move == NULL || curr_source_best_captures > max_captures)
		return true;

	source_col = source_best_move->single_source_moves_list->head->position->col;
	source_row = source_best_move->single_source_moves_list->head->position->row;

	player_col = player_best_move->single_source_moves_list->head->position->col;
	player_row = player_best_move->single_source_moves_list->head->position->row;

	if (player == FIRST_PLAYER && ((source_row > player_row) || (source_row == player_row && source_col > player_col)))
		return true;
	else if (player == SECOND_PLAYER && ((source_row < player_row) || (source_row == player_row && source_col < player_col)))
		return true;
	return false;
}

// This function checks if the source can move or it stuck. If it's stuck, it returns true. 
bool hasValidMove(MultipleSourceMovesListCell* source_best_move)
{
	if (source_best_move->single_source_moves_list->head == source_best_move->single_source_moves_list->tail)
		return false;
	return true;
}

// This function updates the board by finding the best position in the source's tree and pulling out the updated board inside the tree node.
void updateBoard(Board board, SingleSourceMovesTreeNode* source, checkersPos* pSrc)
{
	if (source == NULL)
		return;

	updateBoard(board, source->next_moves[LEFT], pSrc);
	updateBoard(board, source->next_moves[RIGHT], pSrc);

	if (source->pos->col == pSrc->col && source->pos->row == pSrc->row)
		memcpy(board, source->board, sizeof(char) * BOARD_SIZE * BOARD_SIZE);
}


// Q5

// This function is the main game function.
// It gets a board and a startins player, and starts simulating the game, saves statistics, and print the simulation.
void PlayGame(Board board, Player starting_player)
{
	int pNum = 0;
	bool hasWinner = false;
	SingleSourceMovesList* bestList;
	STATS players_stats[2] = { 0 };
	
	players_stats[pNum].player = starting_player;

	if (starting_player == FIRST_PLAYER)
		players_stats[pNum + 1].player = SECOND_PLAYER;
	else
		players_stats[pNum + 1].player = FIRST_PLAYER;

	printBoard(board);

	while (!hasWinner)
	{
		bestList = getBestListForPlayer(board, players_stats[pNum].player);
		Turn(board, players_stats[pNum].player);
		addStats(players_stats, pNum, bestList);

		printf("%c's turn:\n", players_stats[pNum].player);
		printf("%c%c->%c%c\n", bestList->head->position->row, bestList->head->position->col,
			bestList->tail->position->row, bestList->tail->position->col);
		printBoard(board);

		freeSingleSourceMovesList(bestList);

		if (checkWinner(board, players_stats))
		{
			hasWinner = true;
			break;
		}

		SWITCH_PLAYER(pNum);
	}

	printStats(players_stats, pNum);
}

// This function updates the statistics for a a player, for each move performed.
void addStats(STATS* players_stats, int pNum, SingleSourceMovesList* lst)
{
	int captures = lst->tail->captures;

	players_stats[pNum].total_moves++;
	players_stats[pNum].total_captures += captures;

	if (captures > players_stats[pNum].best_capture)
		players_stats[pNum].best_capture = captures;
}

// This function prints the current board.
void printBoard(Board board)
{
	int i, j;
	int inRow = 0, indCol = 0;
	for (i = 0; i < BOARD_PRINT_SIZE; i++)
	{
		if (i % 2 == 0)
			printf("+-+-+-+-+-+-+-+-+-+");
		else if (i == 1)
			printf("+ |1|2|3|4|5|6|7|8|");
		else
		{
			indCol = 0;
			for (j = 0; j < BOARD_PRINT_SIZE; j++)
			{
				if (j % 2 == 0)
					printf("|");
				else if (j == 1)
					printf("%c", 'A' + inRow);
				else
				{
					if (board[inRow][indCol] == FIRST_PLAYER || board[inRow][indCol] == SECOND_PLAYER)
						printf("%c", board[inRow][indCol]);
					else
						printf(" ");
					indCol++;
				}
			}
			inRow++;
		}
		printf("\n");
	}
	printf("\n");
}

// This function returns true if the board received is in status of a "finished game". else it returns false.
bool checkWinner(Board board, STATS* players_stats)
{
	int col;
	// Check first and last row, to see if a player has reached to the end
	for (col = 0; col < BOARD_SIZE; col++)
		if (board[FIRST_ROW][col] == SECOND_PLAYER || board[LAST_ROW][col] == FIRST_PLAYER)
			return true;

	// Check players' capture counters, to see if other player's pieces are no more
	if (players_stats[LEFT].total_captures == MAX_CAPTURES || players_stats[RIGHT].total_captures == MAX_CAPTURES)
		return true;

	return false;
}

// This function gets the statistics of both players and name of winner, and prints the statistics of the game.
void printStats(STATS* players_stats, int pNum)
{
	int best_capturer_ind;

	printf("%c wins!\n", players_stats[pNum].player);
	printf("%c performed %d moves.\n", players_stats[pNum].player, players_stats[pNum].total_moves);

	if (players_stats[LEFT].best_capture > players_stats[RIGHT].best_capture)
		best_capturer_ind = 0;
	else
		best_capturer_ind = 1;

	printf("%c performed the highest number of captures in a single move - %d\n",
		players_stats[best_capturer_ind].player, players_stats[best_capturer_ind].best_capture);
}