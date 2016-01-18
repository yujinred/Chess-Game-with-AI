#ifndef _COMPUTER_H_
#define _COMPUTER_H_
#include "player.h"
#include <string>

class Grid;

class Piece;

class Computer : public Player
{
	int level;
	std::string lastFrom;
	std::string lastTo;
	void levelOne(Grid*);
	void levelTwo(Grid*);
	void levelThree(Grid*);
	void levelFour(Grid*);
public:
	Computer(int, char);
	void generateMove(Grid*);
	std::string getLastFrom();
	std::string getLastTo();
	~Computer();
};

#endif
