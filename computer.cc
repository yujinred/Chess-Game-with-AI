#include "computer.h"
#include "grid.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <time.h>


Computer::Computer(int level, char colour) : Player(colour), level(level) {}

void Computer::levelOne(Grid *theGrid)
{
	srand(time(NULL)); //Set random seed

	Piece ***myBoard = theGrid->myGrid;
	std::cout << "Level One initiated" << std::endl;
	Piece *myPieces[16];
	int num = 0;
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myBoard[i][j] != 0)
			{
				if(myBoard[i][j]->getColour() == colour)
				{
					myPieces[num] = myBoard[i][j];
					num++;
				}
			}
		}
	}
	//num--; //Decrease by 1 for array index

	//Remove pieces that do not have legal moves
	for(int i = 0; i < num; ++i)
	{
		if(!myPieces[i]->hasLegal(myBoard))
		{
			myPieces[i] = 0;
		}
	}

	//Transfer to new array
	//Piece **myPlayablePieces = new Piece*[num];
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
	//count--; //Decrease by 1 for array index

	std::string from;
	std::string to;

	//If computer is in check, avoid
	bool avoidCaptureKing = false;
	if(theGrid->isInCheck(colour))
	{
		Piece *enemyPieces[16];
		int num2 = 0;
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() != colour)
					{
						enemyPieces[num2] = myBoard[i][j];
						num2++;
					}
				}
			}
		}
		//num2--; //Decrease by 1 for array index

		//Remove pieces that do not have legal moves
		for(int i = 0; i < num2; ++i)
		{
			if(!enemyPieces[i]->hasLegal(myBoard))
			{
				enemyPieces[i] = 0;
			}
		}

		//Transfer to new array
		Piece *enemyPlayablePieces[num2];
		int count2 = 0;
		for(int i = 0; i < num2; ++i)
		{
			if(enemyPieces[i] != 0)
			{
				enemyPlayablePieces[count2] = enemyPieces[i];
				count2++;
			}
		}
		//count2--; //Decrease by 1 for array index

		int kingx, kingy;
		//Locate the king
		std::string coord = "  ";
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() == colour)
					{
						if(myBoard[i][j]->whatPiece() == 'K' || myBoard[i][j]->whatPiece() == 'k')
						{
							kingx = i;
							kingy = j;
						}
					}
				}
			}
		}
		//Check if the piece has a safe place to go to
		int possibleSafeNumKing = myBoard[kingx][kingy]->numberPossibleMoves(myBoard);
		std::string *possibleSafeMovesKing = myBoard[kingx][kingy]->possibleMoves(myBoard);
		bool safeKing = true;
		for(int q = 0; q < possibleSafeNumKing; ++q)
		{
			for(int z = 0; z < count2; ++ z)
			{
				if(enemyPlayablePieces[z]->isLegal(myBoard, possibleSafeMovesKing[q]))
				{
					//std::cerr << "avoid cap" << std::endl;
					safeKing = false;
					avoidCaptureKing = false;
					break;
				}
				else
				{
					safeKing = true;
					avoidCaptureKing = true;
					from = myBoard[kingx][kingy]->coord;
					to = possibleSafeMovesKing[q];
				}
			}
			if(safeKing) break;
		}
		delete [] possibleSafeMovesKing;
	}

	//Check for capture king
	bool captureMove = false;
	if(!avoidCaptureKing)
	{
		for(int i = 0; i < count; ++i)
		{
			int numPossibleHere = myPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *tempPossibleMoves = myPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleHere; ++j)
			{
				int y1 = tempPossibleMoves[j][0] - 'a';
				int x1 = 8 - (tempPossibleMoves[j][1] - '1' + 1);
				if(myBoard[x1][y1] != 0)
				{
					if(myBoard[x1][y1]->getColour() != colour)
					{
						if(myBoard[x1][y1]->whatPiece() == 'k' || myBoard[x1][y1]->whatPiece() == 'K')
						{
							from = myPlayablePieces[i]->coord;
							to = tempPossibleMoves[j];
							captureMove = true;
							break;
						}
					}
				}
			}
			delete [] tempPossibleMoves;
		}
	}

	if(!avoidCaptureKing && !captureMove) //If king is not capturable
	{
		//Pick a random piece to move
		int random = rand() % count;
		//std::cerr << "Random: " << random << std::endl;
		from = myPlayablePieces[random]->coord;
		//std::cerr << "From: " << from << std::endl;

		//Pick a random move
		std::string *possibleMoves = myPlayablePieces[random]->possibleMoves(myBoard);
		int numPossible = myPlayablePieces[random]->numberPossibleMoves(myBoard);
		int random2 = rand() % numPossible;
		to = possibleMoves[random2];
		//std::cerr << "To: " << to << std::endl;'
		delete [] possibleMoves;
	}

	//Execute move
	theGrid->movePiece(from, to);
	//std::cerr << "Level one calc ended" << std::endl;
	//delete [] myPlayablePieces;
	lastFrom = from;
	lastTo = to;
}

