//Written by Michael Benoit

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstdlib>

#include "board.h"
#include "ai.h"

using namespace std;

void displayBoard(Board b);

string menu(bool *b, bool *w, int *time, int *team);

void options();

int main() {
	cout << "Welcome to Checkers!" << endl;
	bool wHuman, bHuman;
	int timeLimit, team;
	string s = menu(&bHuman, &wHuman, &timeLimit, &team);
	Board board(s, team);
	AI fred(timeLimit);
	while(!board.Uyen()) {
		string junk;
		board.display();
		char move[12] = {0};
		if(board.getTurn() == BLACK) {
			cout << "Black's Move" << endl;
			if(bHuman) {
				board.displayMoves();
				cout << "Choose a Move and end the sequence with 0\nExample: 9 13 0" << endl;
				int i = 0;
				do {
					string t;
					cin >> t;
					move[i] = atoi(t.c_str());;
					i++;
				}while(move[i-1]  && i < 11);
				if(cin.fail())
					cin.ignore();
				board.doMove(&move[0]);
			}
			else {
				board.displayMoves();
				fred.makeMove(board);
			}
		}
		else {
			cout << "White's Move" << endl;
			if(wHuman) {
				board.displayMoves();
				cout << "Choose a Move and end the sequence with 0\nExample: 9 13 0" << endl;
				int i = 0;
				do {
					string t;
					cin >> t;
					move[i] = atoi(t.c_str());
					i++;
				}while(move[i-1]);
				cin.ignore(32, '\n');
				board.doMove(&move[0]);
			}
			else {
				board.displayMoves();
				fred.makeMove(board);
			}
		}
	}
	board.display();
	if(board.getTurn() != BLACK) {
		cout << "BLACK PLAYER WINS!!!" << endl;
	}
	else {
		cout << "white player wins...I guess..." << endl;
	}
	return 0;
}


string menu(bool *b, bool *w, int *time, int *team) {
	cout << "Is black player a Human? (y or n)" << endl;
	ifstream input;
	char c;
	string s;
	int i;
	string game;
	cin >> c;
	cin.ignore();
	switch(c) {
		case 'y':
		case 'Y':
			cout << "Human player" << endl;
			*b = true;
			break;
		case 'n':
		case 'N':
			cout << "Computer player" << endl;
			*b = false;
			break;
		default:
			cout << "Invalid input.  Assuming computer player" << endl;
			*b = false;
			break;
	}
	cout << "Is white player a Human? (y or n)" << endl;
	cin >> c;
	cin.ignore();
	switch(c) {
		case 'y':
		case 'Y':
			cout << "Human player" << endl;
			*w = true;
			break;
		case 'n':
		case 'N':
			cout << "Computer player" << endl;
			*w = false;
			break;
		default:
			cout << "Invalid input.  Assuming computer player" << endl;
			*w = false;
			break;
	}
	
	
	cout << "Input from file? (y or n)" << endl;
	cin >> c;
	switch(c) {
		case 'y':
		case 'Y':
			cout << "Name of the file: " << endl;
			cin >> s;
			input.open(s.c_str());
			for(int x = 0; x < 32; x++) {
				input >> i;
				switch(i) {
					case 0:
						game += ' ';
						break;
					case 1:
						game += 'b';
						break;
					case 2:
						game += 'w';
						break;
					case 3:
						game += 'B';
						break;
					case 4:
						game += 'W';
						break;
				}
			}
			input >> i;
			(i == 1)?(*team = BLACK):(*team = WHITE);
			input >> i;
			*time = i;
			break;
		default:
			cout << "Invalid input.  Assuming no" << endl;
		case 'n':
		case 'N':
			cout << "How much time is the AI allowed per move? (seconds)" << endl;
			cin >> s;
			*time = atoi(s.c_str());
			
			
			cout << "Do you have a saved game you would like to start from? (y or n)" << endl;
			cin >> c;
			cin.ignore();
			switch(c) {
				case 'y':
				case 'Y':
					cout << "Enter the string representing the game" << endl;
					getline(cin,game);
					if(game.size() < 32) {
						cout << "Invalid input.  Assuming normal game" << endl;
						game =  "bbbbbbbbbbbb        wwwwwwwwwwww";
					}
					break;
				default:
					cout << "Invalid input.  Assuming normal game" << endl;
				case 'n':
				case 'N':
					cout << "Normal game" << endl;
					game =  "bbbbbbbbbbbb        wwwwwwwwwwww";
					break;
			}
			
			cout << "Who goes first? (b or w)" << endl;
			cin >> c;
			switch(c) {
				default:
					cout << "Invalid input.  Assuming black goes first" << endl;
				case 'b':
				case 'B':
					*team = BLACK;
					break;
				case 'w':
				case 'W':
					*team = WHITE;
					break;
			}
			return game;
			break;
	}
	
	
}

void options() {
	
}

