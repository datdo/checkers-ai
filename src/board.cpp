#include "board.h"

#include <cstdio>
#include <cstdlib>

#define REDTEXT	"\x1b[31m"
#define NORM	"\x1b[0m"
#define REDBACK	"\x1b[41m"
#define WHTBACK	"\x1b[1m\x1b[47m"
#define BLKBACK	"\x1b[40m"
#define BUFBACK	"\x1b[47m"
#define GRNBACK	"\x1b[42m"
#define CLEAR	"\x1b[2J\x1b[1;1f"

Board::Board(std::string setup, int t) {
	turn = t;
	numBlack = numWhite = 0;
	jump = false;
	jumpcounter = 0;
	if(setup.size() < 32) {
		std::cerr << "Error: Invalid String!\nDefault board created" << std::endl;
		setup = "bbbbbbbbbbbb        wwwwwwwwwwww";
	}
	for(int r = 0; r < 32; r++) {
		switch(setup[r]) {
			case 'b':
				places[r] = BNORM;
				numBlack++;
				break;
			case 'B':
				places[r] = BKING;
				numBlack++;
				break;
			case 'w':
				places[r] = WNORM;
				numWhite++;
				break;
			case 'W':
				places[r] = WKING;
				numWhite++;
				break;
			default:
				places[r] = EMPTY;
				break;
		}
	}
	blackValue = 0;
	evaluate();
	fillMoves();
	lastMove = 0;
}


char *Board::giveMove() {
	return &moves[0][0];
}

void Board::doMove(char *attempt) {
	int i=0;
	int begin = *attempt;
	int end = attempt[1];
	while(moves[i][0]) {
		for(int ii = 0; moves[i][ii]; ii++) {
			if(moves[i][ii] != attempt[ii]) {
				goto stuff;
			}
		}//strcmp
		if(0){
stuff:		i++;
			continue;
		}
		if(jumpcounter) {
		attempt = &(moves[i][0]);
			while(1) {
				int temp = places[*attempt -1];
				if(temp&1)
					numBlack--;
				else
					numWhite--;
			
				places[begin - 1] = EMPTY;
				if(((begin-1)/4)&1)//odd
					places[(begin+end)/2 - 1] = EMPTY;
				else
					places[(begin+end)/2] = EMPTY;
			
				places[end - 1] = temp;
				if((end > 28 && places[end -1] == BNORM)
				|| (end < 5 && places[end -1] == WNORM))
					places[end -1] += 2;//KING ME
			
				if(attempt[2]) {
					attempt++;
					begin = end;
					end = attempt[1];
				}
				else {
					jumpcounter = 0;
					turn ^= 1;
					fillMoves();
					evaluate();
					return;
				}
			}
		}
		
		int temp = places[begin -1];
		places[begin - 1] = EMPTY;
		places[end - 1] = temp;
		if((end > 28 && places[end -1] == BNORM) ||
		(end < 5 && places[end -1] == WNORM))
			places[end -1] += 2;//KING ME
		turn ^= 1;
		fillMoves();
		evaluate();
		return;
			
	}
}


