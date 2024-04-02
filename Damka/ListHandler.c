#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ListHandler.h"

// Allocation and free Lists and Trees

//int MallocIndex[10] = { 0 };

void* myMalloc(int size, char* desc)
{
	void* p = malloc(size);

	if (!p)
	{
		printf("Memory Allocation Failure!!!");
		exit(1);
	}

	//if (strcmp(desc, "checkersPos") == 0)
	//{
	//	MallocIndex[0] ++;
	//	printf("%d M: %s %p\n", MallocIndex[0], desc, p);
	//}
	//else if (strcmp(desc, "MultipleSourceMovesListCell") == 0)
	//{
	//	MallocIndex[1] ++;
	//	printf("%d M: %s %p\n", MallocIndex[1], desc, p);
	//}
	//else if (strcmp(desc, "MultipleSourceMovesList") == 0)
	//{
	//	MallocIndex[2] ++;
	//	printf("%d M: %s %p\n", MallocIndex[2], desc, p);
	//}
	//else if (strcmp(desc, "SingleSourceMovesTreeNode") == 0)
	//{
	//	MallocIndex[3] ++;
	//	printf("%d M: %s %p\n", MallocIndex[3], desc, p);
	//}
	//else if (strcmp(desc, "SingleSourceMovesTree") == 0)
	//{
	//	MallocIndex[4] ++;
	//	printf("%d M: %s %p\n", MallocIndex[4], desc, p);
	//}
	//else if (strcmp(desc, "SingleSourceMovesList") == 0)
	//{
	//	MallocIndex[5] ++;
	//	printf("%d M: %s %p\n", MallocIndex[5], desc, p);
	//}
	//else if (strcmp(desc, "SingleSourceMovesListCell") == 0)
	//{
	//	MallocIndex[6] ++;
	//	printf("%d M: %s %p\n", MallocIndex[6], desc, p);
	//}
	//else
	//	printf("????????????????????????????????????????????????????????????????????????????????????????????????????????\n");

	return p;
}

void myFree(void* p, char* desc)
{
	//if (strcmp(desc, "checkersPos") == 0)
	//{
	//	MallocIndex[0] --;
	//	printf("%d F: %s %p\n", MallocIndex[0], desc, p);
	//}
	//else if (strcmp(desc, "MultipleSourceMovesListCell") == 0)
	//{
	//	MallocIndex[1] --;
	//	printf("%d F: %s %p\n", MallocIndex[1], desc, p);
	//}
	//else if (strcmp(desc, "MultipleSourceMovesList") == 0)
	//{
	//	MallocIndex[2] --;
	//	printf("%d F: %s %p\n", MallocIndex[2], desc, p);
	//}
	//else if (strcmp(desc, "SingleSourceMovesTreeNode") == 0)
	//{
	//	MallocIndex[3] --;
	//	printf("%d F: %s %p\n", MallocIndex[3], desc, p);
	//}
	//else if (strcmp(desc, "SingleSourceMovesTree") == 0)
	//{
	//	MallocIndex[4] --;
	//	printf("%d F: %s %p\n", MallocIndex[4], desc, p);
	//}
	//else if (strcmp(desc, "SingleSourceMovesList") == 0)
	//{
	//	MallocIndex[5] --;
	//	printf("%d F: %s %p\n", MallocIndex[5], desc, p);
	//}
	//else if (strcmp(desc, "SingleSourceMovesListCell") == 0)
	//{
	//	MallocIndex[6] --;
	//	printf("%d F: %s %p\n", MallocIndex[6], desc, p);
	//}
	//else
	//	printf("********************************************************************************************************\n");
	
	free(p);
}


// Lists Functions

// Q2

// This function sets SingleSourceMovesList's head and tail to null.
void makeEmptySSMList(SingleSourceMovesList* lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}

