#include "grid.h"
#include <iostream>
#include <string>

Grid::Grid() : isTurn('w'), setup(false), justCastled(false), justEnPassant(false), enPassantCapturedLoc("  ")
{
    myGrid = new Piece**[8];
    for (int i = 0; i < 8; i++) {
        myGrid[i] = new Piece*[8];
    }

    for(int i = 0; i < 8; ++i)
    {
    	for(int j = 0; j < 8; ++j)
    	{
    		myGrid[i][j] = 0;
    	}
    }
}

char Grid::validSetup()
{
	// Verify board contains exactly one white king and one black king
	int kingw = 0;
	int kingb = 0;
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myGrid[i][j] != 0)
			{
				if(myGrid[i][j]->whatPiece() == 'K') ++kingw;
				if(myGrid[i][j]->whatPiece() == 'k') ++kingb;
			}
		}
	}
	if(kingw != 1 || kingb != 1) return 'k';

	// No pawns on first or last row
	for(int j = 0; j < 8; ++j)
	{
		if(myGrid[0][j] != 0)
		{
			if(myGrid[0][j]->whatPiece() == 'p' || myGrid[0][j]->whatPiece() == 'P') return 'p';
		}
	}

	// No king is in check
	if(isInCheck('w') || isInCheck('b')) return 'c';
	return '1';
}


void Grid::resetBoard()
{
    for(int i = 0; i < 8; ++i)
    {
    	for(int j = 0; j < 8; ++j)
    	{
    		if(myGrid[i][j] != 0)
    		{
    			delete myGrid[i][j];
    			myGrid[i][j] = 0;
			}
    	}
    }
    isTurn = 'w';
    setup = false;
}

void Grid::startGame()
{
	if(!setup) //If it hasn't been setup, then setup to default
	{
	    isTurn = 'w';
		for(int i = 0; i < 8; ++i) //Loop over rows
		{
			if(i == 0 || i == 7) //The non-pawns
			{
				//Loop over columns
				for(int j = 0; j < 8; ++j)
				{
					//Create Coordinate string
					char x = j + 'a';
					char y = 7 - i + '1';
					std::string coord = "  ";
					coord[0] = x;
					coord[1] = y;

					if(i == 0) //Black Pieces
					{
						//Rook (r)
						if(j == 0 || j == 7) myGrid[i][j] = new Rook(coord, 'b');

						//Knight (n)
						if(j == 1 || j == 6) myGrid[i][j] = new Knight(coord, 'b');

						//Bishop (b)
						if(j == 2 || j == 5) myGrid[i][j] = new Bishop(coord, 'b');

						//Queen (q)
						if(j == 3) myGrid[i][j] = new Queen(coord, 'b');

						//King (k)
						if(j == 4) myGrid[i][j] = new King(coord, 'b');
					}
					else if(i == 7) //White pieces
					{
						//Rook (R)
						if(j == 0 || j == 7) myGrid[i][j] = new Rook(coord, 'w');

						//Knight (N)
						if(j == 1 || j == 6) myGrid[i][j] = new Knight(coord, 'w');

						//Bishop (B)
						if(j == 2 || j == 5) myGrid[i][j] = new Bishop(coord, 'w');

						//Queen (Q)
						if(j == 3) myGrid[i][j] = new Queen(coord, 'w');

						//King (K)
						if(j == 4) myGrid[i][j] = new King(coord, 'w');
					}
				}
			}
			else if(i == 1 || i == 6)//The pawns
			{
				//Loop over columns
				for(int j = 0; j < 8; ++j)
				{
					//Create Coordinate string
					char x = j + 'a';
					char y = 7 - i + '1';
					std::string coord = "  ";
					coord[0] = x;
					coord[1] = y;

					//Initialize
					if(i == 1) myGrid[i][j] = new Pawn(coord, 'b'); //Black pieces
					else if(i == 6) myGrid[i][j] = new Pawn(coord, 'w'); //White pieces
				}
			}
			else //The empty pieces
			{
				for(int j = 0; j < 8; ++j)
				{
					myGrid[i][j] = 0;
				}
			}
		}
	}
}

void Grid::setupBoard(bool s)
{
	setup = s;
}

