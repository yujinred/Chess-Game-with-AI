#include "bishop.h"
#include <stdlib.h>
#include <iostream>

Bishop::Bishop(std::string coord, char colour): Piece(coord, colour){}

bool Bishop::isLegal(Piece*** myPieces, std::string pos) {
    if (pos.length() != 2) return 0;
    // Check if the new pos is within the region of the board
    if ((pos[0] >= 'a') && (pos[0] <= 'h')) {
        if ((pos[1] >= '1') && (pos[1] <= '8')) {
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
            // check rook-type moves
            if ((coord[0] == pos[0]) || (coord[1] == pos[1])) return 0;
            // check valid diagonal
            if (abs(coord[0]-pos[0]) != abs(coord[1]-pos[1])) {
                //std::cerr << coord[0] << " - " << pos[0] << " = " << coord[0] - pos[0] << std::endl;
                return 0;
            }
            else {
                //std::cerr << coord[0] << " - " << pos[0] << " = " << coord[0] - pos[0] << std::endl;
                if (coord[1] < pos[1]) {        // Pos is top of coord
                    if (coord[0] > pos[0]) {    // Pos is top-left of coord
                        //std::cerr << "Pos is on Top-Left" << std::endl;
                        for (int k = 1; k < rowNum - rowPosNum; k++) {
                            //std::cerr << "myPieces[" << rowPosNum+k << "][" << colPosNum+k << "]" << std::endl;
                            if (myPieces[rowPosNum+k][colPosNum+k] != NULL) return 0;
                        }
                    } else {                    // top-right
                        //std::cerr << "Pos is on Top-Right" << std::endl;
                        for (int k = 1; k < rowNum - rowPosNum; k++) {
                            //std::cerr << "myPieces[" << rowPosNum+k << "][" << colPosNum-k << "]" << std::endl;
                            if (myPieces[rowPosNum+k][colPosNum-k] != NULL) return 0;
                        }
                    }
                } else {                        // Pos is on the bottom of coord
                    if (coord[0] > pos[0]) {    // bottom-left
                        //std::cerr << "Pos is on Bottom-Left" << std::endl;
                        for (int k = 1; k < rowPosNum - rowNum; k++) {
                            //std::cerr << "myPieces[" << rowNum+k << "][" << colNum-k << "]" << std::endl;
                            if (myPieces[rowNum+k][colNum-k] != NULL) return 0;
                        }
                    } else {                    // bottom-right
                        //std::cerr << "Pos is on Bottom-Right" << std::endl;
                        for (int k = 1; k < rowPosNum - rowNum; k++) {
                            //std::cerr << "myPieces[" << rowNum+k << "][" << colNum+k << "]" << std::endl;
                            if (myPieces[rowNum+k][colNum+k] != NULL) return 0;
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
    //std::cerr << "BISHOP" << std::endl;
    return 1;
}

char Bishop::whatPiece() {
    if (colour == 'b') {
        return 'b';
    } else {
        return 'B';
    }
}

Bishop::~Bishop(){}
