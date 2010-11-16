#include "ai.h"



AI::AI(int time) {
	timeLimit = time;
	timeval t;
	gettimeofday(&t,NULL);
	table = new hashTable(TABLESIZE);
}

inline double difftime(timeval end, timeval begin) {
	return (end.tv_sec - begin.tv_sec) + 
	(end.tv_usec - begin.tv_usec)/1000000.0;
}

void clockrun(int i) {
	timeval begin, end;
	gettimeofday(&begin,NULL);
	end = begin;
	while(difftime(end, begin) < (i - .0001)) {
		gettimeofday(&end,NULL);
	}
}

int minimax1(Board board,int depth, int color);
int minimax2(Board board,int depth, int color);
int alphabeta(Board board, int depth, int alpha, int beta, int color);

void AI::makeMove(Board &board) {
	int color = board.getTurn();
	//displayTree(board, 3, color);
	timeval begin, end;
	std::cout << "I,Emperor Fred the Great, am moving" << std::endl;
	gettimeofday(&begin,NULL);
	//fillTable(board, 3, -999999, 999999, color);
	srand(time(0));
	char *moves = board.giveMove();
	char *best, *partial;
	Board child;
	best = moves;
	int t = 0;
	if(!(*(moves + MAXJUMPS))) {//no second move
		best = moves;
	}
	else {
		for(t = 1; t < 40; t++) {
			int max = -999999;
			for(int i = 0; *(moves + i*MAXJUMPS); i++) {
				gettimeofday(&end,NULL);
				child = board;
				child.doMove(moves + i*MAXJUMPS);
				int retval;
				retval = alphabeta(child,t, -999999, 999999, color, begin);
				//retval = minimax2(child, t, color);
				if(retval > max) {
					max = retval;
					partial = moves + i*MAXJUMPS;
				}
				else if(retval == max && rand()&1) {
					max = retval;
					partial = moves + i*MAXJUMPS;
				}
				if(difftime(end,begin) > timeLimit*.9) {
					partial = best;
					break;
				}
			}
			best = partial;
			if(difftime(end,begin) > timeLimit*.9) {
				partial = best;
				break;
			}
			/*for(int i = 0; i < 10; i++) {
				std::cout << (int)best[i] << ' ';
			}
			std::cout << std::endl;*/
		}
	}
	gettimeofday(&end,NULL);
	board.doMove(best);
	std::cout << "I,Emperor Fred the Great, took " << difftime(end,begin) << " seconds" << std::endl;
	std::cout << "I,Emperor Fred the Great, got to depth " << t-1 << std::endl;
	clockrun(1);
}

inline int max(int a, int b) {
	if(a > b)
		return a;
	else
		return b;
}
inline int min(int a, int b) {
	if(a < b)
		return a;
	else
		return b;
}

int minimax1(Board board,int depth, int color) {
	if (board.Uyen() || depth == 0)
		return board.getValue(color);
	
	int alpha = -999999;
	char *moves = board.giveMove();
	for(int i = 0; *(moves + i*MAXJUMPS); i++) {
		Board temp = board;
		temp.doMove(moves + i*MAXJUMPS);
		alpha = max(alpha, minimax2(temp, depth-1, color));
	}
	return alpha;
}

int minimax2(Board board,int depth, int color) {
	if (board.Uyen() || depth == 0)
		return board.getValue(color);
	
	int alpha = -999999;
	char *moves = board.giveMove();
	for(int i = 0; *(moves + i*MAXJUMPS); i++) {
		Board temp = board;
		temp.doMove(moves + i*MAXJUMPS);
		alpha = min(alpha, minimax1(temp, depth-1, color));
	}
	return alpha;
}

int AI::alphabeta(Board board, int depth, int alpha, int beta, int color, timeval begin) {
	timeval end;
	gettimeofday(&end, NULL);
	if(difftime(end,begin) > timeLimit*.9) {
		return color?-9999999:9999999;
	}
	if(board.Uyen() || depth == 0) {
		int p =board.getValue(color);
		if(p > 40000) {
			p+= depth&1 ? depth : -depth;
		}
		return p;
	}
	char *moves = board.giveMove();
	for (int i = 0; *(moves + i*MAXJUMPS); i++) {
		Board temp = board;
		temp.doMove(moves + i*MAXJUMPS);
		alpha = max(alpha,-alphabeta(temp, depth-1, -beta, -alpha, 1^color, begin));
		if(beta <= alpha)
			break;
	}
	return alpha;
}

int AI::fillTable(Board board, int depth, int alpha, int beta, int color) {
	timeval end;
	if(board.Uyen() || depth == 0) {
		table->insert(*(board.toString()));
		return board.getValue(color);
	}
	char *moves = board.giveMove();
	for (int i = 0; *(moves + i*MAXJUMPS); i++) {
		Board temp = board;
		temp.doMove(moves + i*MAXJUMPS);
		alpha = max(alpha, -fillTable(temp, depth-1, -beta, -alpha, 1^color));
		if(beta <= alpha)
			break;
	}
	table->insert(*(board.toString()));
	return alpha;
}

int AI::fillTree(Board board, int depth, int alpha, int beta, int color) {
	if(board.Uyen() || depth == 0) {
		tree[depth][treesize[depth]] = board.getValue(color);
		treesize[depth]++;
		return board.getValue(color);
	}
	char *moves = board.giveMove();
	for (int i = 0; *(moves + i*MAXJUMPS); i++) {
		Board temp = board;
		temp.doMove(moves + i*MAXJUMPS);
		alpha = max(alpha, -fillTree(temp, depth-1, -beta, -alpha, 1^color));
		if(beta <= alpha)
			break;
	}
	tree[depth][treesize[depth]] = alpha;
	treesize[depth]++;
	return alpha;
}


int AI::displayTree(Board board, int depth, int color) {
	memset(treesize, 0, sizeof(int)*16);
	fillTree(board, depth+1, -9999999, 9999999, color);
	for(int i = 0; i < depth+1; i++) {
		for(int ii = 0; ii < treesize[i]; ii++) {
			std::cout << tree[i][ii] << ' ';
		}
		std::cout << std::endl;
	}
}

