#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#define EMPTY 0
#define WHITE 0
#define BLACK 1
#define WNORM 1
#define BNORM 2
#define WKING 3
#define BKING 4
#define ENORMSQR 0
#define ERGTWALL 1
#define ELFTWALL 2
#define ONORMSQR 3
#define ORGTWALL 4
#define OLFTWALL 5
#define DOWNRIGHT 3
#define DOWNLEFT 4
#define MAXJUMPS 16

class Board {
	public:
	
	Board(std::string setup = "bbbbbbbbbbbb        wwwwwwwwwwww", int t = BLACK);
	
	void doMove(char *begin);
	
	void fillMoves();
	
	int getTurn();
	
	int getWhiteValue();
	
	int getBlackValue();
	
	std::string *toString();
	
	char *giveMove();
	
	void display();
	
	bool Uyen() {
		if(!moves[0][0] || !numBlack || !numWhite)
			return true;
		else
			return false;
	}
	
	void displayMoves();
	
	int getValue(int color) {
		if(color == WHITE) {
			return whiteValue;
		}
		else
			return blackValue;
	}
	
	private:
	
	int turn;
	bool jump;
	char moves[32][MAXJUMPS];
	char places[32];
	int jumpcounter;
	int numBlack;
	int numWhite;
	int blackValue;
	int whiteValue;
	int lastMove;
	
	int getSqrType(int p) {
		if(!((p-4)&7)) {
			return ERGTWALL;//only left
		}
		else if(!((p-5)&7)) {
			return OLFTWALL;//only left
		}
		if(((p-1)/4)&1) {
			return ONORMSQR;//both
		}
		else {
			return ENORMSQR;//both
		}
	}
	
	void blackJumpAdd(char i, int j);
		
	void blackKingJumpAdd(char i, int j);

	void whiteJumpAdd(char i, int j);
	
	void whiteKingJumpAdd(char i, int j);

	
	void evaluate();
	
	
};


#endif

