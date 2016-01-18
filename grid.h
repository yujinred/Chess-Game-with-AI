#ifndef _GRID_H_
#define _GRID_H_

#include <string>
#include "piece.h"
#include "rook.h"
#include "king.h"
#include "queen.h"
#include "knight.h"
#include "pawn.h"
#include "bishop.h"

#include "window-v2.h"
#include <iostream>


class Grid
{
	Piece*** myGrid;
	char isTurn;
	bool setup;
	friend class Computer;
	bool justCastled;
	char whoCastled;

	bool justEnPassant;
	std::string enPassantCapturedLoc;
public:
	Grid();
	char validSetup(); //Returns 1 if valid setup, else returns p for pawn error, k for king error, c for king in check
	void resetBoard(); //Resets board
	void startGame(); //Starts game with default setup OR pre-setup
	void setupBoard(bool); //sets setup status
	void addPiece(char, std::string); //Add a Game piece to board ie. k e1
	void removePiece(std::string); //Remove a game piece from board
	void setTurn(char); //Sets the current turn to that colour/char, ie. 'b' -> blacks turn, 'w' -> whites turn
	char getTurn(); //Returns isTurn;
	bool isInCheck(char); //Returns 0 for none are in check, 'b' or 'w' respectively for black or white king in check
	bool isInCheckmate(char); //Same as above but for checkmate
	bool isStalemate(char); //Returns true if the board is in stalemate status
	bool movePiece(std::string, std::string); //Coordinates, ie: from e1, to e2. returns true if valid move, else false. From, To
	bool pawnCheck(); //Returns true if there is a pawn on the edge of board
	void promotePawn(char); //Promotes the pawn based on char
	~Grid();

	bool getJustCastled();
	void setJustCastled(bool state);

	bool getJustEnPassant();
	void setJustEnPassant(bool state);

	friend std::ostream& operator<<(std::ostream&, const Grid&);

	void printChess(Xwindow& w); // Prints the whole chess board
	void updateBoard(Xwindow& w, std::string from, std::string to); // Update board to move piece from 'from' to 'to'
	void updateBoard(Xwindow& w, char newPiece, std::string newLoc); // Create a piece at a 'newLoc'
	void updateBoard(Xwindow& w, std::string delSpace); // Erase the piece at 'delSpace'
	void updateBoard(Xwindow& w, char turnState); // update the turns
	void updateBoard(Xwindow& w, int cases); // For special cases
};



#endif
