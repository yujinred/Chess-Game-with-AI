#ifndef PIECE_H
#define PIECE_H

#include <string>

class Grid;

class Piece
{
    public:
        Piece(std::string coord, char colour);
        virtual bool isLegal(Piece*** myPieces, std::string pos);
        void setCoord(std::string pos);
        char getColour();
        bool hasLegal(Piece*** myPieces);
        virtual ~Piece();

        std::string* possibleMoves(Piece*** myPieces);
        int numberPossibleMoves(Piece*** myPieces);
        virtual bool isChecking(Piece*** myPieces);
        virtual void setIsMoved(bool state);
        virtual bool getJustTwiceStep();
        virtual void setJustTwiceStep(bool state);
        virtual char whatPiece();

        friend class Computer;
    protected:
        std::string coord;
        char colour;

};

#endif // PIECE_H