void Grid::addPiece(char p, std::string coord)
{
	//Convert y axis letter coordinate to array number
	int y = coord[0] - 'a';

	//Convert x axis number coordinate to array number
	int x = 8 - (coord[1] - '1' + 1);

	//If space isn't empty, delete first
	if(myGrid[x][y] != 0)
	{
		delete myGrid[x][y];
		myGrid[x][y] = 0;
	}
	switch(p)
	{
		case 'r':
			myGrid[x][y] = new Rook(coord, 'b');
			break;
		case 'R':
			myGrid[x][y] = new Rook(coord, 'w');
			break;
		case 'n':
			myGrid[x][y] = new Knight(coord, 'b');
			break;
		case 'N':
			myGrid[x][y] = new Knight(coord, 'w');
			break;
		case 'b':
			myGrid[x][y] = new Bishop(coord, 'b');
			break;
		case 'B':
			myGrid[x][y] = new Bishop(coord, 'w');
			break;
		case 'q':
			myGrid[x][y] = new Queen(coord, 'b');
			break;
		case 'Q':
			myGrid[x][y] = new Queen(coord, 'w');
			break;
		case 'k':
			myGrid[x][y] = new King(coord, 'b');
			break;
		case 'K':
			myGrid[x][y] = new King(coord, 'w');
			break;
		case 'p':
			myGrid[x][y] = new Pawn(coord, 'b');
			break;
		case 'P':
			myGrid[x][y] = new Pawn(coord, 'w');
			break;
	}

}

void Grid::removePiece(std::string coord)
{
	//Convert y axis letter coordinate to array number
	int y = coord[0] - 'a';

	//Convert x axis number coordinate to array number
	int x = 8 - (coord[1] - '1' + 1);

	//Check if location is empty or not. If not, remove/set to null
	if(myGrid[x][y] != 0)
	{
		//Delete and set to null
		delete myGrid[x][y];
		myGrid[x][y] = 0;
	}
}

void Grid::setTurn(char c)
{
	isTurn = c;
}

char Grid::getTurn()
{
	return isTurn;
}

bool Grid::isInCheck(char c)
{
	//Locate the king
	std::string coord = "  ";
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myGrid[i][j] != 0)
			{
				if(myGrid[i][j]->getColour() == c)
				{
					if((myGrid[i][j]->whatPiece() == 'K' && c == 'w') || (myGrid[i][j]->whatPiece() == 'k' && c == 'b'))
					{
						char x = j + 'a';
						char y = 7 - i + '1';
						coord[0] = x;
						coord[1] = y;
						break;
					}
				}
			}
		}
	}
	//std::cerr << "King location: " << c << " " << coord << std::endl;
	//Check if opponents pieces can target the king
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myGrid[i][j] != 0)
			{
				if(myGrid[i][j]->getColour() != c)
				{
					if(myGrid[i][j]->isLegal(myGrid, coord))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Grid::isInCheckmate(char c)
{
	int kingx, kingy;
	//Locate the king
	std::string coord = "  ";
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myGrid[i][j] != 0)
			{
				if(myGrid[i][j]->getColour() == c)
				{
					if(myGrid[i][j]->whatPiece() == 'K' || myGrid[i][j]->whatPiece() == 'k')
					{
						kingx = i;
						kingy = j;
					}
				}
			}
		}
	}

	//Assuming isInCheckmate is called when isInCheck is true
	if(!myGrid[kingx][kingy]->hasLegal(myGrid)) {
        Piece *myPieces[16];
        int num = 0;
        for(int i = 0; i < 8; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                if(myGrid[i][j] != 0)
                {
                    if(myGrid[i][j]->getColour() == c)
                    {
                        myPieces[num] = myGrid[i][j];
                        num++;
                    }
                }
            }
        }

        //Remove pieces that do not have legal moves
        for(int i = 0; i < num; ++i)
        {
            if(!myPieces[i]->hasLegal(myGrid))
            {
                myPieces[i] = 0;
            }
        }

        //Transfer to new array
        Piece *myPlayablePieces[num];
        int count = 0;
        for(int i = 0; i < num; ++i)
        {
            if(myPieces[i] != 0)
            {
                myPlayablePieces[count] = myPieces[i];
                count++;
            }
        }

        for(int i = 0; i < count; ++i)
        {
            int numPossibleHere = myPlayablePieces[i]->numberPossibleMoves(myGrid);
            std::string *tempPossibleMoves = myPlayablePieces[i]->possibleMoves(myGrid);
            for(int j = 0; j < numPossibleHere; ++j)
            {
                int colNum = tempPossibleMoves[j][0] - 'a';
                int rowNum = 8 - (tempPossibleMoves[j][1] - '1' + 1);
                Piece* temp = myGrid[colNum][rowNum];
                myGrid[colNum][rowNum] = myPlayablePieces[i];
                if (!isInCheck(c)) {
                    myGrid[colNum][rowNum] = temp;
                    return false;
                }
                myGrid[colNum][rowNum] = temp;
                temp = NULL;
            }
            delete [] tempPossibleMoves;
        }
        return true;
	}
	else return false;

	//resetBoard(); //Clear board
}

