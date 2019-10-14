#include "grid.h"
#include "info.h"
#include "piece.h"
#include <QDebug>
#include <QMessageBox>
#include <Qtimer>
#include <QInputDialog>
extern Info *info;
Grid::Grid(QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    //making the Square CHess Box
    setRect(0,0,100,100);
    brush.setStyle(Qt::SolidPattern);
    setZValue(-1);
    setPiece(false);
    setColor("NONE");
    nowPiece = NULL;
}

Grid::~Grid()
{
    delete this;
}

void Grid::mousePressEvent(QGraphicsSceneMouseEvent *event)
{   if((info->isserver&&info->getTurn()=="BLACK")||(!info->isserver&&info->getTurn()=="WHITE"))
        return;



        if(nowPiece == info->pieceToMove && nowPiece){

            nowPiece->mousePressEvent(event);
            return;
        }

        if(info->pieceToMove){

            if(this->getColor() == info->pieceToMove->getSide())
                return;

            QList <Grid *> movLoc = info->pieceToMove->moveLocation();

            int check = 0;
            for(size_t i = 0, n = movLoc.size(); i < n;i++) {
                if(movLoc[i] == this) {
                    check++;

                }
            }

            if(check == 0)
                return;

             info->pieceToMove->decolor();

             info->pieceToMove->firstMove = false;

           if(this->ifhasPiece()){


  //移走被吃棋子
                if(nowPiece->getSide() == "WHITE") {

                    King *g = dynamic_cast <King *>(this->nowPiece);
                    if(g){

                        if(!info->isserver)
                        {
                            QMessageBox message(QMessageBox::Information,"Info","YOU Win",QMessageBox::Ok,NULL);

                                                if (message.exec()==QMessageBox::Ok)
                                                    {
                                                      qDebug()<<"clicked Ok\n";
                                                    }
                                                else
                                                    {
                                                      qDebug()<<"clicked no\n";
                                                    }

                                                QString ss;
                                                ss.clear();
                                                ss.append('w');
                                                info->sentstring(ss);
                                                info->gameOver();
                            return;
                        }
                        else
                        {
                            QMessageBox message(QMessageBox::Information,"Info","YOU LOSE",QMessageBox::Ok,NULL);

                                                if (message.exec()==QMessageBox::Ok)
                                                    {
                                                      qDebug()<<"clicked Ok\n";
                                                    }
                                                else
                                                    {
                                                      qDebug()<<"clicked no\n";
                                                    }

                                                QString ss;
                                                ss.clear();
                                                ss.append('f');
                                                info->sentstring(ss);
                                                info->gameOver();
                            return;
                        }
                    }

                }
                else{

                    King *g = dynamic_cast <King *>(this->nowPiece);
                    if(g){
                    if(info->isserver)
                    {
                        QMessageBox message(QMessageBox::Information,"Info","YOU Win",QMessageBox::Ok,NULL);

                                            if (message.exec()==QMessageBox::Ok)
                                                {
                                                  qDebug()<<"clicked Ok\n";
                                                }
                                            else
                                                {
                                                  qDebug()<<"clicked no\n";
                                                }

                                            QString ss;
                                            ss.clear();
                                            ss.append('w');
                                            info->sentstring(ss);
                                            info->gameOver();
                        return;
                    }
                    else
                    {
                        QMessageBox message(QMessageBox::Information,"Info","YOU LOSE",QMessageBox::Ok,NULL);

                                            if (message.exec()==QMessageBox::Ok)
                                                {
                                                  qDebug()<<"clicked Ok\n";
                                                }
                                            else
                                                {
                                                  qDebug()<<"clicked no\n";
                                                }
                                            QString ss;
                                            ss.clear();
                                            ss.append('f');
                                            info->sentstring(ss);
                                            info->gameOver();
                        return;
                    }


                    }

                }


                this->nowPiece->setIsPlaced(false);
                this->nowPiece->setCurrentGrid(NULL);
                this->nowPiece->hide();
                info->alivePiece.removeAll(this->nowPiece);

            }




           QString writeQString;
           writeQString.clear();
           if(info->pieceToMove->getSide()=="WHITE")
               writeQString.append('0');
           else writeQString.append('1');
           writeQString.append('m');
           writeQString.append(QString::number(info->pieceToMove->getCurrentGrid()->rowLoc));
           writeQString.append(QString::number(info->pieceToMove->getCurrentGrid()->colLoc));
           writeQString.append(QString::number(this->rowLoc));
           writeQString.append(QString::number(this->colLoc));
           if(info->pieceToMove->name=="Bishop")
               writeQString.append(QString::number(1));
           if(info->pieceToMove->name=="King")
               writeQString.append(QString::number(2));
           if(info->pieceToMove->name=="Knight")
               writeQString.append(QString::number(3));
           if(info->pieceToMove->name=="Pawn")
               writeQString.append(QString::number(4));
           if(info->pieceToMove->name=="Queen")
               writeQString.append(QString::number(5));
           if(info->pieceToMove->name=="Rook")
               writeQString.append(QString::number(6));



           qDebug()<<writeQString;
           info->sentstring(writeQString);




            info->pieceToMove->getCurrentGrid()->setPiece(false);
            info->pieceToMove->getCurrentGrid()->nowPiece = NULL;
            info->pieceToMove->getCurrentGrid()->resetOriginalColor();
            placePiece(info->pieceToMove);
            checkPawn();
            info->pieceToMove = NULL;

            info->changeTurn();


            if(!info->checkCheck())
            {   qDebug()<<info->getTurn();
                QList <Piece *> pList = info->alivePiece;
                for(size_t i = 0,n=pList.size(); i < n; i++ )
                {   if(pList[i]->getSide()!=info->getTurn())
                        continue;
                    pList[i]->findways();
                    pList[i]->decolor();
                    QList <Grid *> bList = pList[i]->moveLocation();

                    for(size_t j = 0,n = bList.size(); j < n; j ++)
                    {
                            bool flag=false;
                            if(bList[j]->ifhasPiece()){flag=true;}

                            int row1=pList[i]->getCurrentGrid()->rowLoc;
                            int col1=pList[i]->getCurrentGrid()->colLoc;



                            Piece *piece1=pList[i];
                            piece1->getCurrentGrid()->setPiece(false);
                            piece1->getCurrentGrid()->nowPiece = NULL;
                            piece1->setIsPlaced(false);
                            piece1->setCurrentGrid(NULL);


                            Piece *piece2;
                            if(flag)
                            {
                                piece2=bList[j]->nowPiece;
                                piece2->setIsPlaced(false);
                                piece2->setCurrentGrid(NULL);
                                info->alivePiece.removeAll(piece2);
                            }

                            bList[j]->placePiece(piece1);
                            piece1->setIsPlaced(true);


                            if(!info->checkCheck())
                            {
                                piece1->getCurrentGrid()->setPiece(false);
                                piece1->getCurrentGrid()->nowPiece = NULL;
                                piece1->setIsPlaced(false);
                                piece1->setCurrentGrid(NULL);




                                if(flag)
                                {
                                    bList[j]->placePiece(piece2);
                                    piece2->setIsPlaced(true);
                                    info->alivePiece.append(piece2);
                                }

                                info->grids[row1][col1]->placePiece(piece1);
                                piece1->setIsPlaced(true);
                              return;


                            }
                            piece1->getCurrentGrid()->setPiece(false);
                            piece1->getCurrentGrid()->nowPiece = NULL;
                            piece1->setIsPlaced(false);
                            piece1->setCurrentGrid(NULL);




                            if(flag)
                            {
                                bList[j]->placePiece(piece2);
                                piece2->setIsPlaced(true);
                                info->alivePiece.append(piece2);
                            }

                            info->grids[row1][col1]->placePiece(piece1);
                            piece1->setIsPlaced(true);



                    }


                }
                QTimer::singleShot(200, info, SLOT(sent()));
                info->gameOver();
                QMessageBox::about(NULL,"Info","DRAW");
            }
            else qDebug()<<"Check";



        }

        else if(this->ifhasPiece())
        {
            this->nowPiece->mousePressEvent(event);
        }
}