void Computer::levelTwo(Grid *theGrid)
{
	srand(time(NULL)); //Set random seed

	Piece ***myBoard = theGrid->myGrid;
	std::cout << "Level Two initiated" << std::endl;
	Piece *myPieces[16];
	int num = 0;
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myBoard[i][j] != 0)
			{
				if(myBoard[i][j]->getColour() == colour)
				{
					myPieces[num] = myBoard[i][j];
					num++;
				}
			}
		}
	}
	//num--; //Decrease by 1 for array index

	//Remove pieces that do not have legal moves
	for(int i = 0; i < num; ++i)
	{
		if(!myPieces[i]->hasLegal(myBoard))
		{
			myPieces[i] = 0;
		}
	}

	//Transfer to new array
	//Piece **myPlayablePieces = new Piece*[num];
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
	//count--; //Decrease by 1 for array index

	std::string from;
	std::string to;

	//If computer is in check, avoid
	bool avoidCaptureKing = false;
	if(theGrid->isInCheck(colour))
	{
		Piece *enemyPieces[16];
		int num2 = 0;
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() != colour)
					{
						enemyPieces[num2] = myBoard[i][j];
						num2++;
					}
				}
			}
		}
		//num2--; //Decrease by 1 for array index

		//Remove pieces that do not have legal moves
		for(int i = 0; i < num2; ++i)
		{
			if(!enemyPieces[i]->hasLegal(myBoard))
			{
				enemyPieces[i] = 0;
			}
		}

		//Transfer to new array
		Piece *enemyPlayablePieces[num2];
		int count2 = 0;
		for(int i = 0; i < num2; ++i)
		{
			if(enemyPieces[i] != 0)
			{
				enemyPlayablePieces[count2] = enemyPieces[i];
				count2++;
			}
		}
		//count2--; //Decrease by 1 for array index

		int kingx, kingy;
		//Locate the king
		std::string coord = "  ";
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() == colour)
					{
						if(myBoard[i][j]->whatPiece() == 'K' || myBoard[i][j]->whatPiece() == 'k')
						{
							kingx = i;
							kingy = j;
						}
					}
				}
			}
		}
		//Check if the piece has a safe place to go to
		int possibleSafeNumKing = myBoard[kingx][kingy]->numberPossibleMoves(myBoard);
		std::string *possibleSafeMovesKing = myBoard[kingx][kingy]->possibleMoves(myBoard);
		bool safeKing = true;
		for(int q = 0; q < possibleSafeNumKing; ++q)
		{
			for(int z = 0; z < count2; ++ z)
			{
				if(enemyPlayablePieces[z]->isLegal(myBoard, possibleSafeMovesKing[q]))
				{
					//std::cerr << "avoid cap" << std::endl;
					safeKing = false;
					avoidCaptureKing = false;
					break;
				}
				else
				{
					safeKing = true;
					avoidCaptureKing = true;
					from = myBoard[kingx][kingy]->coord;
					to = possibleSafeMovesKing[q];
				}
			}
			if(safeKing) break;
		}
		delete [] possibleSafeMovesKing;
	}

	//Check for capturing move
	bool captureMove = false;
	if(!avoidCaptureKing)
	{
		for(int i = 0; i < count; ++i)
		{
			//std::cerr << "capture try" << std::endl;
			int numPossibleHere = myPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *tempPossibleMoves = myPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleHere; ++j)
			{
				int y1 = tempPossibleMoves[j][0] - 'a';
				int x1 = 8 - (tempPossibleMoves[j][1] - '1' + 1);
				if(myBoard[x1][y1] != 0)
				{
					if(myBoard[x1][y1]->getColour() != colour)
					{
						//std::cerr <<"cap found" << std::endl;
						if(myBoard[x1][y1]->whatPiece() == 'k' || myBoard[x1][y1]->whatPiece() == 'K')
						{
							from = myPlayablePieces[i]->coord;
							to = tempPossibleMoves[j];
							captureMove = true;
							break;
						}
						else if(!captureMove)
						{
							from = myPlayablePieces[i]->coord;
							to = tempPossibleMoves[j];
							captureMove = true;
						}
					}
				}
			}
			delete [] tempPossibleMoves;
		}
	}

	//Check if check
	bool checkMove = false;
	if(!captureMove && !avoidCaptureKing) //IF there is no capturing move
	{
		//Locate enemy king
		std::string coord2 = "  ";
		bool kingFound = false;
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() != colour)
					{
						if((myBoard[i][j]->whatPiece() == 'K' && colour == 'b') || (myBoard[i][j]->whatPiece() == 'k' && colour == 'w'))
						{
							//std::cerr << "king found" << std::endl;
							char x = j + 'a';
							char y = 7 - i + '1';
							coord2[0] = x;
							coord2[1] = y;
							kingFound = true;
							break;
						}
					}
				}
			}
			if(kingFound) break;
		}
		for(int i = 0; i < count; ++i)
		{
			if(myPlayablePieces[i]->whatPiece() == 'k' || myPlayablePieces[i]->whatPiece() == 'K') continue;
			int numPossibleHere2 = myPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *tempPossibleMoves2 = myPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleHere2; ++j)
			{
				int x5 = 8 - (tempPossibleMoves2[j][1] - '1' + 1);
				int y5 = tempPossibleMoves2[j][0] - 'a';
				char piece = myPlayablePieces[i]->whatPiece();
				//std::cout << "here" << " " << piece << " " << tempPossibleMoves2[j] << " "<< x5 << " " << y5 << std::endl;
				int xP = 8 - (myPlayablePieces[i]->coord[1] - '1' + 1);
				int yP = myPlayablePieces[i]->coord[0] - 'a';
				Piece *tempPiece = myBoard[xP][yP];
				myBoard[xP][yP] = 0;
				if(piece == 'R' || piece == 'r') myBoard[x5][y5] = new Rook(tempPossibleMoves2[j], colour);
				else if(piece == 'N' || piece == 'n') myBoard[x5][y5] = new Knight(tempPossibleMoves2[j], colour);
				else if(piece == 'B' || piece == 'b') myBoard[x5][y5] = new Bishop(tempPossibleMoves2[j], colour);
				else if(piece == 'Q' || piece == 'q') myBoard[x5][y5] = new Queen(tempPossibleMoves2[j], colour);
				else if(piece == 'P' || piece == 'p') myBoard[x5][y5] = new Pawn(tempPossibleMoves2[j], colour);

				std::string *tempPossibleMoves5 = myBoard[x5][y5]->possibleMoves(myBoard);
				for(int z = 0; z < myBoard[x5][y5]->numberPossibleMoves(myBoard); ++z)
				{
					if(tempPossibleMoves5[z] == coord2)
					{
						//std::cerr << "check found" << std::endl;
						from = myPlayablePieces[i]->coord;
						to = tempPossibleMoves2[j];
						checkMove = true;
						break;
					}
				}
				delete myBoard[x5][y5];
				myBoard[x5][y5] = 0;
				myBoard[xP][yP] = tempPiece;
				if(checkMove) break;
				// std::cout << "end" << std::endl;
				delete tempPossibleMoves5;
			}
        delete [] tempPossibleMoves2;
		}
	}

	if(!checkMove && !captureMove && !avoidCaptureKing) //If there is no checking move or capturing move, do random move
	{
		//std::cerr << "random move" << std::endl;
		//Pick a random piece to move
		int random = rand() % count;
		from = myPlayablePieces[random]->coord;

		//Pick a random move
		std::string *possibleMoves = myPlayablePieces[random]->possibleMoves(myBoard);
		int numPossible = myPlayablePieces[random]->numberPossibleMoves(myBoard);
		int random2 = rand() % numPossible;
		to = possibleMoves[random2];
		delete [] possibleMoves;
	}

	//Execute move
	theGrid->movePiece(from, to);
	//std::cerr << "Level two calc ended" << std::endl;
	//delete [] myPlayablePieces;

	lastFrom = from;
	lastTo = to;
}

