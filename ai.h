#ifndef AI_H
#define AI_H

#include <ctime>
#include <cstdlib>
#include <sys/time.h>

#include "board.h"
#include "hash.h"

#define TREEMAX 128
#define TABLESIZE 100000


class AI {
public:
	AI(int time);
	
	void makeMove(Board &b);

private:
	int timeLimit;
	int tree[16][TREEMAX];
	int treesize[16];
	int nodes,nodes2;
	hashTable *table;
	
	int alphabeta(Board board, int depth, int alpha, int beta, int color, timeval begin);
	
	int fillTable(Board board, int depth, int alpha, int beta, int color);
	
	int fillTree(Board board, int depth, int alpha, int beta, int color);
	
	int displayTree(Board board, int depth, int color = 1);
};


#endif