void Grid::settheColor(QColor color)
{
    brush.setColor(color);
    setBrush(color);
}

void Grid::placePiece(Piece *piece)
{

    piece->setPos(x()+50- piece->pixmap().width()/2  ,y()+50-piece->pixmap().width()/2);
    piece->setCurrentGrid(this);
    setPiece(true,piece);
    nowPiece = piece;


}

void Grid::resetOriginalColor()
{
    settheColor(originalColor);
}



void Grid::setOriginalColor(QColor value)
{
    originalColor = value;
    settheColor(originalColor);
}

bool Grid::ifhasPiece()
{
    return hasChessPiece;
}

void Grid::setPiece(bool value, Piece *piece)
{
    hasChessPiece = value;
    if(value)
        setColor(piece->getSide());
    else
        setColor("NONE");
}



int Grid::checkPawn()
{
    QList <Piece *> pList = info->alivePiece;
    for(size_t i = 0,n=pList.size(); i < n; i++ ) {
        bool isOK;
        Pawn * p = dynamic_cast<Pawn *> (pList[i]);
        if(!p){
            continue;
        }

        if(pList[i]->getCurrentGrid()->rowLoc==0&&pList[i]->getSide()=="WHITE")
        {

            QString text = QInputDialog::getText(NULL, "Input Dialog", "What do you want to turn your Pawn into?\n(Bishop,Knight,Queen,Rook)", QLineEdit::Normal, "", &isOK);
            qDebug()<<text;
            int row = pList[i]->getCurrentGrid()->rowLoc;
            int col = pList[i]->getCurrentGrid()->colLoc;


           // qDebug()<<game->alivePiece.size();
            info->alivePiece.removeAt(i);
            info->alivePiece.removeAll(pList[i]);
            info->removeFromScene(pList[i]);
           // qDebug()<<game->alivePiece.size();
            Grid *grid =info->grids[row][col];
            grid->setPiece(false);
            grid->setColor("NONE");
            grid->nowPiece = NULL;

            if(text=="Bishop")
            {
                Bishop *piece=new Bishop("WHITE");
                grid->placePiece(piece);
                piece->setIsPlaced(true);
                piece->firstMove = true;
                 info->addToScene(piece);
                info->alivePiece.append(piece);

                QString writeQString;
                writeQString.clear();
                writeQString.append('0');
                writeQString.append('t');
                writeQString.append(QString::number(row));
                writeQString.append(QString::number(col));
                writeQString.append(QString::number(1));
                info->sentstring(writeQString);
                return 1;
            }
            if(text=="Knight")
            {
                Knight *piece = new Knight("WHITE");
                grid->placePiece(piece);
                piece->setIsPlaced(true);
                piece->firstMove = true;
                 info->addToScene(piece);
                info->alivePiece.append(piece);

                QString writeQString;
                writeQString.clear();
                writeQString.append('0');
                writeQString.append('t');
                writeQString.append(QString::number(row));
                writeQString.append(QString::number(col));
                writeQString.append(QString::number(3));
                info->sentstring(writeQString);
                 return 3;
            }
            if(text=="Queen")
            {
                Queen *piece = new Queen("WHITE");
                grid->placePiece(piece);
                piece->setIsPlaced(true);
                piece->firstMove = true;
                 info->addToScene(piece);
                info->alivePiece.append(piece);

                QString writeQString;
                writeQString.clear();
                writeQString.append('0');
                writeQString.append('t');
                writeQString.append(QString::number(row));
                writeQString.append(QString::number(col));
                writeQString.append(QString::number(5));
                info->sentstring(writeQString);
                 return 5;
            }
            if(text=="Rook")
            {
                Rook *piece = new Rook("WHITE");
                grid->placePiece(piece);
                piece->setIsPlaced(true);
                piece->firstMove = true;
                 info->addToScene(piece);
                info->alivePiece.append(piece);

                QString writeQString;
                writeQString.clear();
                writeQString.append('0');
                writeQString.append('t');
                writeQString.append(QString::number(row));
                writeQString.append(QString::number(col));
                writeQString.append(QString::number(6));
                info->sentstring(writeQString);
                 return 6;
            }
        }

        if(pList[i]->getCurrentGrid()->rowLoc==7&&pList[i]->getSide()=="BLACK")
        {

            QString text = QInputDialog::getText(NULL, "Input Dialog", "What do you want to turn your Pawn into?\n(Bishop,Knight,Queen,Rook)", QLineEdit::Normal, "", &isOK);
            qDebug()<<text;
            int row = pList[i]->getCurrentGrid()->rowLoc;
            int col = pList[i]->getCurrentGrid()->colLoc;

            qDebug()<<info->alivePiece.size();
            info->alivePiece.removeAt(i);
             info->alivePiece.removeAll(pList[i]);
             info->removeFromScene(pList[i]);
            qDebug()<<info->alivePiece.size();
            Grid *grid =info->grids[row][col];
            grid->setPiece(false);
            grid->setColor("NONE");
            grid->nowPiece = NULL;
            if(text=="Bishop")
            {
                Bishop *piece=new Bishop("BLACK");
                grid->placePiece(piece);
                piece->setIsPlaced(true);
                piece->firstMove = true;
                 info->addToScene(piece);
                info->alivePiece.append(piece);


                QString writeQString;
                writeQString.clear();
                writeQString.append('1');
                writeQString.append('t');
                writeQString.append(QString::number(row));
                writeQString.append(QString::number(col));
                writeQString.append(QString::number(1));
                info->sentstring(writeQString);
                 return 1;
            }
            if(text=="Knight")
            {
                Knight *piece = new Knight("BLACK");
                grid->placePiece(piece);
                piece->setIsPlaced(true);
                piece->firstMove = true;
                 info->addToScene(piece);
                info->alivePiece.append(piece);

                QString writeQString;
                writeQString.clear();
                writeQString.append('1');
                writeQString.append('t');
                writeQString.append(QString::number(row));
                writeQString.append(QString::number(col));
                writeQString.append(QString::number(3));
                info->sentstring(writeQString);

                 return 3;
            }
            if(text=="Queen")
            {
                Queen *piece = new Queen("BLACK");
                grid->placePiece(piece);
                piece->setIsPlaced(true);
                piece->firstMove = true;
                 info->addToScene(piece);
                info->alivePiece.append(piece);

                QString writeQString;
                writeQString.clear();
                writeQString.append('1');
                writeQString.append('t');
                writeQString.append(QString::number(row));
                writeQString.append(QString::number(col));
                writeQString.append(QString::number(5));
                info->sentstring(writeQString);

                 return 5;
            }
            if(text=="Rook")
            {
                Rook *piece = new Rook("BLACK");
                grid->placePiece(piece);
                piece->setIsPlaced(true);
                piece->firstMove = true;
                 info->addToScene(piece);
                info->alivePiece.append(piece);

                QString writeQString;
                writeQString.clear();
                writeQString.append('1');
                writeQString.append('t');
                writeQString.append(QString::number(row));
                writeQString.append(QString::number(col));
                writeQString.append(QString::number(6));
                info->sentstring(writeQString);
                 return 6;
            }
        }

    }

return 0;
}



QString Grid::getColor()
{
    return chessPieceColor;
}

void Grid::setColor(QString value)
{
    chessPieceColor = value;
}
