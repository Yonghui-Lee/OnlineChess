#include "piece.h"
#include "info.h"
#include <QDebug>

extern Info *info;
Piece::Piece(QString team, QGraphicsItem *parent):QGraphicsPixmapItem(parent)
{
    side = team;
    isPlaced = true;
    firstMove = true;
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if((info->isserver&&info->getTurn()=="BLACK")||(!info->isserver&&info->getTurn()=="WHITE"))
        return;

    if(this == info->pieceToMove){
        info->pieceToMove->getCurrentGrid()->resetOriginalColor();
        info->pieceToMove->decolor();
        info->pieceToMove = NULL;
       return;
    }

    if((!getIsPlaced() )|| ( (info->getTurn() != this->getSide())&& (!info->pieceToMove)) )
        return;

    if(!info->pieceToMove){

        info->pieceToMove = this;
        info->pieceToMove->getCurrentGrid()->settheColor(Qt::red);
        info->pieceToMove->findways();
    }

    else if(this->getSide() != info->pieceToMove->getSide()){
        this->getCurrentGrid()->mousePressEvent(event);
    }


}

void Piece::setCurrentGrid(Grid *grid)
{

    nowGrid = grid;
}

Grid *Piece::getCurrentGrid()
{
    return nowGrid;
}

QString Piece::getSide()
{
    return side;
}



bool Piece::getIsPlaced()
{
    return isPlaced;
}

void Piece::setIsPlaced(bool value)
{
    isPlaced = value;

}
QList<Grid *> Piece::moveLocation()
{
    return location;
}
void Piece::decolor()
{
    for(size_t i = 0, n = location.size(); i < n;i++) {
         location[i]->resetOriginalColor();
    }
}

bool Piece::GridSetting(Grid *grid)
{
    if(grid->ifhasPiece()) {
            grid->settheColor(Qt::red);
        return true;
    }
    else
        location.last()->settheColor(Qt::darkRed);
    return false;
}

Bishop::Bishop(QString team,QGraphicsItem *parent):Piece(team,parent)
{
    loadImage();
    name = "Bishop";
}

void Bishop::loadImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/images/bishop1.png").scaled(80,80));
    else
        setPixmap(QPixmap(":/images/bishop.png").scaled(80,80));
}


void Bishop::findways()
{
    location.clear();
    int row = this->getCurrentGrid()->rowLoc;
    int col = this->getCurrentGrid()->colLoc;
    QString team = this->getSide();


     for(int i = row-1,j = col-1; i >= 0 && j >=0; i--,j--) {
       if(info->grids[i][j]->getColor() == team ) {
           break;

       }
       else
       {
           location.append(info->grids[i][j]);
           if(GridSetting(location.last()) ){
               break;
           }
       }
    }


      for(int i = row-1,j = col+1; i >= 0 && j <= 7; i--,j++) {
        if(info->grids[i][j]->getColor() == team ) {
            break;

        }
        else
        {
            location.append(info->grids[i][j]);
            if(GridSetting(location.last())){
                break;
            }
        }
     }



       for(int i = row+1,j = col+1; i <= 7 && j <= 7; i++,j++) {
         if(info->grids[i][j]->getColor() == team ) {
             break;

         }
         else
         {
             location.append(info->grids[i][j]);
             if(GridSetting(location.last())){
                 break;
             }
         }
      }



        for(int i = row+1,j = col-1; i <= 7 && j >= 0; i++,j--) {
          if(info->grids[i][j]->getColor() == team ) {
              break;

          }
          else
          {
              location.append(info->grids[i][j]);
              if(GridSetting(location.last())){
                  break;
              }

          }
       }

}


King::King(QString team,QGraphicsItem *parent):Piece(team,parent)
{
    loadImage();
    name="King";
}

void King::loadImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/images/king1.png").scaled(80,80));
    else
        setPixmap(QPixmap(":/images/king.png").scaled(80,80));
}


