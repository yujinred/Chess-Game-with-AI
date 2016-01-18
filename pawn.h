#ifndef PAWN_H
#define PAWN_H

#include "piece.h"


class Pawn : public Piece
{
        bool justTwiceStep;
    public:
        Pawn(std::string coord, char colour);
        bool isLegal(Piece*** myPieces, std::string pos);
        char whatPiece();
        ~Pawn();

        bool isChecking(Piece*** myPieces);
        bool getJustTwiceStep();
        void setJustTwiceStep(bool state);

};

#endif // PAWN_H
