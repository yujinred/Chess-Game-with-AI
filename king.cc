#include "king.h"
#include <stdlib.h>
#include <iostream>
#include "rook.h"

King::King(std::string coord, char colour): Piece(coord, colour){}


bool King::isLegal(Piece*** myPieces, std::string pos) {
    if (pos.length() != 2) return 0;
    // Check if the new pos is within the region of the board
    if ((pos[0] >= 'a') && (pos[0] <= 'h')) {
        if ((pos[1] >= '1') && (pos[1] <= '8')) {
            //std::cerr << "Correct Position THE KING" << std::endl;
            int colNum = coord[0]-'a';
            int rowNum = 8 - (coord[1]-'1' + 1);
            int colPosNum = pos[0]-'a';
            int rowPosNum = 8 - (pos[1]-'1' + 1);

            // Check if capture own piece
            if (myPieces[rowPosNum][colPosNum] != NULL) {
                if (myPieces[rowPosNum][colPosNum]->getColour() == colour) {
                    //std::cerr << "Capturing own Pieces" << std::endl;
                    return 0;
                }
            }

            Piece* tempPiece = myPieces[rowPosNum][colPosNum];
            myPieces[rowPosNum][colPosNum] = 0;
            Piece* tempKing = myPieces[rowNum][colNum];
            myPieces[rowNum][colNum] = 0;

            // Check if king is only moving one space
            if (((abs(colNum-colPosNum) == 1) && (abs(rowNum-rowPosNum) == 1)) ||
                ((abs(colNum-colPosNum) == 0) && (abs(rowNum-rowPosNum) == 1)) ||
                ((abs(colNum-colPosNum) == 1) && (abs(rowNum-rowPosNum) == 0))) {
            // Check if King will move into Check
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if (myPieces[i][j] != NULL) {
                            if ((myPieces[i][j]->whatPiece() != 'k') && (myPieces[i][j]->whatPiece() != 'K') &&
                                (myPieces[i][j]->whatPiece() != 'p') && (myPieces[i][j]->whatPiece() != 'P')) {
                                if ((myPieces[i][j]->getColour() != colour)) {
                                    if (myPieces[i][j]->isLegal(myPieces, pos)) {
                                        if ((myPieces[i][j]->whatPiece() == 'r') || (myPieces[i][j]->whatPiece() == 'R')) {
                                            myPieces[i][j]->setIsMoved(0);
                                        }
                                        //std::cerr << "King cannot move into check." << std::endl;
                                        myPieces[rowPosNum][colPosNum] = tempPiece;
                                        myPieces[rowNum][colNum] = tempKing;
                                        return 0;
                                    }
                                }
                            // Special Pawn case: since pawn capture different than how they move
                            } else if ((myPieces[i][j]->whatPiece() == 'p') || (myPieces[i][j]->whatPiece() == 'P')) {
                                // Pawn can't move to this space by capturing since it is empty
                                if (myPieces[i][j]->getColour() != colour) {
                                    int pawnDir;
                                    if (colour == 'w') pawnDir = -1;
                                    else if (colour == 'b') pawnDir = 1;
                                    //std::cerr << (i != rowPosNum+pawnDir) << " or " << (1 != abs(colPosNum-j)) << std::endl;
                                    if ((i == rowPosNum+pawnDir) && (1 == abs(colPosNum-j))) {
                                        //std::cerr << (i != rowPosNum+pawnDir) << " or " << (1 != abs(colPosNum-j)) << std::endl;
                                        //std::cerr << "Pawn interception" << std::endl;
                                        myPieces[rowPosNum][colPosNum] = tempPiece;
                                        myPieces[rowNum][colNum] = tempKing;
                                        return 0;
                                    }
                                }
                            // Check if king can't move near another King
                            } else if ((myPieces[i][j]->whatPiece() == 'k') || (myPieces[i][j]->whatPiece() == 'K')) {
                                if (myPieces[i][j]->getColour() != colour) {
                                    if (((abs(j-colPosNum) == 1) && (abs(i-rowPosNum) == 1)) ||
                                        ((abs(j-colPosNum) == 0) && (abs(i-rowPosNum) == 1)) ||
                                        ((abs(j-colPosNum) == 1) && (abs(i-rowPosNum) == 0)))
                                    {
                                        myPieces[rowPosNum][colPosNum] = tempPiece;
                                        myPieces[rowNum][colNum] = tempKing;
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                myPieces[rowPosNum][colPosNum] = tempPiece;
                myPieces[rowNum][colNum] = tempKing;
                return 0;
            }
            myPieces[rowPosNum][colPosNum] = tempPiece;
            myPieces[rowNum][colNum] = tempKing;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    //std::cerr << "KING" << std::endl;
    setIsMoved(1);
    return 1;
}

void King::setIsMoved(bool state) {
    isMoved = state;
}

bool King::canCastling(Piece*** myPieces) {
    if (isMoved == 1) return 0;
    return 1;
}

char King::whatPiece() {
    if (colour == 'b') {
        return 'k';
    } else {
        return 'K';
    }
}

King::~King() {}
