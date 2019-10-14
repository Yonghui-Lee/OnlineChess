#ifndef INFO_H
#define INFO_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <board.h>
#include "piece.h"
class Info:public QGraphicsView
{
    Q_OBJECT
public:
    bool whiteshortcastling;
    bool whitelongcastling;
    bool blackshortcastling;
    bool blacklongcastling;
    bool isserver;
    void checkCastling();
    bool checkCheck();
    QGraphicsTextItem * Display;


    Info(QWidget *parent = 0);

    void drawBoard();

    void readfile();
    void savefile();



    void addToScene(QGraphicsItem *item);
    void removeFromScene(QGraphicsItem *item);


    Piece *pieceToMove;
    QString getTurn() ;
    void setTurn( QString value);
    void changeTurn();
    void sentstring(QString m);



    Grid *grids[8][8];
    QList <Piece *> alivePiece;

    void gameOver();
    void removeAll();
    void start();
protected slots:
    void sent();
signals:
    void needtowrite(QString m);
private:
    QGraphicsScene *gameScene;
    QString turn;
    Board *chess;


};

#endif // INFO_H
