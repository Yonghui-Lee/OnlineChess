#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QGraphicsPixmapItem>
#include "grid.h"
#include <QGraphicsSceneMouseEvent>

class Grid;
class Piece:public QGraphicsPixmapItem
{
public:
    Piece(QString team = "",QGraphicsItem *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void setCurrentGrid(Grid *grid);

    Grid *getCurrentGrid() ;

    QString getSide() ;
    QString name;
    virtual void loadImage() = 0;

    bool getIsPlaced() ;
    void setIsPlaced(bool value);

    QList <Grid *> moveLocation();
    virtual void findways() = 0;
    void decolor();

    bool firstMove;

    bool GridSetting(Grid *grid);
protected:
    Grid *nowGrid;
    QString side;
    bool isPlaced;
    QList <Grid *> location;

};

class Bishop:public Piece
{
public:
    Bishop(QString team,QGraphicsItem *parent = 0);
    void loadImage();
    void findways();

};

class King:public Piece
{
public:
    King(QString team,QGraphicsItem *parent = 0);
    void loadImage();
    void findways();
    bool issafe();

};

class Knight:public Piece
{
public:
    Knight(QString team,QGraphicsItem *parent = 0);
    void loadImage();
    void findways();
    bool issafe();

};

class Pawn:public Piece
{
public:
    Pawn(QString team,QGraphicsItem *parent = 0);
    void loadImage();
    void findways();

};

class Queen:public Piece
{
public:
    Queen(QString team,QGraphicsItem *parent = 0);
    void loadImage();
    void findways();

};

class Rook:public Piece
{
public:
    Rook(QString team, QGraphicsItem *parent = 0);

    void loadImage();
    void findways();

};


#endif // CHESSPIECE_H