void Computer::levelThree(Grid *theGrid)
{
	srand(time(NULL)); //Set random seed

	Piece ***myBoard = theGrid->myGrid;
	std::cout << "Level Three initiated" << std::endl;
	Piece *myPieces[16];
	int num = 0;
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myBoard[i][j] != 0)
			{
				if(myBoard[i][j]->getColour() == colour)
				{
					myPieces[num] = myBoard[i][j];
					num++;
				}
			}
		}
	}
	//num--; //Decrease by 1 for array index

	//Remove pieces that do not have legal moves
	for(int i = 0; i < num; ++i)
	{
		if(!myPieces[i]->hasLegal(myBoard))
		{
			myPieces[i] = 0;
		}
	}

	//Transfer to new array
	//Piece **myPlayablePieces = new Piece*[num];
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
	//count--; //Decrease by 1 for array index

	Piece *enemyPieces[16];
	int num2 = 0;
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myBoard[i][j] != 0)
			{
				if(myBoard[i][j]->getColour() != colour)
				{
					enemyPieces[num2] = myBoard[i][j];
					num2++;
				}
			}
		}
	}
	//num2--; //Decrease by 1 for array index

	//Remove pieces that do not have legal moves
	for(int i = 0; i < num2; ++i)
	{
		if(!enemyPieces[i]->hasLegal(myBoard))
		{
			enemyPieces[i] = 0;
		}
	}

	//Transfer to new array
	Piece *enemyPlayablePieces[num2];
	int count2 = 0;
	for(int i = 0; i < num2; ++i)
	{
		if(enemyPieces[i] != 0)
		{
			enemyPlayablePieces[count2] = enemyPieces[i];
			count2++;
		}
	}
	//count2--; //Decrease by 1 for array index

	std::string from;
	std::string to;
	//If computer is in check, avoid
	bool avoidCaptureKing = false;
	if(theGrid->isInCheck(colour))
	{
		int kingx, kingy;
		//Locate the king
		std::string coord = "  ";
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() == colour)
					{
						if(myBoard[i][j]->whatPiece() == 'K' || myBoard[i][j]->whatPiece() == 'k')
						{
							kingx = i;
							kingy = j;
						}
					}
				}
			}
		}
		//Check if the piece has a safe place to go to
		int possibleSafeNumKing = myBoard[kingx][kingy]->numberPossibleMoves(myBoard);
		std::string *possibleSafeMovesKing = myBoard[kingx][kingy]->possibleMoves(myBoard);
		bool safeKing = true;
		for(int q = 0; q < possibleSafeNumKing; ++q)
		{
			for(int z = 0; z < count2; ++ z)
			{
				if(enemyPlayablePieces[z]->isLegal(myBoard, possibleSafeMovesKing[q]))
				{
					//std::cerr << "avoid cap" << std::endl;
					safeKing = false;
					avoidCaptureKing = false;
					break;
				}
				else
				{
					safeKing = true;
					avoidCaptureKing = true;
					from = myBoard[kingx][kingy]->coord;
					to = possibleSafeMovesKing[q];
				}
			}
			if(safeKing) break;
		}
		delete [] possibleSafeMovesKing;
	}

	//Check for avoiding capture
	bool avoidCapture = false;
	if(!avoidCaptureKing)
	{
		for(int i = 0; i < count2; ++i)
		{
			//std::cerr << "Check avoid Cap" << std::endl;
			int numPossibleEnemy = enemyPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *enemyPossibleMoves = enemyPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleEnemy; ++j)
			{
				int y3 = enemyPossibleMoves[j][0] - 'a';
				int x3 = 8 - (enemyPossibleMoves[j][1] - '1' + 1);
				if(myBoard[x3][y3] != 0)
				{
					if(myBoard[x3][y3]->getColour() == colour && myBoard[x3][y3]->hasLegal(myBoard))
					{
						//Check if the piece has a safe place to go to
						int possibleSafeNum = myBoard[x3][y3]->numberPossibleMoves(myBoard);
						std::string *possibleSafeMoves = myBoard[x3][y3]->possibleMoves(myBoard);
						bool safe = true;
						for(int q = 0; q < possibleSafeNum; ++q)
						{
							for(int z = 0; z < count2; ++ z)
							{
								if(enemyPlayablePieces[z]->isLegal(myBoard, possibleSafeMoves[q]))
								{
									//std::cerr << "avoid cap" << std::endl;
									safe = false;
									avoidCapture = false;
									break;
								}
								else
								{
									safe = true;
									avoidCapture = true;
									from = myBoard[x3][y3]->coord;
									to = possibleSafeMoves[q];
								}
							}
							if(safe) break;
						}
						delete [] possibleSafeMoves;
					}
				}
			}
			delete [] enemyPossibleMoves;
			if(avoidCapture) break;
		}
	}

	//Check for capturing move
	bool captureMove = false;
	if(!avoidCapture && !avoidCaptureKing) //Checks if no avoiding capture move
	{
		//std::cerr << "cap move try" << std::endl;
		for(int i = 0; i < count; ++i)
		{
			int numPossibleHere = myPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *tempPossibleMoves = myPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleHere; ++j)
			{
				int y1 = tempPossibleMoves[j][0] - 'a';
				int x1 = 8 - (tempPossibleMoves[j][1] - '1' + 1);
				if(myBoard[x1][y1] != 0)
				{
					if(myBoard[x1][y1]->getColour() != colour)
					{
						//std::cerr << "found enemy" << std::endl;
						if(myBoard[x1][y1]->whatPiece() == 'k' || myBoard[x1][y1]->whatPiece() == 'K')
						{
							from = myPlayablePieces[i]->coord;
							to = tempPossibleMoves[j];
							captureMove = true;
							break;
						}
						else if(!captureMove)
						{
							from = myPlayablePieces[i]->coord;
							to = tempPossibleMoves[j];
							captureMove = true;
						}
					}
				}
			}
			delete [] tempPossibleMoves;
		}
	}

	//Check if check
	bool checkMove = false;
	if(!captureMove && !avoidCapture && !avoidCaptureKing) //IF there is no capturing move
	{
		//Locate enemy king
		std::string coord2 = "  ";
		bool kingFound = false;
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() != colour)
					{
						if((myBoard[i][j]->whatPiece() == 'K' && colour == 'b') || (myBoard[i][j]->whatPiece() == 'k' && colour == 'w'))
						{
							//std::cerr << "king found" << std::endl;
							char x = j + 'a';
							char y = 7 - i + '1';
							coord2[0] = x;
							coord2[1] = y;
							kingFound = true;
							break;
						}
					}
				}
			}
			if(kingFound) break;
		}
		for(int i = 0; i < count; ++i)
		{
			if(myPlayablePieces[i]->whatPiece() == 'k' || myPlayablePieces[i]->whatPiece() == 'K') continue;
			int numPossibleHere2 = myPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *tempPossibleMoves2 = myPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleHere2; ++j)
			{
				int x5 = 8 - (tempPossibleMoves2[j][1] - '1' + 1);
				int y5 = tempPossibleMoves2[j][0] - 'a';
				char piece = myPlayablePieces[i]->whatPiece();
				//std::cout << "here" << " " << piece << " " << tempPossibleMoves2[j] << " "<< x5 << " " << y5 << std::endl;
				int xP = 8 - (myPlayablePieces[i]->coord[1] - '1' + 1);
				int yP = myPlayablePieces[i]->coord[0] - 'a';
				Piece *tempPiece = myBoard[xP][yP];
				myBoard[xP][yP] = 0;
				if(piece == 'R' || piece == 'r') myBoard[x5][y5] = new Rook(tempPossibleMoves2[j], colour);
				else if(piece == 'N' || piece == 'n') myBoard[x5][y5] = new Knight(tempPossibleMoves2[j], colour);
				else if(piece == 'B' || piece == 'b') myBoard[x5][y5] = new Bishop(tempPossibleMoves2[j], colour);
				else if(piece == 'Q' || piece == 'q') myBoard[x5][y5] = new Queen(tempPossibleMoves2[j], colour);
				else if(piece == 'P' || piece == 'p') myBoard[x5][y5] = new Pawn(tempPossibleMoves2[j], colour);

				std::string *tempPossibleMoves5 = myBoard[x5][y5]->possibleMoves(myBoard);
				for(int z = 0; z < myBoard[x5][y5]->numberPossibleMoves(myBoard); ++z)
				{
					if(tempPossibleMoves5[z] == coord2)
					{
						//std::cerr << "check found" << std::endl;
						from = myPlayablePieces[i]->coord;
						to = tempPossibleMoves2[j];
						checkMove = true;
						break;
					}
				}
				delete myBoard[x5][y5];
				myBoard[x5][y5] = 0;
				myBoard[xP][yP] = tempPiece;
				if(checkMove) break;
				// std::cout << "end" << std::endl;
				delete [] tempPossibleMoves5;
			}
			delete [] tempPossibleMoves2;
		}
	}

	if(!checkMove && !captureMove && !avoidCapture && !avoidCaptureKing) //If there is no checking move or capturing move or avoid cap, do random move
	{
		//std::cerr << "random move" << std::endl;
		//Pick a random piece to move
		int random = rand() % count;
		from = myPlayablePieces[random]->coord;

		//Pick a random move
		std::string *possibleMoves = myPlayablePieces[random]->possibleMoves(myBoard);
		int numPossible = myPlayablePieces[random]->numberPossibleMoves(myBoard);
		int random2 = rand() % numPossible;
		to = possibleMoves[random2];
		delete [] possibleMoves;
	}

	//Execute move
	theGrid->movePiece(from, to);
	//std::cerr << "Level three calc ended" << std::endl;
	//delete [] myPlayablePieces;
	//delete [] enemyPlayablePieces;
	lastFrom = from;
	lastTo = to;
}

