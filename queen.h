#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"


class Queen : public Piece
{
    public:
        Queen(std::string coord, char colour);
        bool isLegal(Piece*** myPieces, std::string pos);
        char whatPiece();
        ~Queen();
};

#endif // QUEEN_H
