#include "pawn.h"
#include <iostream>
#include <stdlib.h>

Pawn::Pawn(std::string coord, char colour): Piece(coord, colour), justTwiceStep(0) {}

bool Pawn::isLegal(Piece*** myPieces, std::string pos) {
    if (pos.length() != 2) return 0;
    // Check if the new pos is within the region of the board
    if ((pos[0] >= 'a') && (pos[0] <= 'h')) {
        if ((pos[1] >= '1') && (pos[1] <= '8')) {
            //std::cout << "Correct Position " << std::endl;
            int colNum = coord[0]-'a';
            int rowNum = 8 - (coord[1]-'1' + 1);
            int colPosNum = pos[0]-'a';
            int rowPosNum = 8 - (pos[1]-'1' + 1);
            // Movement check
            // Check if capture own piece
            if (myPieces[rowPosNum][colPosNum] != NULL) {
                if (myPieces[rowPosNum][colPosNum]->getColour() == colour) {
                    //std::cerr << "Capturing own Pieces" << std::endl;
                    return 0;
                }
            }
            // Pawn capture differently depending on their side
            int pawnMoveCheck;
            if (colour == 'w') pawnMoveCheck = rowNum - rowPosNum;
            else if (colour == 'b') pawnMoveCheck = rowPosNum - rowNum;
            if (pawnMoveCheck == 1) {
                //std::cerr << "Pawn moving one space " << std::endl;
                if (myPieces[rowPosNum][colPosNum] != NULL) {
                    //std::cerr << "Pos already have piece" << std::endl;
                    if (abs(colNum - colPosNum) != 1) return 0;
                } else {
                    //std::cerr << "Pos Empty" << std::endl;
                    if (colNum != colPosNum) return 0;
                }
                justTwiceStep = 0;
            } else if (pawnMoveCheck == 2) {
                //std::cerr << "Pawn First move" << std::endl;
                if (colNum != colPosNum) {
                    //std::cerr << "Different column" << std::endl;
                    return 0;
                }
                else if ((rowNum != 1) && (rowNum != 6)) {
                   // std::cerr << "row isn't on starting: row is on " << rowNum << std::endl;
                    return 0;
                } else {
                    if (myPieces[(rowNum+rowPosNum)/2][colPosNum] != NULL) {
                        //std::cerr << "Pawn is blocked" << std::endl;
                        return 0;
                    }
                    if (myPieces[rowPosNum][colPosNum] != NULL) {
                        //std::cerr << "Can't two step capture" << std::endl;
                        return 0;
                    }
                    justTwiceStep = 1;
                }

            } else {
                return 0;
            }

        } else {
            return 0;
        }
    } else {
        return 0;
    }
    //std::cerr << "PAWN" << std::endl;
    return 1;
}

bool Pawn::isChecking(Piece*** myPieces) {
    int colNum = coord[0]-'a';
    int rowNum = 8 - (coord[1]-'1' + 1);
    int pawnDir;
    if (colour == 'w') pawnDir = -1;
    else if (colour == 'b') pawnDir = 1;
    int kingRowPos = rowNum + pawnDir;
    char PieceAtPos;
    if (colNum-1 > 0){
        if (myPieces[kingRowPos][colNum-1] != NULL) {
            PieceAtPos = myPieces[kingRowPos][colNum-1]->whatPiece();
            if ((PieceAtPos == 'k') || (PieceAtPos == 'K')) {
                return 1;
            }
        }
    }
    if (colNum+1 < 8) {
        if (myPieces[kingRowPos][colNum+1] != NULL) {
            PieceAtPos = myPieces[kingRowPos][colNum+1]->whatPiece();
            if ((PieceAtPos == 'k') || (PieceAtPos == 'K')) {
                return 1;
            }

        }
    }
    return 0;
}

bool Pawn::getJustTwiceStep() {
    return justTwiceStep;
}

void Pawn::setJustTwiceStep(bool state) {
    justTwiceStep = state;
}

char Pawn::whatPiece() {
    if (colour == 'b') {
        return 'p';
    } else {
        return 'P';
    }
}

Pawn::~Pawn() {}
