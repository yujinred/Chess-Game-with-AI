#include "queen.h"
#include <stdlib.h>
#include <iostream>

Queen::Queen(std::string coord, char colour): Piece(coord, colour){}

bool Queen::isLegal(Piece*** myPieces, std::string pos) {
    if (pos.length() != 2) return 0;
    if ((pos[0] >= 'a') && (pos[0] <= 'h')) {
        if ((pos[1] >= '1') && (pos[1] <= '8')) {
            //std::cerr << "Correct Position THE QUEEN " << std::endl;
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
            if (!(coord[0] == pos[0])) {
                // Diagonal Algorithm
                if (!(coord[1] == pos[1])) {
                    if (abs(coord[0]-pos[0]) != abs(coord[1]-pos[1])) return 0;
                    else {
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
                                   // std::cerr << "myPieces[" << rowPosNum+k << "][" << colPosNum-k << "]" << std::endl;
                                    if (myPieces[rowPosNum+k][colPosNum-k] != NULL) return 0;
                                }
                            }
                        } else {                        // Pos is on the bottom of coord
                            if (coord[0] > pos[0]) {    // bottom-left
                               // std::cerr << "Pos is on Bottom-Left" << std::endl;
                                for (int k = 1; k < rowPosNum - rowNum; k++) {
                                   // std::cerr << "myPieces[" << rowNum+k << "][" << colNum-k << "]" << std::endl;
                                    if (myPieces[rowNum+k][colNum-k] != NULL) return 0;
                                }
                            } else {                    // bottom-right
                               // std::cerr << "Pos is on Bottom-Right" << std::endl;
                                for (int k = 1; k < rowPosNum - rowNum; k++) {
                                    //std::cerr << "myPieces[" << rowNum+k << "][" << colNum+k << "]" << std::endl;
                                    if (myPieces[rowNum+k][colNum+k] != NULL) return 0;
                                }
                            }
                        }
                    }
                }
                else {
                   // std::cout << "Horizontal move " << std::endl;
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
                // If the player didn't move the piece, then it is illegal
                if (coord[1] == pos[1]) return 0;
                else {
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
   // std::cerr << "QUEEN" << std::endl;
    return 1;
}

char Queen::whatPiece() {
    if (colour == 'b') {
        return 'q';
    } else {
        return 'Q';
    }
}

Queen::~Queen() {}