void Board::fillMoves() {
	int j = 0;
	int z = 0;
	jumpcounter = 0;
	for(int i = 1; i < 33; i++) {
		if(turn == BLACK) {
			if(places[i-1] == BNORM) {
				blackJumpAdd(i,0);
				if(jumpcounter)
					continue;
				switch(getSqrType(i)) {
					case ENORMSQR:
						if(!places[i+5-1]) {
							moves[j][0] = i;
							moves[j][1] = i+5;
							moves[j][2] = 0;
							j++;
						}
					case ERGTWALL:
						if(!places[i+4-1]) {
							moves[j][0] = i;
							moves[j][1] = i+4;
							moves[j][2] = 0;
							j++;
						}
						break;
					case ONORMSQR:
						if(!places[i+3-1]) {
							moves[j][0] = i;
							moves[j][1] = i+3;
							moves[j][2] = 0;
							j++;
						}
					case OLFTWALL:
						if(!places[i+4-1]) {
							moves[j][0] = i;
							moves[j][1] = i+4;
							moves[j][2] = 0;
							j++;
						}
						break;
				}
			}
			else if(places[i-1] == BKING) {
				blackKingJumpAdd(i,0);
				if(jumpcounter)
					continue;
				switch(getSqrType(i)) {
					case ENORMSQR:
						if(i < 29 && !places[i+5-1]) {
							moves[j][0] = i;
							moves[j][1] = i+5;
							moves[j][2] = 0;
							j++;
						}
						if(i > 4 && !places[i-3-1]) {
							moves[j][0] = i;
							moves[j][1] = i-3;
							moves[j][2] = 0;
							j++;
						}
					case ERGTWALL:
						if(i > 4 && !places[i-4-1]) {
							moves[j][0] = i;
							moves[j][1] = i-4;
							moves[j][2] = 0;
							j++;
						}
						if(i < 29 && !places[i+4-1]) {
							moves[j][0] = i;
							moves[j][1] = i+4;
							moves[j][2] = 0;
							j++;
						}
						break;
					case ONORMSQR:
						if((i < 29) && !places[i+3-1]) {
							moves[j][0] = i;
							moves[j][1] = i+3;
							moves[j][2] = 0;
							j++;
						}
						if((i > 4) && !places[i-5-1]) {
							moves[j][0] = i;
							moves[j][1] = i-5;
							moves[j][2] = 0;
							j++;
						}
					case OLFTWALL:
						if((i < 29) && !places[i+4-1]) {
							moves[j][0] = i;
							moves[j][1] = i+4;
							moves[j][2] = 0;
							j++;
						}
						if((i > 4) && !places[i-4-1]) {
							moves[j][0] = i;
							moves[j][1] = i-4;
							moves[j][2] = 0;
							j++;
						}
						break;
				}
			}
		}
		else {
			if(places[i-1] == WNORM) {
				whiteJumpAdd(i,0);
				if(jumpcounter)
					continue;
				switch(getSqrType(i)) {
					case ENORMSQR:
						if(!places[i-3-1]) {
							moves[j][0] = i;
							moves[j][1] = i-3;
							moves[j][2] = 0;
							j++;
						}
					case ERGTWALL:
						if(!places[i-4-1]) {
							moves[j][0] = i;
							moves[j][1] = i-4;
							moves[j][2] = 0;
							j++;
						}
						break;
					case ONORMSQR:
						if(!places[i-5-1]) {
							moves[j][0] = i;
							moves[j][1] = i-5;
							moves[j][2] = 0;
							j++;
						}
					case OLFTWALL:
						if(!places[i-4-1]) {
							moves[j][0] = i;
							moves[j][1] = i-4;
							moves[j][2] = 0;
							j++;
						}
						break;
				}
			}
			else if(places[i-1] == WKING) {
				whiteKingJumpAdd(i,0);
				if(jumpcounter)
					continue;
				switch(getSqrType(i)) {
					case ENORMSQR:
						if(i < 29 && !places[i+5-1]) {
							moves[j][0] = i;
							moves[j][1] = i+5;
							moves[j][2] = 0;
							j++;
						}
						if(i > 4 && !places[i-3-1]) {
							moves[j][0] = i;
							moves[j][1] = i-3;
							moves[j][2] = 0;
							j++;
						}
					case ERGTWALL:
						if(i > 4 && !places[i-4-1]) {
							moves[j][0] = i;
							moves[j][1] = i-4;
							moves[j][2] = 0;
							j++;
						}
						if(i < 29 && !places[i+4-1]) {
							moves[j][0] = i;
							moves[j][1] = i+4;
							moves[j][2] = 0;
							j++;
						}
						break;
					case ONORMSQR:
						if((i < 29) && !places[i+3-1]) {
							moves[j][0] = i;
							moves[j][1] = i+3;
							moves[j][2] = 0;
							j++;
						}
						if((i > 4) && !places[i-5-1]) {
							moves[j][0] = i;
							moves[j][1] = i-5;
							moves[j][2] = 0;
							j++;
						}
					case OLFTWALL:
						if((i < 29) && !places[i+4-1]) {
							moves[j][0] = i;
							moves[j][1] = i+4;
							moves[j][2] = 0;
							j++;
						}
						if((i > 4) && !places[i-4-1]) {
							moves[j][0] = i;
							moves[j][1] = i-4;
							moves[j][2] = 0;
							j++;
						}
						break;
				}
			}
		} 
	}
	
	if(jumpcounter)
		moves[jumpcounter][0] = 0;
	else
		moves[j][0] = 0;
}

