#ifndef BOARD_H
#define BOARD_H
#include <QGraphicsRectItem>
#include "piece.h"
class Board
{
public:
    Board();


    void drawGrids(int x, int y);
    void initArray();



    void addPiece();
    void reset();
private:
    QList <Piece *> white;
    QList <Piece *> black;



};

#endif // BOARD_H
