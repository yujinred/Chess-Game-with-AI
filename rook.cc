#include "rook.h"
#include <iostream>
#include <string>

Rook::Rook(std::string coord, char colour): Piece(coord, colour), isMoved(0) {}

bool Rook::isLegal(Piece*** myPieces, std::string pos) {
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

            // Check if there is any pieces in rook's path
            if (!(coord[0] == pos[0])) {
                //std::cout << "sdtcoord[1]=" << coord[1] << "pos[1]=" << pos[1] << std::endl;
                if (!(coord[1] == pos[1])) return 0;
                else {
                    //std::cout << "Horizontal move " << std::endl;
                    // Horizontal Movement
                    if (colNum > colPosNum) {
                        for (int k=colPosNum + 1; k < colNum; k++) {
                            //std::cerr << "testing: myPieces[" << rowNum << "][" << k <<"] == " << myPieces[rowNum][k] << std::endl;
                            if (myPieces[rowNum][k] != NULL) return 0;
                        }
                    } else {
                        for (int k = colNum + 1; k < colPosNum; k++) {
                            //std::cerr << "testing: myPieces[" << rowNum << "][" << k <<"] == " << myPieces[rowNum][k] << std::endl;
                            if (myPieces[rowNum][k] != NULL) return 0;
                        }
                    }
                }
            } else {
                //std::cout << "coord[1]=" << coord[1] << "pos[1]=" << pos[1] << std::endl;
                // If the player didn't move the piece, then it is illegal
                if (coord[1] == pos[1]) return 0;
                else {
                    // Vertical Movement
                    //std::cerr << "VERTICAL MOVEMENT" << std::endl;
                    if (rowNum > rowPosNum) {
                        for (int k = rowPosNum + 1; k < rowNum; k++) {
                           // std::cerr << "testing: myPieces[" << k << "][" << colNum <<"] == " << myPieces[k][colNum] << std::endl;
                            if (myPieces[k][colNum] != NULL) return 0;
                        }
                    }
                    else {
                        for (int k = rowNum + 1; k < rowPosNum; k++) {
                           // std::cerr << "testing: myPieces[" << k << "][" << colNum <<"] == " << myPieces[k][colNum] << std::endl;
                            if (myPieces[k][colNum] != NULL) return 0;
                        }
                    }
                }
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    //std::cerr << "ROOK" << std::endl;
    return 1;
}

void Rook::setIsMoved(bool state) {
    isMoved = state;
}

bool Rook::canCastling(Piece*** myPieces) {
    if (isMoved == 1) return 0;
    return 1;
}

char Rook::whatPiece() {
    if (colour == 'b') {
        return 'r';
    } else {
        return 'R';
    }
}

Rook::~Rook() {}

