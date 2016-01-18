#ifndef ROOK_H
#define ROOK_H

#include "piece.h"


class Rook : public Piece
{
        bool isMoved;
    public:
        Rook(std::string coord, char colour);
        bool isLegal(Piece*** myPieces, std::string pos);
        char whatPiece();
        ~Rook();

        void setIsMoved(bool state);
        bool canCastling(Piece*** myPieces);
};

#endif // ROOK_H
