#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"


class Knight : public Piece
{
    public:
        Knight(std::string coord, char colour);
        bool isLegal(Piece*** myPieces, std::string pos);
        char whatPiece();
        ~Knight();

};

#endif // KNIGHT_H
