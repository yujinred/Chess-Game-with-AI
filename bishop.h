#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece {
    public:
        Bishop(std::string coord, char colour);
        bool isLegal(Piece*** myPieces, std::string pos);
        char whatPiece();
        ~Bishop();
};

#endif // BISHOP_H