// This function checks if a SingleSourceMovesList is empty.
bool isEmptyList(SingleSourceMovesList* lst)
{
	if (lst->head == NULL && lst->tail == NULL)
		return true;
	else
		return false;
}

// This function creates a new SingleSourceMovesListCell, and returns it.
SingleSourceMovesListCell* createNewSSMListCell(checkersPos* position, unsigned short captures, SingleSourceMovesListCell* next)
{
	SingleSourceMovesListCell* newNode;
	newNode = (SingleSourceMovesListCell*)myMalloc(sizeof(SingleSourceMovesListCell), "SingleSourceMovesListCell");
	newNode->position = (checkersPos*)myMalloc(sizeof(checkersPos), "checkersPos");

	newNode->position->row = position->row;
	newNode->position->col = position->col;
	newNode->captures = captures;
	newNode->next = next;

	return newNode;
}

// This function gets a SingleSourceMovesList, and inserts to the beggining of it a SingleSourceMovesListCell
void insertSSMListCellToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* cell)
{
	if (isEmptyList(lst) == true)
	{
		cell->next = NULL;
		lst->head = lst->tail = cell;
	}
	else
	{
		cell->next = lst->head;
		lst->head = cell;
	}

}

// This function gets a SingleSourceMovesList, and inserts to the endding of it a SingleSourceMovesListCell
void insertSSMListCellToEndList(SingleSourceMovesList* lst, SingleSourceMovesListCell* cell)
{
	if (isEmptyList(lst) == true)
	{
		cell->next = NULL;
		lst->head = lst->tail = cell;
	}
	else
	{
		lst->tail->next = cell;
		cell->next = NULL;
	}
}


// Q3

void freeSingleSourceMovesList(SingleSourceMovesList* lst)
{
	SingleSourceMovesListCell* current,* next;
	current = lst->head;
	while (current != NULL)
	{
		next = current->next;
		myFree(current->position, "checkersPos");
		myFree(current, "SingleSourceMovesListCell");
		current = next;
	}
	myFree(lst, "SingleSourceMovesList");
}

void insertCellToEndOfMultipleSourceMovesList(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* tail)
{
	if (isEmptyMultipleSourceMovesList(lst) == true)
		lst->head = lst->tail = tail;
	else
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
}

void insertDataToEndOfMultipleSourceMovesList(MultipleSourceMovesList* lst, SingleSourceMovesList* data)
{
	MultipleSourceMovesListCell* newTail;
	newTail = createNewSingleSourceMovesList(data, NULL);
	insertCellToEndOfMultipleSourceMovesList(lst, newTail);
}

MultipleSourceMovesListCell* createNewSingleSourceMovesList(SingleSourceMovesList* data, MultipleSourceMovesListCell* next)
{
	MultipleSourceMovesListCell* res;
	res = (MultipleSourceMovesListCell*)myMalloc(sizeof(MultipleSourceMovesListCell), "MultipleSourceMovesListCell");

	res->single_source_moves_list = data;
	res->next = next;
	return res;
}

void makeEmptyMultipleSourceMovesList(MultipleSourceMovesList* lst)
{
	lst->head = lst->tail = NULL;
}

bool isEmptyMultipleSourceMovesList(MultipleSourceMovesList* lst)
{
	if (lst->head == NULL)
		return true;
	return false;
}

// Q4

void freeWithoutBestMovesList(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* player_best_moves_lst)
{
	MultipleSourceMovesListCell* single_source_best_moves_lst ,* next;
	single_source_best_moves_lst = lst->head;

	while (single_source_best_moves_lst != NULL)
	{
		if (single_source_best_moves_lst != player_best_moves_lst)
			freeSingleSourceMovesList(single_source_best_moves_lst->single_source_moves_list);
		next = single_source_best_moves_lst->next;
		myFree(single_source_best_moves_lst, "MultipleSourceMovesListCell");
		single_source_best_moves_lst = next;
	}
	myFree(lst, "MultipleSourceMovesList");
}