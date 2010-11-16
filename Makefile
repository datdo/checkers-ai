checkers: ai.o board.o checkers.o hash.o
	g++ -O3 ai.o board.o checkers.o hash.o -o checkers
	
ai.o: ai.cpp ai.h board.h hash.h
	g++ -O3 -c ai.cpp
	
board.o: board.cpp board.h
	g++ -O3 -c board.cpp

checkers.o: checkers.cpp ai.h board.h
	g++ -O3 -c checkers.cpp

hash.o: hash.cpp hash.h
	g++ -O3 -c hash.cpp

debug:
	g++ -g checkers.cpp ai.cpp board.cpp hash.cpp -o checkersDebug
	
clean: 
	rm *.o *.exe