void King::findways()
{

    location.clear();
    int row = this->getCurrentGrid()->rowLoc;
    int col = this->getCurrentGrid()->colLoc;
    QString team = this->getSide();

        if(col > 0 && row > 0 && !(info->grids[row-1][col-1]->getColor() == team)) {
            location.append(info->grids[row-1][col-1]);
            info->grids[row-1][col-1]->settheColor(Qt::darkRed);
            if(location.last()->ifhasPiece()){
                location.last()->settheColor(Qt::red);
            }
        }
        if(col < 7 && row > 0 && !(info->grids[row-1][col+1]->getColor() == team)) {
            location.append(info->grids[row-1][col+1]);
            info->grids[row-1][col+1]->settheColor(Qt::darkRed);
            if(location.last()->ifhasPiece()){
                location.last()->settheColor(Qt::red);
            }
        }
        if(row>0 && !(info->grids[row-1][col]->getColor() == team)) {
            location.append(info->grids[row-1][col]);
            info->grids[row-1][col]->settheColor(Qt::darkRed);
            if(location.last()->ifhasPiece()){
                location.last()->settheColor(Qt::red);
            }
        }
        if(row<7 && !(info->grids[row+1][col]->getColor() == team)) {
            location.append(info->grids[row+1][col]);
            info->grids[row+1][col]->settheColor(Qt::darkRed);
            if(location.last()->ifhasPiece()){
                location.last()->settheColor(Qt::red);
            }
        }
        if(col>0 && !(info->grids[row][col-1]->getColor() == team)) {
            location.append(info->grids[row][col-1]);
            info->grids[row][col-1]->settheColor(Qt::darkRed);
            if(location.last()->ifhasPiece()){
                location.last()->settheColor(Qt::red);
            }
        }
        if(col<7 && !(info->grids[row][col+1]->getColor() == team)) {
            location.append(info->grids[row][col+1]);
            info->grids[row][col+1]->settheColor(Qt::darkRed);
            if(location.last()->ifhasPiece()){
                location.last()->settheColor(Qt::red);
            }
        }
        if(col > 0 && row < 7  && !(info->grids[row+1][col-1]->getColor() == team)) {
            location.append(info->grids[row+1][col-1]);
            info->grids[row+1][col-1]->settheColor(Qt::darkRed);
            if(location.last()->ifhasPiece()){
                location.last()->settheColor(Qt::red);
            }
        }
        if(col < 7 && row < 7 && !(info->grids[row+1][col+1]->getColor() == team)) {
            location.append(info->grids[row+1][col+1]);
            info->grids[row+1][col+1]->settheColor(Qt::darkRed);
            if(location.last()->ifhasPiece()){
                location.last()->settheColor(Qt::red);
            }
        }





}

bool King::issafe()
{   bool safeornot[12];
    for(int i=0;i<=11;i++)
        safeornot[i]=true;
    QList <Piece *> pList = info->alivePiece;
    for(size_t i = 0,n = pList.size(); i < n; i++) {

        if(pList[i]->getSide() != this->getSide())
        {
            QList <Grid *> bList = pList[i]->moveLocation();

            for(size_t j = 0, n = bList.size(); j < n; j++) {

                for(size_t k = 0, n = location.size(); k < n; k++) {
                   if(bList[j] == location [k]) {
                       safeornot[k]=false;
                   }
                }
            }
        }
    }

    for(size_t k = 0, n = location.size(); k < n; k++)
    {
        if(safeornot[k])
            return true;
    }

    return false;
}


Knight::Knight(QString team,QGraphicsItem *parent):Piece(team,parent)
{
    loadImage();
    name="Knight";
}

void Knight::loadImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/images/knight1.png").scaled(80,80));
    else
        setPixmap(QPixmap(":/images/knight.png").scaled(80,80));
}