bool Grid::isStalemate(char c)
{
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            if(myGrid[i][j] != 0)
            {
                if (myGrid[i][j]->getColour() == c) {
                    if(myGrid[i][j]->hasLegal(myGrid)) {
                        //std::cerr << "myGrid["<< i<<"]["<<j<<"]->hasLegal(myGrid) = true" << std::endl;
                        return false;
                    }
                }
            }
        }
	}
	return true;
}

bool Grid::movePiece(std::string from, std::string to)
{
	int fromCol = from[0] - 'a';
	int fromRow = 8 - (from[1] - '1' + 1);

	//Check if the piece selected is indeed a piece and is the current player's piece
	if(myGrid[fromRow][fromCol] != 0 && myGrid[fromRow][fromCol]->getColour() == isTurn) {
		int toCol = to[0] - 'a';
		int toRow = 8 - (to[1] - '1' + 1);

		//std::cout << "correct player" << std::endl;

		//Check if the move is indeed legal
		if(myGrid[fromRow][fromCol]->isLegal(myGrid, to)) {
			//std::cout << "legal move" << std::endl;

			myGrid[fromRow][fromCol]->setCoord(to); //Change coordinates

			if (myGrid[fromRow][fromCol]->whatPiece() == 'k' || myGrid[fromRow][fromCol]->whatPiece() == 'K' ||
                myGrid[fromRow][fromCol]->whatPiece() == 'r' || myGrid[fromRow][fromCol]->whatPiece() == 'R')
            {
                myGrid[fromRow][fromCol]->setIsMoved(1);
            }

            //If the destination is empty
			if(myGrid[toRow][toCol] == NULL) {
				myGrid[toRow][toCol] = myGrid[fromRow][fromCol];
				myGrid[fromRow][fromCol] = NULL;
				//std::cout << "empty destination" << std::endl;
			} else { //The destination contains an enemy piece
				delete myGrid[toRow][toCol];
				myGrid[toRow][toCol] = myGrid[fromRow][fromCol];
				myGrid[fromRow][fromCol] = NULL;
				//std::cout << "capture" << std::endl;
			}

			return true;
		} else if (myGrid[fromRow][fromCol]->whatPiece() == 'p' || myGrid[fromRow][fromCol]->whatPiece() == 'P') { // For En Passant
		    int direction;
		    if (myGrid[fromRow][fromCol]->whatPiece() == 'p') direction = 1;
		    else direction = -1;
		    //std::cerr << "First Gate" << std::endl;
            // check left-side
            if (fromCol != 0 && myGrid[fromRow][fromCol-1] != NULL) {
                if (myGrid[fromRow][fromCol-1]->whatPiece() == 'p' || myGrid[fromRow][fromCol-1]->whatPiece() == 'P') {
                    if (myGrid[fromRow][fromCol-1]->getJustTwiceStep() == 1) {
                            //std::cerr << "Second Gate" << std::endl;
                        std::string toEnPassant = "  ";
                        toEnPassant[0] = 'a' + (fromCol - 1);
                        toEnPassant[1] = '8' - (fromRow + direction);
                        if (to == toEnPassant) {
                            enPassantCapturedLoc[0] = toEnPassant[0];
                            enPassantCapturedLoc[1] = '8' - fromRow;
                            delete myGrid[fromRow][fromCol-1];
                            myGrid[fromRow][fromCol-1] = 0;
                            myGrid[toRow][toCol] = myGrid[fromRow][fromCol];
                            myGrid[toRow][toCol]->setCoord(to);
                            myGrid[fromRow][fromCol] = NULL;
                            std::cout << "Pawn en Passant" << std::endl;
                            justEnPassant = true;
                            return true;
                        }
                    }
                }
            } else if (fromCol != 7 && myGrid[fromRow][fromCol+1] != NULL) {
                if (myGrid[fromRow][fromCol+1]->whatPiece() == 'p' || myGrid[fromRow][fromCol+1]->whatPiece() == 'P') {
                    if (myGrid[fromRow][fromCol+1]->getJustTwiceStep() == 1) {
                      //      std::cerr << "SECOND.2 Gate" << std::endl;
                        std::string toEnPassant = "  ";
                        toEnPassant[0] = 'a' + (fromCol + 1);
                        toEnPassant[1] = '8' - (fromRow + direction);
                        //std::cout << toEnPassant << std::endl;
                        if (to == toEnPassant) {
                            enPassantCapturedLoc[0] = toEnPassant[0];
                            enPassantCapturedLoc[1] = '8' - fromRow;
                            delete myGrid[fromRow][fromCol+1];
                            myGrid[fromRow][fromCol+1] = 0;
                            myGrid[toRow][toCol] = myGrid[fromRow][fromCol];
                            myGrid[toRow][toCol]->setCoord(to);
                            myGrid[fromRow][fromCol] = NULL;
                            std::cout << "Pawn en Passant" << std::endl;
                            justEnPassant = true;
                            return true;
                        }
                    }
                }
            }
		} else { //For castling
		    // Check if
			if(myGrid[fromRow][fromCol] != NULL && myGrid[fromRow][fromCol+3] != NULL) {
				if(to == "g1" && myGrid[fromRow][fromCol]->whatPiece() == 'K' && myGrid[fromRow][fromCol+3]->whatPiece() == 'R') {
					if(static_cast<King*>(myGrid[fromRow][fromCol])->canCastling(myGrid) && static_cast<Rook*>(myGrid[fromRow][fromCol+3])->canCastling(myGrid)) {
						myGrid[toRow][toCol] = myGrid[fromRow][fromCol];
						myGrid[toRow][toCol]->setCoord(to);
						myGrid[fromRow][fromCol] = 0;
						myGrid[toRow][toCol-1] = myGrid[fromRow][fromCol+3];
						std::string newPos = "  ";
                        newPos[0] = 'a' + (toRow);
						newPos[1] = '8' - (toCol-1);
						//std::cerr << newPos << std::endl;
						myGrid[toRow][toCol-1]->setCoord(newPos);
						myGrid[fromRow][fromCol+3] = 0;
						justCastled = true;
						whoCastled = 'w';
						std::cout << "Castling" << std::endl;
						return true;
					}
				} else if(to == "g8" && myGrid[fromRow][fromCol]->whatPiece() == 'k' && myGrid[fromRow][fromCol+3]->whatPiece() == 'r') {
					if(static_cast<King*>(myGrid[fromRow][fromCol])->canCastling(myGrid) && static_cast<Rook*>(myGrid[fromRow][fromCol+3])->canCastling(myGrid)) {
						myGrid[toRow][toCol] = myGrid[fromRow][fromCol];
						myGrid[toRow][toCol]->setCoord(to);
						myGrid[fromRow][fromCol] = 0;
						myGrid[toRow][toCol-1] = myGrid[fromRow][fromCol+3];
						std::string newPos = "  ";
                        newPos[0] = 'a' + (toRow);
						newPos[1] = '8' - (toCol - 1);
						//std::cerr << newPos << std::endl;
						myGrid[toRow][toCol-1]->setCoord(newPos);
						myGrid[fromRow][fromCol+3] = 0;
						justCastled = true;
						whoCastled = 'b';
						std::cout << "Castling" << std::endl;
						return true;
					}
				}
			}
			if(myGrid[fromRow][fromCol] != 0 && myGrid[fromRow][fromCol-4] != 0)
			{
				if(to == "c8" && myGrid[fromRow][fromCol]->whatPiece() == 'k' && myGrid[fromRow][fromCol-4]->whatPiece() == 'r')
				{
					if(static_cast<King*>(myGrid[fromRow][fromCol])->canCastling(myGrid) && static_cast<Rook*>(myGrid[fromRow][fromCol-4])->canCastling(myGrid))
					{
						myGrid[toRow][toCol] = myGrid[fromRow][fromCol];
						myGrid[toRow][toCol]->setCoord(to);
						myGrid[fromRow][fromCol] = 0;
						myGrid[toRow][toCol+1] = myGrid[fromRow][fromCol-4];
						std::string newPos = "  ";
                        newPos[0] = 'a' + (toRow);
						newPos[1] = '8' - (toCol + 1);
						//std::cerr << newPos << std::endl;
						myGrid[toRow][toCol+1]->setCoord(newPos);
						myGrid[fromRow][fromCol-4] = 0;
						justCastled = true;
						whoCastled = 'b';
						return true;
					}
				}
				else if(to == "c1" && myGrid[fromRow][fromCol]->whatPiece() == 'K' && myGrid[fromRow][fromCol-4]->whatPiece() == 'R')
				{
					if(static_cast<King*>(myGrid[fromRow][fromCol])->canCastling(myGrid) && static_cast<Rook*>(myGrid[fromRow][fromCol-4])->canCastling(myGrid))
					{
						myGrid[toRow][toCol] = myGrid[fromRow][fromCol];
						myGrid[toRow][toCol]->setCoord(to);
						myGrid[fromRow][fromCol] = 0;
						myGrid[toRow][toCol+1] = myGrid[fromRow][fromCol-4];
						std::string newPos = "  ";
                        newPos[0] = 'a' + (toRow);
						newPos[1] = '8' - (toCol + 1);
						//std::cerr << newPos << std::endl;
						myGrid[toRow][toCol+1]->setCoord(newPos);
						myGrid[fromRow][fromCol-4] = 0;
						justCastled = true;
						whoCastled = 'w';
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Grid::pawnCheck()
{
	for(int i = 0; i < 8; i += 7) //Check if any pawns on end rows
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myGrid[i][j] != 0)
			{
				if(myGrid[i][j]->whatPiece() == 'p' || myGrid[i][j]->whatPiece() == 'P')
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Grid::getJustCastled() {
    return justCastled;
}

void Grid::setJustCastled(bool state) {
    justCastled = state;
}

bool Grid::getJustEnPassant() {
    return justEnPassant;
}

void Grid::setJustEnPassant(bool state) {
    justEnPassant = state;
}


void Grid::promotePawn(char c = 0)
{
	if(c == 0)
	{
		std::cout << "No pawn promotion specified, default to queen" << std::endl;
		if(isTurn == 'w') c = 'Q';
		else c = 'q';
	}
	//Locate the pawn
	for(int i = 0; i < 8; i += 7)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myGrid[i][j] != 0)
			{
				if(myGrid[i][j]->whatPiece() == 'p' || myGrid[i][j]->whatPiece() == 'P')
				{
					delete myGrid[i][j];
					myGrid[i][j] = 0;
					//Create Coordinate string
					char x = j + 'a';
					char y = 7 - i + '1';
					std::string coord = "  ";
					coord[0] = x;
					coord[1] = y;

					switch(c)
					{
						case 'r':
							myGrid[i][j] = new Rook(coord, 'b');
							break;
						case 'R':
							myGrid[i][j] = new Rook(coord, 'w');
							break;
						case 'n':
							myGrid[i][j] = new Knight(coord, 'b');
							break;
						case 'N':
							myGrid[i][j] = new Knight(coord, 'w');
							break;
						case 'b':
							myGrid[i][j] = new Bishop(coord, 'b');
							break;
						case 'B':
							myGrid[i][j] = new Bishop(coord, 'w');
							break;
						case 'q':
							myGrid[i][j] = new Queen(coord, 'b');
							break;
						case 'Q':
							myGrid[i][j] = new Queen(coord, 'w');
							break;
					}
				}
			}
		}
	}
}

Grid::~Grid() {
    resetBoard();
    for (int i=0; i < 8; i++) delete [] myGrid[i];
    delete [] myGrid;
}

std::ostream& operator<<(std::ostream& out, const Grid& g)
{
	for(int i = 0; i < 8; ++i)
	{
		out << 8 - i << " "; //Print the axis number on the side
		for(int j = 0; j < 8; ++j)
		{
			if(g.myGrid[i][j] != 0)
			{
				out << g.myGrid[i][j]->whatPiece(); //Print piece
			}
			else //If the space is blank
			{
				//Print the space on every even column on even rows and every odd column on odd rows
				if((8 - i)%2 == 1 && (8 - j)%2 == 0)
				{
					out << "_";
				}
				else if ((8 - i)%2 == 0 && (8 - j)%2 == 1)
				{
					out << "_";
				}
				else
				{
					out << " ";
				}
			}
		}
		std::cout << std::endl; //Print new line for the end of row
	}
	out << std::endl << "  " << "abcdefgh" << std::endl;
	std::string turnMsg = "This turn is: ";
	if (g.isTurn == 'b') {
        turnMsg += "black";
	} else {
        turnMsg += "white";
	}
	out << std::endl << turnMsg << std::endl;
	return out;
}

std::string charToString(char c) {
    std::string s = "";
    s = s + c;
    return s;
}

void Grid::printChess(Xwindow& w) {
    int blockSize = 40;
    int displacement = 30;
	w.fillRectangle(displacement, 0, blockSize*8, blockSize*8, 9);
    for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
            //Print the space on every even column on even rows and every odd column on odd rows
			if((8 - i)%2 == 1 && (8 - j)%2 == 0) {
				w.fillRectangle(displacement+blockSize*j, blockSize*i, blockSize, blockSize, 8);
			} else if ((8 - i)%2 == 0 && (8 - j)%2 == 1) {
				w.fillRectangle(displacement+blockSize*j, blockSize*i, blockSize, blockSize, 8);
			}
			if(myGrid[i][j] != 0) {
                int pieceColor;
                if (myGrid[i][j]->getColour() == 'b') {
                    pieceColor = 1;
                } else {
                    pieceColor = 0;
                }
                w.drawBigString(displacement+blockSize*j+12, blockSize*i+25, charToString(myGrid[i][j]->whatPiece()), pieceColor);
            }
            if (i == 7) {
                char letterChart = 'a'+j;
                w.drawBigString(displacement+blockSize*j+12, blockSize*8+25, charToString(letterChart), 4);
            }
        }
        char numChart = '8'-i;
        w.drawBigString(7, blockSize*i+27, charToString(numChart), 7);
	}
	std::string turnMsg = "This turn is: ";
	if (isTurn == 'b') {
        turnMsg += "black";
	} else {
        turnMsg += "white";
	}
	w.fillRectangle(displacement, blockSize* 9, blockSize*8, blockSize, 0);
	w.drawBigString(displacement, blockSize* 10 - 15, turnMsg, 1);
}

void Grid::updateBoard(Xwindow& w, std::string from, std::string to) {
    int blockSize = 40;
    int displacement = 30;
    int fromCol = from[0]-'a';
    int fromRow = 8 - (from[1]-'1'+1);
    int toCol = to[0]-'a';
    int toRow = 8 - (to[1]-'1'+1);

    if (myGrid[toRow][toCol] != NULL) {
        int pieceColor;
        if (myGrid[toRow][toCol]->getColour() == 'b') {
            pieceColor = 1;
        } else {
            pieceColor = 0;
        }
        int boardColor;
        if ((toRow%2==0 && toCol%2==1) || (toRow%2==1 && toCol%2==0)) {
            boardColor = 8;
        } else {
            boardColor = 9;
        }
        w.fillRectangle(displacement+blockSize*toCol, blockSize*toRow, blockSize, blockSize, boardColor);
        w.drawBigString(displacement+blockSize*toCol+12, blockSize*toRow+25,charToString(myGrid[toRow][toCol]->whatPiece()), pieceColor);
    }
    int boardColor;
    if ((fromRow%2==0 && fromCol%2==1) || (fromRow%2==1 && fromCol%2==0)) {
		boardColor = 8;
	} else {
		boardColor = 9;
    }
    w.fillRectangle(displacement+blockSize*fromCol, blockSize*fromRow, blockSize, blockSize, boardColor);
    std::string turnMsg = "This turn is: ";
	if (isTurn == 'b') {
        turnMsg += "black";
	} else {
        turnMsg += "white";
	}
	w.fillRectangle(displacement, blockSize* 9, blockSize*8, blockSize, 0);
	w.drawBigString(displacement, blockSize* 10 - 15, turnMsg, 1);
}

void Grid::updateBoard(Xwindow& w, char newPiece, std::string newLoc) {
    int blockSize = 40;
    int displacement = 30;
    int newLocCol = newLoc[0]-'a';
    int newLocRow = 8 - (newLoc[1]-'1'+1);

    if (myGrid[newLocRow][newLocCol] != NULL) {
        int pieceColor;
        if (myGrid[newLocRow][newLocCol]->getColour() == 'b') {
            pieceColor = 1;
        } else {
            pieceColor = 0;
        }
        int boardColor;
        if ((newLocRow%2==0 && newLocCol%2==1) || (newLocRow%2==1 && newLocCol%2==0)) {
            boardColor = 8;
        } else {
            boardColor = 9;
        }
        w.fillRectangle(displacement+blockSize*newLocCol, blockSize*newLocRow, blockSize, blockSize, boardColor);
        w.drawBigString(displacement+blockSize*newLocCol+12, blockSize*newLocRow+25,charToString(myGrid[newLocRow][newLocCol]->whatPiece()), pieceColor);
    }
}

void Grid::updateBoard(Xwindow& w, std::string delSpace) {
    int blockSize = 40;
    int displacement = 30;
    int fromCol = delSpace[0]-'a';
    int fromRow = 8 - (delSpace[1]-'1'+1);
    int boardColor;
    if ((fromRow%2==0 && fromCol%2==1) || (fromRow%2==1 && fromCol%2==0)) {
		boardColor = 8;
	} else {
		boardColor = 9;
    }
    w.fillRectangle(displacement+blockSize*fromCol, blockSize*fromRow, blockSize, blockSize, boardColor);
}

void Grid::updateBoard(Xwindow& w, char turnState) {
    int blockSize = 40;
    int displacement = 30;
    std::string turnMsg = "This turn is: ";
	if (turnState == 'b') {
        turnMsg += "black";
	} else {
        turnMsg += "white";
	}
	w.fillRectangle(displacement, blockSize* 9, blockSize*8, blockSize, 0);
	w.drawBigString(displacement, blockSize* 10 - 15, turnMsg, 1);
}

// cases == 1 is castling
// cases == 2 is en passant
void Grid::updateBoard(Xwindow& w, int cases) {
    int blockSize = 40;
    int displacement = 30;
    if (cases == 1) { // for castling
        if (whoCastled == 'w') {
            for (int i = 0; i < 8; i++) {
                if (myGrid[7][i] != NULL) {
                    int boardColor;
                    if (i%2 == 0) {
                        boardColor = 8;
                    } else {
                        boardColor = 9;
                    }
                    w.fillRectangle(displacement+blockSize*i, blockSize*7, blockSize, blockSize, boardColor);
                    int pieceColor;
                    if (myGrid[7][i]->getColour() == 'b') {
                        pieceColor = 1;
                    } else {
                        pieceColor = 0;
                    }
                    w.drawBigString(displacement+blockSize*i+12, blockSize*7+25,charToString(myGrid[7][i]->whatPiece()), pieceColor);
                } else {
                    int boardColor;
                    if (i%2 == 0) {
                        boardColor = 8;
                    } else {
                        boardColor = 9;
                    }
                    w.fillRectangle(displacement+blockSize*i, blockSize*7, blockSize, blockSize, boardColor);
                }
            }
        } else {
            for (int i = 0; i < 8; i++) {
                if (myGrid[0][i] != NULL) {
                    int boardColor;
                    if (i%2 == 1) {
                        boardColor = 8;
                    } else {
                        boardColor = 9;
                    }
                    w.fillRectangle(displacement+blockSize*i, 0, blockSize, blockSize, boardColor);
                    int pieceColor;
                    if (myGrid[0][i]->getColour() == 'b') {
                        pieceColor = 1;
                    } else {
                        pieceColor = 0;
                    }
                    w.drawBigString(displacement+blockSize*i+12, 25,charToString(myGrid[0][i]->whatPiece()), pieceColor);
                } else {
                    int boardColor;
                    if (i%2 == 1) {
                        boardColor = 8;
                    } else {
                        boardColor = 9;
                    }
                    w.fillRectangle(displacement+blockSize*i, 0, blockSize, blockSize, boardColor);
                }
            }
        }
    } // Castling ends
    else if (cases == 2) { // For En Passant
        updateBoard(w, enPassantCapturedLoc);
    }
}