int Board::getTurn() {
	return turn;
}

void Board::evaluate() {
	blackValue = 0;
	
	for(int r = 0; r < 32; r++) {
		switch(places[r]) {
			case BKING:
				blackValue += 140;
				break;
			case BNORM:
				blackValue += 100;
				if(r < 4) {
					blackValue += 30;
				}
				break;
			case WKING:
				blackValue -= 140;
				break;
			case WNORM:
				blackValue -= 100;
				if(r > 27) {
					blackValue -= 30;
				}
				break;
			default:
				break;
		}
		
		
	}
	
	if(Uyen()) {
		if(getTurn()) {
			blackValue += -40000;
		}
		else {
			blackValue += 40000;
		}
	}
	
	else if(numBlack < 3 && numBlack < numWhite) {
		if(places[0] == BKING || places[4] == BKING) {
			blackValue +=100;
		}
		if(places[27] == BKING || places[31] == BKING) {
			blackValue +=100;
		}
	}
	else if(numWhite < 3 && numBlack > numWhite) {
		if(places[0] == WKING || places[4] == WKING) {
			blackValue -=100;
		}
		if(places[27] == WKING || places[31] == WKING) {
			blackValue -=100;
		}
	}
	
	whiteValue = -blackValue;
}

void Board::display() {
	std::cout << CLEAR;
	int p;
	for(int r = 0; r < 8; r++) {
		for(int c = 0; c < 8; c++) {
			p = r*8 + c;
			if(p&1 ^ (r&1)) {
				std::cout << GRNBACK;
				if		(places[p/2] == WNORM) printf ("  %s%2d%s  ", GRNBACK, p/2 +1, GRNBACK);
				else if	(places[p/2] == WKING) printf ("  %s%2d%s  ", GRNBACK, p/2 +1, GRNBACK);
				else if	(places[p/2] == BNORM) printf ("  %s%2d%s  ", GRNBACK, p/2 +1, GRNBACK);
				else if	(places[p/2] == BKING) printf ("  %s%2d%s  ", GRNBACK, p/2 +1, GRNBACK);
				else printf ("  %2d  ", p/2 +1);
				std::cout << NORM;
			}
			else {
				printf ("%s      %s", BUFBACK, NORM);
			}
		}
		std::cout << std::endl;
		for(int c = 0; c < 8; c++) {
			p = r*8 + c;
			if(p&1 ^ (r&1)) {
				std::cout << GRNBACK;
				if		(places[p/2] == WNORM) printf ("  %s  %s  ", WHTBACK, GRNBACK);
				else if	(places[p/2] == WKING) printf ("  %sKK%s  ", WHTBACK, GRNBACK);
				else if	(places[p/2] == BNORM) printf ("  %s  %s  ", REDBACK, GRNBACK);
				else if	(places[p/2] == BKING) printf ("  %sKK%s  ", REDBACK, GRNBACK);
				else printf ("      ");
				std::cout << NORM;
			}
			else {
				printf ("%s      %s", BUFBACK, NORM);
			}
		}
		std::cout << std::endl;
		for(int c = 0; c < 8; c++) {
			p = r*8 + c;
			if(p&1 ^ (r&1)) {
				std::cout << GRNBACK;
				if		(places[p/2] == WNORM) printf ("  %s  %s  ", GRNBACK, GRNBACK);
				else if	(places[p/2] == WKING) printf ("  %s  %s  ", GRNBACK, GRNBACK);
				else if	(places[p/2] == BNORM) printf ("  %s  %s  ", GRNBACK, GRNBACK);
				else if	(places[p/2] == BKING) printf ("  %s  %s  ", GRNBACK, GRNBACK);
				else printf ("      ");
				std::cout << NORM;
			}
			else {
				printf ("%s      %s", BUFBACK, NORM);
			}
		}
		std::cout << std::endl;
	}
}

