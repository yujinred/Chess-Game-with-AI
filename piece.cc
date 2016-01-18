#include "piece.h"

Piece::Piece(std::string coord, char colour): coord(coord), colour(colour) {}

void Piece::setCoord(std::string pos) {
    coord = pos;
}

char Piece::getColour() {
    return colour;
}

bool Piece::hasLegal(Piece*** myPieces) {
    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            char x = i + 'a';
            char y = j + '1';
            std::string newCord = "  ";
            newCord[0] = x;
            newCord[1] = y;
            if (this->isLegal(myPieces, newCord)) return 1;
        }
    }
    return 0;
}

std::string* Piece::possibleMoves(Piece*** myPieces) {
    std::string* possibilities = new std::string[64];
    int count = 0;
    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            char x = i + 'a';
            char y = j + '1';
            std::string newCord = "  ";
            newCord[0] = x;
            newCord[1] = y;
            if (this->isLegal(myPieces, newCord)) {
                possibilities[count] = newCord;
                count++;
            }
        }
    }
    return possibilities;
}

int Piece::numberPossibleMoves(Piece*** myPieces) {
    int count = 0;
    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            char x = i + 'a';
            char y = j + '1';
            std::string newCord = "  ";
            newCord[0] = x;
            newCord[1] = y;
            if (this->isLegal(myPieces, newCord)) {
                count++;
            }
        }
    }
    return count;
}

bool Piece::isLegal(Piece*** myPieces, std::string pos) {
    return 1;
}

void Piece::setIsMoved(bool state) {
}
bool Piece::isChecking(Piece*** myPieces) {
    return 1;
}

bool Piece::getJustTwiceStep() {
    return 0;
}
void Piece::setJustTwiceStep(bool state) {}

char Piece::whatPiece() {
    return 'a';
}

Piece::~Piece() {}