void Knight::findways()
{
    location.clear();
    int row = this->getCurrentGrid()->rowLoc;
    int col = this->getCurrentGrid()->colLoc;
    int i ,j;
    QString team  = this->getSide();


    i = row - 2;
    j = col - 1;
    if(i >=0 && j>=0 && (info->grids[i][j]->getColor() != team) ) {
        location.append(info->grids[i][j]);
        if(location.last()->ifhasPiece())
            location.last()->settheColor(Qt::red);
        else
            location.last()->settheColor(Qt::darkRed);
    }


    i = row - 2;
    j = col + 1;
    if(i >=0 && j<=7 && (info->grids[i][j]->getColor() != team) ) {
        location.append(info->grids[i][j]);
        if(location.last()->ifhasPiece())
            location.last()->settheColor(Qt::red);
        else
            location.last()->settheColor(Qt::darkRed);
    }


    i = row + 2;
    j = col - 1;
    if(i <= 7 && j>=0 && (info->grids[i][j]->getColor() != team) ) {
        location.append(info->grids[i][j]);
        if(location.last()->ifhasPiece())
            location.last()->settheColor(Qt::red);
        else
            location.last()->settheColor(Qt::darkRed);
    }


    i = row + 2;
    j = col + 1;
    if(i <=7 && j<=7 && (info->grids[i][j]->getColor() != team) ) {
        location.append(info->grids[i][j]);
        if(location.last()->ifhasPiece())
            location.last()->settheColor(Qt::red);
        else
            location.last()->settheColor(Qt::darkRed);
    }


    i = row - 1;
    j = col - 2;
    if(i >=0 && j>=0 && (info->grids[i][j]->getColor() != team) ) {
        location.append(info->grids[i][j]);
        if(location.last()->ifhasPiece())
            location.last()->settheColor(Qt::red);
        else
            location.last()->settheColor(Qt::darkRed);
    }


    i = row + 1;
    j = col - 2;
    if(i <=7 && j>=0 && (info->grids[i][j]->getColor() != team) ) {
        location.append(info->grids[i][j]);
        if(location.last()->ifhasPiece())
            location.last()->settheColor(Qt::red);
        else
            location.last()->settheColor(Qt::darkRed);
    }


    i = row - 1;
    j = col + 2;
    if(i >=0 && j<=7 && (info->grids[i][j]->getColor() != team) ) {
        location.append(info->grids[i][j]);
        if(location.last()->ifhasPiece())
            location.last()->settheColor(Qt::red);
        else
            location.last()->settheColor(Qt::darkRed);
    }


    i = row + 1;
    j = col +  2;
    if(i <=7 && j<=7 && (info->grids[i][j]->getColor() != team) ) {
        location.append(info->grids[i][j]);
        if(location.last()->ifhasPiece())
            location.last()->settheColor(Qt::red);
        else
            location.last()->settheColor(Qt::darkRed);
    }



}


Pawn::Pawn(QString team,QGraphicsItem *parent):Piece(team,parent)
{
    loadImage();
    name = "Pawn";
    firstMove = true;
}

void Pawn::loadImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/images/pawn1.png").scaled(80,80));
    else
        setPixmap(QPixmap(":/images/pawn.png").scaled(80,80));
}



void Pawn::findways()
{
    location.clear();

    int row = this->getCurrentGrid()->rowLoc;

    int col = this->getCurrentGrid()->colLoc;
    if(this->getSide() == "WHITE")  {
        if(col > 0 && row > 0 && info->grids[row-1][col-1]->getColor() == "BLACK") {
            location.append(info->grids[row-1][col-1]);
            GridSetting(location.last());
        }
        if(col < 7 && row > 0 && info->grids[row-1][col+1]->getColor() == "BLACK") {
            location.append(info->grids[row-1][col+1]);
            GridSetting(location.last());
        }
        if(row>0 && (!info->grids[row-1][col]->ifhasPiece())) {
            location.append(info->grids[row-1][col]);
            GridSetting(location.last());
            if(firstMove && !info->grids[row-2][col]->ifhasPiece()){
                location.append(info->grids[row-2][col]);
                GridSetting(location.last());
            }
        }

    }
    else{
        if(col > 0 && row < 7 && info->grids[row+1][col-1]->getColor() == "WHITE") {
            location.append(info->grids[row+1][col-1]);
            GridSetting(location.last());
        }
        if(col < 7 && row <  7 && info->grids[row+1][col+1]->getColor() == "WHITE") {
            location.append(info->grids[row+1][col+1]);
            GridSetting(location.last());
        }
        if(row<7 && (!info->grids[row+1][col]->ifhasPiece())) {
            location.append(info->grids[row+1][col]);
            GridSetting(location.last());
            if(firstMove && !info->grids[row+2][col]->ifhasPiece()){
                location.append(info->grids[row+2][col]);
                GridSetting(location.last());
            }

        }

    }

}



Queen::Queen(QString team,QGraphicsItem *parent):Piece(team,parent)
{
    loadImage();
    name = "Queen";
}