void Board::displayMoves() {
	for(int i = 0; moves[i][0]; i++) {
		std::cout << (int)moves[i][0];
		for(int ii = 1; moves[i][ii]; ii++) {
			std::cout << " => " << (int)moves[i][ii];
		}
		std::cout << std::endl;
	}
}

void Board::blackJumpAdd(char i, int j) {
	int retval = 0;
	if(i < 25) {
		switch(getSqrType(i)) {
			case ENORMSQR:
				if(places[i+5-1]&1 && !places[i+9-1]) {
					++retval;
				}
			case ERGTWALL:
				if(places[i+4-1]&1 && !places[i+7-1] && (i-1)&7) {
					retval += 2;
				}
				break;
			case ONORMSQR:
				if(places[i+3-1]&1 && !places[i+7-1]) {
					retval += 2;
				}
			case OLFTWALL:
				if(places[i+4-1]&1 && !places[i+9-1] && i&7) {
					++retval;
				}
				break;
		}
	}
	
	if(!retval) {
		if(j) {
			moves[jumpcounter][j] = i;
			moves[jumpcounter][j+1] = 0;
			jumpcounter++;
			moves[jumpcounter][0] = 0;
		}
		return;
	}
	
	else if(retval == 1) {
		moves[jumpcounter][j] = i;
		blackJumpAdd(i+9,j+1);
	}
	else if(retval == 2) {
		moves[jumpcounter][j] = i;
		blackJumpAdd(i+7,j+1);
	}
	else if(retval == 3) {
		int temp[MAXJUMPS];
		moves[jumpcounter][j] = i;
		memcpy(temp, moves[jumpcounter], sizeof(int)*MAXJUMPS);
		blackJumpAdd(i+7,j+1);
		memcpy(moves[jumpcounter], temp, sizeof(int)*MAXJUMPS);
		blackJumpAdd(i+9,j+1);
	}
}