void Computer::levelFour(Grid *theGrid)
{
    srand(time(NULL)); //Set random seed

	Piece ***myBoard = theGrid->myGrid;
	std::cout << "Level Four initiated" << std::endl;
	Piece *myPieces[16];
	int num = 0;
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myBoard[i][j] != 0)
			{
				if(myBoard[i][j]->getColour() == colour)
				{
					myPieces[num] = myBoard[i][j];
					num++;
				}
			}
		}
	}
	//num--; //Decrease by 1 for array index

	//Remove pieces that do not have legal moves
	for(int i = 0; i < num; ++i)
	{
		if(!myPieces[i]->hasLegal(myBoard))
		{
			myPieces[i] = 0;
		}
	}

	//Transfer to new array
	//Piece **myPlayablePieces = new Piece*[num];
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
	//count--; //Decrease by 1 for array index

	Piece *enemyPieces[16];
	int num2 = 0;
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(myBoard[i][j] != 0)
			{
				if(myBoard[i][j]->getColour() != colour)
				{
					enemyPieces[num2] = myBoard[i][j];
					num2++;
				}
			}
		}
	}
	//num2--; //Decrease by 1 for array index

	//Remove pieces that do not have legal moves
	for(int i = 0; i < num2; ++i)
	{
		if(!enemyPieces[i]->hasLegal(myBoard))
		{
			enemyPieces[i] = 0;
		}
	}

	//Transfer to new array
	Piece *enemyPlayablePieces[num2];
	int count2 = 0;
	for(int i = 0; i < num2; ++i)
	{
		if(enemyPieces[i] != 0)
		{
			enemyPlayablePieces[count2] = enemyPieces[i];
			count2++;
		}
	}
	//count2--; //Decrease by 1 for array index


	std::string from;
	std::string to;

	//If computer is in check, avoid
	bool avoidCaptureKing = false;
	if(theGrid->isInCheck(colour))
	{
		int kingx, kingy;
		//Locate the king
		std::string coord = "  ";
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() == colour)
					{
						if(myBoard[i][j]->whatPiece() == 'K' || myBoard[i][j]->whatPiece() == 'k')
						{
							kingx = i;
							kingy = j;
						}
					}
				}
			}
		}
		//Check if the piece has a safe place to go to
		int possibleSafeNumKing = myBoard[kingx][kingy]->numberPossibleMoves(myBoard);
		std::string *possibleSafeMovesKing = myBoard[kingx][kingy]->possibleMoves(myBoard);
		bool safeKing = true;
		for(int q = 0; q < possibleSafeNumKing; ++q)
		{
			for(int z = 0; z < count2; ++ z)
			{
				if(enemyPlayablePieces[z]->isLegal(myBoard, possibleSafeMovesKing[q]))
				{
					//std::cerr << "avoid cap" << std::endl;
					safeKing = false;
					avoidCaptureKing = false;
					break;
				}
				else
				{
					safeKing = true;
					avoidCaptureKing = true;
					from = myBoard[kingx][kingy]->coord;
					to = possibleSafeMovesKing[q];
				}
			}
			if(safeKing) break;
		}
		delete [] possibleSafeMovesKing;
	}

	//Check for capturing move
	bool captureMove = false;
	if(!avoidCaptureKing)
	{
		for(int i = 0; i < count; ++i)
		{
			int numPossibleHere = myPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *tempPossibleMoves = myPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleHere; ++j)
			{
				int y1 = tempPossibleMoves[j][0] - 'a';
				int x1 = 8 - (tempPossibleMoves[j][1] - '1' + 1);
				if(myBoard[x1][y1] != 0)
				{
					if(myBoard[x1][y1]->getColour() != colour)
					{
						//std::cerr << "found enemy" << std::endl;
						if(myBoard[x1][y1]->whatPiece() == 'k' || myBoard[x1][y1]->whatPiece() == 'K')
						{
							from = myPlayablePieces[i]->coord;
							to = tempPossibleMoves[j];
							captureMove = true;
							break;
						}
						else if(!captureMove)
						{
							//std::cout << "cap" << std::endl;
							bool safe = true;
							//Check if the capture move location is either safe from enemy pieces or protected by allied pieces
							for(int z = 0; z < count2; ++z)
							{
								if(enemyPlayablePieces[z]->isLegal(myBoard, tempPossibleMoves[j]))
								{
									//std::cerr << "avoid cap" << std::endl;
									safe = false;
									break;
								}
								else
								{
									safe = true;
									from = myPlayablePieces[i]->coord;
									to = tempPossibleMoves[j];
									captureMove = true;
								}
							}
							//If it is not safe, check if allied piece is protecting
							if(!safe)
							{
								//std::cout << "check protect" << std::endl;
								for(int z = 0; z < count; ++z)
								{
									std::string *superTempPossibleMoves = myPlayablePieces[z]->possibleMoves(myBoard);
									for(int a = 0; a < myPlayablePieces[z]->numberPossibleMoves(myBoard); ++a)
									{
										if(superTempPossibleMoves[a] == tempPossibleMoves[j])
										{
											from = myPlayablePieces[i]->coord;
											to = tempPossibleMoves[j];
											captureMove = true;
											break;
										}
									}
									delete [] superTempPossibleMoves;
								}
							}
						}
					}
				}
				if(captureMove) break;
			}
			delete [] tempPossibleMoves;
		}
	}

	//Check for avoiding capture
	bool avoidCapture = false;
	if(!captureMove && !avoidCaptureKing)
	{
		for(int i = 0; i < count2; ++i)
		{
			//std::cerr << "Check avoid Cap" << std::endl;
			int numPossibleEnemy = enemyPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *enemyPossibleMoves = enemyPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleEnemy; ++j)
			{
				int y3 = enemyPossibleMoves[j][0] - 'a';
				int x3 = 8 - (enemyPossibleMoves[j][1] - '1' + 1);
				if(myBoard[x3][y3] != 0)
				{
					if(myBoard[x3][y3]->getColour() == colour && myBoard[x3][y3]->hasLegal(myBoard))
					{
						//Check if the piece has a safe place to go to
						int possibleSafeNum = myBoard[x3][y3]->numberPossibleMoves(myBoard);
						std::string *possibleSafeMoves = myBoard[x3][y3]->possibleMoves(myBoard);
						bool safe = true;
						for(int q = 0; q < possibleSafeNum; ++q)
						{
							for(int z = 0; z < count2; ++ z)
							{
								if(enemyPlayablePieces[z]->isLegal(myBoard, possibleSafeMoves[q]))
								{
									//std::cerr << "avoid cap" << std::endl;
									safe = false;
									avoidCapture = false;
									break;
								}
								else
								{
									safe = true;
									avoidCapture = true;
									from = myBoard[x3][y3]->coord;
									to = possibleSafeMoves[q];
								}
							}
							if(safe) break;
						}
						delete [] possibleSafeMoves;
					}
				}
			}
			delete [] enemyPossibleMoves;
			if(avoidCapture) break;
		}
	}

	//Check if check
	bool checkMove = false;
	if(!captureMove && !avoidCapture && !avoidCaptureKing) //IF there is no capturing move
	{
		//Locate enemy king
		std::string coord2 = "  ";
		bool kingFound = false;
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				if(myBoard[i][j] != 0)
				{
					if(myBoard[i][j]->getColour() != colour)
					{
						if((myBoard[i][j]->whatPiece() == 'K' && colour == 'b') || (myBoard[i][j]->whatPiece() == 'k' && colour == 'w'))
						{
							//std::cerr << "king found" << std::endl;
							char x = j + 'a';
							char y = 7 - i + '1';
							coord2[0] = x;
							coord2[1] = y;
							kingFound = true;
							break;
						}
					}
				}
			}
			if(kingFound) break;
		}
		for(int i = 0; i < count; ++i)
		{
			if(myPlayablePieces[i]->whatPiece() == 'k' || myPlayablePieces[i]->whatPiece() == 'K') continue;
			int numPossibleHere2 = myPlayablePieces[i]->numberPossibleMoves(myBoard);
			std::string *tempPossibleMoves2 = myPlayablePieces[i]->possibleMoves(myBoard);
			for(int j = 0; j < numPossibleHere2; ++j)
			{
				int x5 = 8 - (tempPossibleMoves2[j][1] - '1' + 1);
				int y5 = tempPossibleMoves2[j][0] - 'a';
				char piece = myPlayablePieces[i]->whatPiece();
				//std::cout << "here" << " " << piece << " " << tempPossibleMoves2[j] << " "<< x5 << " " << y5 << std::endl;
				int xP = 8 - (myPlayablePieces[i]->coord[1] - '1' + 1);
				int yP = myPlayablePieces[i]->coord[0] - 'a';
				Piece *tempPiece = myBoard[xP][yP];
				myBoard[xP][yP] = 0;
				if(piece == 'R' || piece == 'r') myBoard[x5][y5] = new Rook(tempPossibleMoves2[j], colour);
				else if(piece == 'N' || piece == 'n') myBoard[x5][y5] = new Knight(tempPossibleMoves2[j], colour);
				else if(piece == 'B' || piece == 'b') myBoard[x5][y5] = new Bishop(tempPossibleMoves2[j], colour);
				else if(piece == 'Q' || piece == 'q') myBoard[x5][y5] = new Queen(tempPossibleMoves2[j], colour);
				else if(piece == 'P' || piece == 'p') myBoard[x5][y5] = new Pawn(tempPossibleMoves2[j], colour);

				std::string *tempPossibleMoves5 = myBoard[x5][y5]->possibleMoves(myBoard);
				for(int z = 0; z < myBoard[x5][y5]->numberPossibleMoves(myBoard); ++z)
				{
					if(tempPossibleMoves5[z] == coord2)
					{
						//std::cerr << "check found" << std::endl;
						from = myPlayablePieces[i]->coord;
						to = tempPossibleMoves2[j];
						checkMove = true;
						break;
					}
				}
				delete myBoard[x5][y5];
				myBoard[x5][y5] = 0;
				myBoard[xP][yP] = tempPiece;
				if(checkMove) break;
				// std::cout << "end" << std::endl;
				delete [] tempPossibleMoves5;
			}
			delete [] tempPossibleMoves2;
		}
	}

	if(!checkMove && !captureMove && !avoidCapture && !avoidCaptureKing) //If there is no checking move or capturing move or avoid cap, do random move
	{
		//std::cerr << "random move" << std::endl;
		//Pick a random piece to move
		int random = rand() % count;
		from = myPlayablePieces[random]->coord;

		//Pick a random move
		std::string *possibleMoves = myPlayablePieces[random]->possibleMoves(myBoard);
		int numPossible = myPlayablePieces[random]->numberPossibleMoves(myBoard);
		int random2 = rand() % numPossible;
		to = possibleMoves[random2];
		delete [] possibleMoves;
	}

	//Execute move
	theGrid->movePiece(from, to);
	//std::cerr << "Level four calc ended" << std::endl;
	//delete [] myPlayablePieces;
	//delete [] enemyPlayablePieces;
	lastFrom = from;
	lastTo = to;
}

void Computer::generateMove(Grid *myBoard)
{
	//std::cerr << "Generate Move initiated" << std::endl;
	if(level == 1) levelOne(myBoard);
	else if(level == 2) levelTwo(myBoard);
	else if(level == 3) levelThree(myBoard);
	else if(level == 4) levelFour(myBoard);
}

std::string Computer::getLastFrom()
{
	return lastFrom;
}

std::string Computer::getLastTo()
{
	return lastTo;
}

Computer::~Computer() {}
