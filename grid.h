#ifndef GRID_H
#define GRID_H

#include <QGraphicsRectItem>
#include <QBrush>
#include "piece.h"
#include <QGraphicsSceneMouseEvent>

class Piece;
class Grid:public QGraphicsRectItem
{
public:


    Grid(QGraphicsItem *parent=0);
    ~Grid();



    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void settheColor(QColor color);
    void placePiece(Piece *piece);


    void resetOriginalColor();
    void setOriginalColor(QColor value);

    bool ifhasPiece();
    void setPiece(bool value,Piece *piece = 0);

    int checkPawn();

    QString getColor() ;
    void setColor(QString value);

    int rowLoc;
    int colLoc;

    Piece * nowPiece;

private:
    QBrush brush;
    QColor originalColor;
    bool hasChessPiece;
    QString chessPieceColor;


};

#endif // GRID_H