void Board::blackKingJumpAdd(char i, int j) {
		int retval1 = 0;
		int retval2 = 0;
	if(i < 25) {
		switch(getSqrType(i)) {
			case ENORMSQR:
				if(places[i+5-1]&1 && !places[i+9-1]) {
					++retval1;
				}
			case ERGTWALL:
				if(places[i+4-1]&1 && !places[i+7-1] && (i-1)&7) {
					retval1 += 2;
				}
				break;
			case ONORMSQR:
				if(places[i+3-1]&1 && !places[i+7-1]) {
					retval1 += 2;
				}
			case OLFTWALL:
				if(places[i+4-1]&1 && !places[i+9-1] && i&7) {
					++retval1;
				}
				break;
		}
	}	
	if(i > 8) {
		switch(getSqrType(i)) {
			case ENORMSQR:
				if(places[i-3-1]&1 && !places[i-7-1]&1) {
					retval2 += 2;
				}
			case ERGTWALL:
				if(places[i-4-1]&1 && !places[i-9-1] && (i-1)&7) {
					++retval2;
 				}
				break;
			case ONORMSQR:
				if(places[i-5-1]&1 && !places[i-9-1]) {
					++retval2;
				}
			case OLFTWALL:
				if(places[i-4-1]&1 && !places[i-7-1]&1 && i&7) {
					retval2 += 2;
				}
				break;
		}
	}
			
	if(!retval1 && !retval2) {
		if(j) {
			moves[jumpcounter][j] = i;
			moves[jumpcounter][j+1] = 0;
			jumpcounter++;
			moves[jumpcounter][0] = 0;
		}
		return;
	}
		
	int temp[MAXJUMPS];
	moves[jumpcounter][j] = i;
	memcpy(temp, moves[jumpcounter], sizeof(int)*MAXJUMPS);
	if(retval1 == 1) {
		int p1 = places[i-1];
		int p2 = (((i-1)/4)&1?places[i+4-1]:places[i+5-1]);
		places[i-1] = (((i-1)/4)&1?places[i+4-1]:places[i+5-1]) = EMPTY;
		blackKingJumpAdd(i+9,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i+4-1]:places[i+5-1]) = p2;
	}
	else if(retval1 == 2) {
		int p1 = places[i-1];
		int p2 = places[i-1] = (((i-1)/4)&1?places[i+3-1]:places[i+4-1]);
		places[i-1] = (((i-1)/4)&1?places[i+3-1]:places[i+4-1]) = EMPTY;
		blackKingJumpAdd(i+7,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i+3-1]:places[i+4-1]) = p2;
	}
	else if(retval1 == 3) {
		int p1 = places[i-1];
		int p2 = places[i-1] = (((i-1)/4)&1?places[i+3-1]:places[i+4-1]);
		places[i-1] = (((i-1)/4)&1?places[i+3-1]:places[i+4-1]) = EMPTY;
		blackKingJumpAdd(i+7,j+1);
		(((i-1)/4)&1?places[i+3-1]:places[i+4-1]) = p2;
		
		p2 = (((i-1)/4)&1?places[i+4-1]:places[i+5-1]);
		(((i-1)/4)&1?places[i+4-1]:places[i+5-1]) = EMPTY;
		memcpy(moves[jumpcounter], temp, sizeof(int)*MAXJUMPS);
		//moves[jumpcounter][j] = i;
		blackKingJumpAdd(i+9,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i+4-1]:places[i+5-1]) = p2;
	}
	
	memcpy(moves[jumpcounter], temp, sizeof(int)*MAXJUMPS);
	
	if(retval2 == 1) {
		int p1 = places[i-1];
		int p2 = (((i-1)/4)&1?places[i-5-1]:places[i-4-1]);
		places[i-1] = (((i-1)/4)&1?places[i-5-1]:places[i-4-1]) = EMPTY;
		blackKingJumpAdd(i-9,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i-5-1]:places[i-4-1]) = p2;
	}
	else if(retval2 == 2) {
		int p1 = places[i-1];
		int p2 = (((i-1)/4)&1?places[i-4-1]:places[i-3-1]);
		places[i-1] = (((i-1)/4)&1?places[i-4-1]:places[i-3-1]) = EMPTY;
		blackKingJumpAdd(i-7,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i-4-1]:places[i-3-1]) = p2;
	}
	else if(retval2 == 3) {
		int p1 = places[i-1];
		int p2 = places[i-1] = (((i-1)/4)&1?places[i-4-1]:places[i-3-1]);
		places[i-1] = (((i-1)/4)&1?places[i-4-1]:places[i-3-1]) = EMPTY;
		memcpy(temp, moves[jumpcounter], sizeof(int)*MAXJUMPS);
		blackKingJumpAdd(i-7,j+1);
		(((i-1)/4)&1?places[i-4-1]:places[i-3-1]) = p2;
		
		p2 = (((i-1)/4)&1?places[i-5-1]:places[i-4-1]);
		(((i-1)/4)&1?places[i-5-1]:places[i-4-1]) = EMPTY;
		memcpy(moves[jumpcounter], temp, sizeof(int)*MAXJUMPS);
		//moves[jumpcounter][j] = i;
		blackKingJumpAdd(i-9,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i-5-1]:places[i-4-1]) = p2;
	}
}