void Queen::loadImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/images/queen1.png").scaled(80,80));
    else
        setPixmap(QPixmap(":/images/queen.png").scaled(80,80));
}


void Queen::findways()
{
    location.clear();
    int row = this->getCurrentGrid()->rowLoc;
    int col = this->getCurrentGrid()->colLoc;
    QString team = this->getSide();


     for(int i = row-1,j = col; i >= 0 ; i--) {
       if(info->grids[i][j]->getColor() == team ) {
           break;
       }
       else
       {
           location.append(info->grids[i][j]);
           if(GridSetting(location.last()))
               break;
        }
    }



      for(int i = row+1,j = col; i <= 7 ; i++) {
        if(info->grids[i][j]->getColor() == team ) {
            break;
        }
        else
        {
            location.append(info->grids[i][j]);
            if(GridSetting(location.last())){
                break;
            }
        }
     }



       for(int i = row,j = col-1; j >= 0 ; j--) {
         if(info->grids[i][j]->getColor() == team ) {
             break;
         }
         else
         {
             location.append(info->grids[i][j]);
             if(GridSetting(location.last()))
                break;
         }
        }


        for(int i = row,j = col+1; j <= 7 ; j++)
        {
              if(info->grids[i][j]->getColor() == team ) {
                  break;
              }
              else
              {
                  location.append(info->grids[i][j]);
                  if(GridSetting(location.last()))
                      break;
               }

       }



         for(int i = row-1,j = col-1; i >= 0 && j >=0; i--,j--) {
           if(info->grids[i][j]->getColor() == team ) {
               break;

           }
           else
           {
               location.append(info->grids[i][j]);
               if(GridSetting(location.last()) ){
                   break;
               }
           }
        }



          for(int i = row-1,j = col+1; i >= 0 && j <= 7; i--,j++) {
            if(info->grids[i][j]->getColor() == team ) {
                break;

            }
            else
            {
                location.append(info->grids[i][j]);
                if(GridSetting(location.last())){
                    break;
                }
            }
         }


           for(int i = row+1,j = col+1; i <= 7 && j <= 7; i++,j++) {
             if(info->grids[i][j]->getColor() == team ) {
                 break;

             }
             else
             {
                 location.append(info->grids[i][j]);
                 if(GridSetting(location.last())){
                     break;
                 }
             }
          }


            for(int i = row+1,j = col-1; i <= 7 && j >= 0; i++,j--) {
              if(info->grids[i][j]->getColor() == team ) {
                  break;

              }
              else
              {
                  location.append(info->grids[i][j]);
                  if(GridSetting(location.last())){
                      break;
                  }

              }
           }


}


Rook::Rook(QString team,QGraphicsItem *parent):Piece(team,parent)
{
    loadImage();
    name = "Rook";
}


void Rook::loadImage()
{
    if(side == "WHITE")
        setPixmap(QPixmap(":/images/rook1.png").scaled(80,80));
    else
        setPixmap(QPixmap(":/images/rook.png").scaled(80,80));
}

void Rook::findways()
{
    location.clear();
    int row = this->getCurrentGrid()->rowLoc;
    int col = this->getCurrentGrid()->colLoc;
    QString team = this->getSide();


     for(int i = row-1,j = col; i >= 0 ; i--) {
       if(info->grids[i][j]->getColor() == team ) {
           break;
       }
       else
       {
           location.append(info->grids[i][j]);
           if(GridSetting(location.last()))
               break;
        }
    }



      for(int i = row+1,j = col; i <= 7 ; i++) {
        if(info->grids[i][j]->getColor() == team ) {
            break;
        }
        else
        {
            location.append(info->grids[i][j]);
            if(GridSetting(location.last())){
                break;
            }
        }
     }



       for(int i = row,j = col-1; j >= 0 ; j--) {
         if(info->grids[i][j]->getColor() == team ) {
             break;
         }
         else
         {
             location.append(info->grids[i][j]);
             if(GridSetting(location.last()))
                break;
         }
        }

        for(int i = row,j = col+1; j <= 7 ; j++)
        {
              if(info->grids[i][j]->getColor() == team ) {
                  break;
              }
              else
              {
                  location.append(info->grids[i][j]);
                  if(GridSetting(location.last()))
                      break;
               }

       }


}












