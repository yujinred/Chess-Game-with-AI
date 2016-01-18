#ifndef KING_H
#define KING_H

#include "piece.h"


class King : public Piece
{
        bool isMoved;
    public:
        King(std::string coord, char colour);
        bool isLegal(Piece*** myPieces, std::string pos);
        char whatPiece();
        ~King();

        void setIsMoved(bool state);
        bool canCastling(Piece*** myPieces);

};

#endif // KING_H