void Board::whiteJumpAdd(char i, int j) {
	int retval = 0;
	if(i > 8) {
		switch(getSqrType(i)) {
			case ENORMSQR:
				if(!(places[i-3-1]&1) && places[i-3-1] && !places[i-7-1]) {
					retval+=2;
				}
			case ERGTWALL:
				if(!(places[i-4-1]&1) && places[i-4-1] && !places[i-9-1]&1 && (i-1)&7) {
					++retval;
				}
				break;
			case ONORMSQR:
				if(!(places[i-5-1]&1) && places[i-5-1] && !places[i-9-1]) {
					++retval;
				}
			case OLFTWALL:
				if(!(places[i-4-1]&1) && places[i-4-1] && !places[i-7-1]&1 && i&7) {
					retval+=2;
				}
				break;
		}
	}
	
	if(!retval) {
		if(j) {
			moves[jumpcounter][j] = i;
			moves[jumpcounter][j+1] = 0;
			jumpcounter++;
			moves[jumpcounter][0] = 0;
		}
		return;
	}
	
	else if(retval == 1) {
		moves[jumpcounter][j] = i;
		whiteJumpAdd(i-9,j+1);
	}
	else if(retval == 2) {
		moves[jumpcounter][j] = i;
		whiteJumpAdd(i-7,j+1);
	}
	else if(retval == 3) {
		int temp[MAXJUMPS];
		moves[jumpcounter][j] = i;
		memcpy(temp, moves[jumpcounter], MAXJUMPS*sizeof(int));
		whiteJumpAdd(i-9,j+1);
		memcpy(moves[jumpcounter], temp, MAXJUMPS*sizeof(int));
		whiteJumpAdd(i-7,j+1);
	}
}

