#ifndef _ListHandler
#define _ListHandler

typedef struct checkersPos
{
	char row, col;
} checkersPos;

// Q2 Structs
typedef struct SingleSourceMovesListCell
{
	checkersPos* position;
	unsigned short captures;
	struct SingleSourceMovesListCell* next;
} SingleSourceMovesListCell;

typedef struct SingleSourceMovesList
{
	SingleSourceMovesListCell* head;
	SingleSourceMovesListCell* tail;
} SingleSourceMovesList;

// Q3 Structs
typedef struct multipleSourceMovesListCell {
	SingleSourceMovesList* single_source_moves_list;
	struct multipleSourceMovesListCell* next;
} MultipleSourceMovesListCell;

typedef struct multipleSourceMovesList {
	MultipleSourceMovesListCell* head;
	MultipleSourceMovesListCell* tail;
} MultipleSourceMovesList;

void* myMalloc(int size, char* desc);
void myFree(void* p, char* desc);

// List Functions

// Q2

void makeEmptySSMList(SingleSourceMovesList* lst);
bool isEmptyList(SingleSourceMovesList* lst);
SingleSourceMovesListCell* createNewSSMListCell(checkersPos* position, unsigned short captures, SingleSourceMovesListCell* next);
void insertSSMListCellToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* cell);
void insertSSMListCellToEndList(SingleSourceMovesList* lst, SingleSourceMovesListCell* cell);
void freeSingleSourceMovesList(SingleSourceMovesList* lst);

// Q3

void insertCellToEndOfMultipleSourceMovesList(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* tail);
void insertDataToEndOfMultipleSourceMovesList(MultipleSourceMovesList* lst, SingleSourceMovesList* data);
MultipleSourceMovesListCell* createNewSingleSourceMovesList(SingleSourceMovesList* data, MultipleSourceMovesListCell* next);
void makeEmptyMultipleSourceMovesList(MultipleSourceMovesList* lst);
bool isEmptyMultipleSourceMovesList(MultipleSourceMovesList* lst);

// Q4

void freeWithoutBestMovesList(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* player_best_moves_lst);

#endif // !ListHandler
	