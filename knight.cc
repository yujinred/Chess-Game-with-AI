#include "knight.h"
#include <stdlib.h>
#include <iostream>

Knight::Knight(std::string coord, char colour): Piece(coord, colour){}

bool Knight::isLegal(Piece*** myPieces, std::string pos) {
    if (pos.length() != 2) return 0;
    // Check if the new pos is within the region of the board
    if ((pos[0] >= 'a') && (pos[0] <= 'h')) {
        if ((pos[1] >= '1') && (pos[1] <= '8')) {
            //std::cerr << "Correct Position " << std::endl;
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

            // Movement check
            if (!(((abs(colNum-colPosNum) == 1) && (abs(rowNum-rowPosNum) == 2)) ||
                ((abs(colNum-colPosNum) == 2) && (abs(rowNum-rowPosNum) == 1))))
                return 0;

        } else {
            return 0;
        }
    } else {
        return 0;
    }
    //std::cerr << "KNIGHT" << std::endl;
    return 1;
}

char Knight::whatPiece() {
    if (colour == 'b') {
        return 'n';
    } else {
        return 'N';
    }
}

Knight::~Knight() {}