void Board::whiteKingJumpAdd(char i, int j) {
	int retval1 = 0;
	int retval2 = 0;
	if(i < 25) {
		switch(getSqrType(i)) {
			case ENORMSQR:
				if(places[i+5-1] && !(places[i+5-1]&1) && !places[i+9-1]) {
					++retval1;
				}
			case ERGTWALL:
				if(places[i+4-1] && !(places[i+4-1]&1) && !places[i+7-1] && (i-1)&7) {
					retval1 += 2;
				}
				break;
			case ONORMSQR:
				if(places[i+3-1] && !(places[i+3-1]&1) && !places[i+7-1]) {
					retval1 += 2;
				}
			case OLFTWALL:
				if(places[i+4-1] && !(places[i+4-1]&1) && !places[i+9-1] && i&7) {
					++retval1;
				}
				break;
		}
	}	
	if(i > 8) {
		switch(getSqrType(i)) {
			case ENORMSQR:
				if(places[i-3-1] && !(places[i-3-1]&1) && !places[i-7-1]&1) {
					retval2 += 2;
				}
			case ERGTWALL:
				if(places[i-4-1] && !(places[i-4-1]&1) && !places[i-9-1] && (i-1)&7) {
					++retval2;
 				}
				break;
			case ONORMSQR:
				if(places[i-5-1] && !(places[i-5-1]&1) && !places[i-9-1]) {
					++retval2;
				}
			case OLFTWALL:
				if(places[i-4-1] && !(places[i-4-1]&1) && !places[i-7-1]&1 && i&7) {
					retval2 += 2;
				}
				break;
		}
	}
	
	if(!retval1 && !retval2) {
		if(j) {
			moves[jumpcounter][j] = i;
			moves[jumpcounter][j+1] = 0;
			jumpcounter++;
			moves[jumpcounter][0] = 0;
		}
		return;
	}
		
	int temp[MAXJUMPS];
	moves[jumpcounter][j] = i;
	memcpy(temp, moves[jumpcounter], sizeof(int)*MAXJUMPS);
	if(retval1 == 1) {
		int p1 = places[i-1];
		int p2 = (((i-1)/4)&1?places[i+4-1]:places[i+5-1]);
		places[i-1] = (((i-1)/4)&1?places[i+4-1]:places[i+5-1]) = EMPTY;
		whiteKingJumpAdd(i+9,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i+4-1]:places[i+5-1]) = p2;
	}
	else if(retval1 == 2) {
		int p1 = places[i-1];
		int p2 = places[i-1] = (((i-1)/4)&1?places[i+3-1]:places[i+4-1]);
		places[i-1] = (((i-1)/4)&1?places[i+3-1]:places[i+4-1]) = EMPTY;
		whiteKingJumpAdd(i+7,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i+3-1]:places[i+4-1]) = p2;
	}
	else if(retval1 == 3) {
		int p1 = places[i-1];
		int p2 = places[i-1] = (((i-1)/4)&1?places[i+3-1]:places[i+4-1]);
		places[i-1] = (((i-1)/4)&1?places[i+3-1]:places[i+4-1]) = EMPTY;
		whiteKingJumpAdd(i+7,j+1);
		(((i-1)/4)&1?places[i+3-1]:places[i+4-1]) = p2;
		
		p2 = (((i-1)/4)&1?places[i+4-1]:places[i+5-1]);
		(((i-1)/4)&1?places[i+4-1]:places[i+5-1]) = EMPTY;
		memcpy(moves[jumpcounter], temp, sizeof(int)*MAXJUMPS);
		//moves[jumpcounter][j] = i;
		whiteKingJumpAdd(i+9,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i+4-1]:places[i+5-1]) = p2;
	}
	
	memcpy(moves[jumpcounter], temp, sizeof(int)*MAXJUMPS);
	
	if(retval2 == 1) {
		int p1 = places[i-1];
		int p2 = (((i-1)/4)&1?places[i-5-1]:places[i-4-1]);
		places[i-1] = (((i-1)/4)&1?places[i-5-1]:places[i-4-1]) = EMPTY;
		whiteKingJumpAdd(i-9,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i-5-1]:places[i-4-1]) = p2;
	}
	else if(retval2 == 2) {
		int p1 = places[i-1];
		int p2 = (((i-1)/4)&1?places[i-4-1]:places[i-3-1]);
		places[i-1] = (((i-1)/4)&1?places[i-4-1]:places[i-3-1]) = EMPTY;
		whiteKingJumpAdd(i-7,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i-4-1]:places[i-3-1]) = p2;
	}
	else if(retval2 == 3) {
		int p1 = places[i-1];
		int p2 = places[i-1] = (((i-1)/4)&1?places[i-4-1]:places[i-3-1]);
		places[i-1] = (((i-1)/4)&1?places[i-4-1]:places[i-3-1]) = EMPTY;
		memcpy(temp, moves[jumpcounter], sizeof(int)*MAXJUMPS);
		whiteKingJumpAdd(i-7,j+1);
		(((i-1)/4)&1?places[i-4-1]:places[i-3-1]) = p2;
		
		p2 = (((i-1)/4)&1?places[i-5-1]:places[i-4-1]);
		(((i-1)/4)&1?places[i-5-1]:places[i-4-1]) = EMPTY;
		memcpy(moves[jumpcounter], temp, sizeof(int)*MAXJUMPS);
		//moves[jumpcounter][j] = i;
		whiteKingJumpAdd(i-9,j+1);
		places[i-1] = p1;
		(((i-1)/4)&1?places[i-5-1]:places[i-4-1]) = p2;
	}
}

std::string *Board::toString() {
	std::string *s = new std::string;
	s->resize(32);
	for(int r = 0; r < 32; r++) {
		switch(places[r]) {
			case BNORM:
				places[r] = 'b';
				break;
			case BKING:
				places[r] = 'B';
				break;
			case WNORM:
				places[r] = 'w';
				break;
			case WKING:
				places[r] = 'W';
				break;
			default:
				places[r] = ' ';
				break;
		}
	}
	return s;
}


