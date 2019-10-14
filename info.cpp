#include "info.h"
#include <QPixmap>
#include "piece.h"
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QFileDialog>
Info::Info(QWidget *parent ):QGraphicsView(parent)
{

    whiteshortcastling=false;
    whitelongcastling=false;
    blackshortcastling=false;
    blacklongcastling=false;
    gameScene = new QGraphicsScene();
    gameScene->setSceneRect(0,0,800,850);

    setFixedSize(800,850);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(gameScene);
    pieceToMove = NULL;

    Display = new QGraphicsTextItem();
    Display->setPos(width()/2-100,0);
    Display->setZValue(1);
    Display->setDefaultTextColor(Qt::black);
    Display->setFont(QFont("",18));
    Display->setPlainText("Turn : WHITE");

    setTurn("WHITE");



}

void Info::sentstring(QString m)
{
    emit needtowrite(m);

}

void Info::drawBoard()
{

    chess = new Board();

     chess->drawGrids(width()/2-400,50);

}





void Info::addToScene(QGraphicsItem *item)
{
    gameScene->addItem(item);
}

void Info::removeFromScene(QGraphicsItem *item)
{
    gameScene->removeItem(item);

}

QString Info::getTurn()
{
    return turn;
}

void Info::setTurn(QString value)
{
    turn = value;
}

void Info::changeTurn()
{
    if(getTurn() == "WHITE")
        setTurn("BLACK");
    else
        setTurn("WHITE");
    Display->setPlainText("Turn : " + getTurn());
}

void Info::start()
{
    drawBoard();
    addToScene(Display);
    chess->addPiece();
}

void Info::sent()
{
    emit needtowrite("y");
}







void Info::gameOver()
{
    removeAll();
    Display->setPlainText("Turn : WHITE");
    setTurn("WHITE");
     drawBoard();
     pieceToMove = NULL;
    alivePiece.clear();
    chess->reset();



}

void Info::removeAll(){

    QList<QGraphicsItem*> itemsList = gameScene->items();
    for(size_t i = 0, n = itemsList.size();i<n;i++) {
        if(itemsList[i]!=Display)
            removeFromScene(itemsList[i]);


    }
}


void Info::checkCastling()
{   whitelongcastling=false;
    whiteshortcastling=false;
    blacklongcastling=false;
    blackshortcastling=false;

    bool whitelongflag = true;
    bool whiteshortflag=true;
    bool blacklongflag=true;
    bool blackshortflag=true;

    QList <Piece *> pList = alivePiece;
     for(size_t i = 0,n=pList.size(); i < n; i++ )
    {
          pList[i]->findways();
          pList[i]->decolor();
         QList <Grid *> bList = pList[i]->moveLocation();
       for(size_t j = 0,n = bList.size(); j < n; j ++)
      {
          if(pList[i]->getSide()=="BLACK"
                  &&((bList[j]->rowLoc==7&&bList[j]->colLoc==4)
                  ||(bList[j]->rowLoc==7&&bList[j]->colLoc==3)
                  ||(bList[j]->rowLoc==7&&bList[j]->colLoc==2)
                  ))
          {whitelongflag=false;}

          if(pList[i]->getSide()=="BLACK"
                  &&((bList[j]->rowLoc==7&&bList[j]->colLoc==4)
                  ||(bList[j]->rowLoc==7&&bList[j]->colLoc==5)
                  ||(bList[j]->rowLoc==7&&bList[j]->colLoc==6)
                  ))
          {whiteshortflag=false;}

          if(pList[i]->getSide()=="WHITE"
             &&((bList[j]->rowLoc==0&&bList[j]->colLoc==4)
                  ||(bList[j]->rowLoc==0&&bList[j]->colLoc==3)
                  ||(bList[j]->rowLoc==0&&bList[j]->colLoc==2)
                  ))
          {blacklongflag=false;}


          if(pList[i]->getSide()=="WHITE"
             &&((bList[j]->rowLoc==0&&bList[j]->colLoc==4)
                  ||(bList[j]->rowLoc==0&&bList[j]->colLoc==5)
                  ||(bList[j]->rowLoc==0&&bList[j]->colLoc==6)
                  ))
          {blackshortflag=false;}


      }


   }



    if(grids[7][0]->ifhasPiece()
            &&grids[7][4]->ifhasPiece()
            &&grids[7][4]->nowPiece->name=="King"
            &&grids[7][4]->nowPiece->getSide()=="WHITE"
            &&!grids[7][3]->ifhasPiece()
            &&!grids[7][2]->ifhasPiece()
            &&!grids[7][1]->ifhasPiece()
            &&grids[7][0]->nowPiece->name=="Rook"
            &&grids[7][0]->nowPiece->getSide()=="WHITE"
            )
    {


        if(whitelongflag)
        {
            whitelongcastling=true;
            qDebug()<<"whitelong";
        }
    }

    if(grids[7][4]->ifhasPiece()
            &&grids[7][7]->ifhasPiece()
            &&grids[7][4]->nowPiece->name=="King"
            &&grids[7][4]->nowPiece->getSide()=="WHITE"
            &&!grids[7][5]->ifhasPiece()
            &&!grids[7][6]->ifhasPiece()
            &&grids[7][7]->nowPiece->name=="Rook"
            &&grids[7][7]->nowPiece->getSide()=="WHITE"
            )
    {

        if(whiteshortflag)
        {
            whiteshortcastling=true;
            qDebug()<<"whiteshort";
        }

    }

    if(grids[0][4]->ifhasPiece()
            &&grids[0][0]->ifhasPiece()
            &&grids[0][4]->nowPiece->name=="King"
            &&grids[0][4]->nowPiece->getSide()=="BLACK"
            &&!grids[0][3]->ifhasPiece()
            &&!grids[0][2]->ifhasPiece()
            &&!grids[0][1]->ifhasPiece()
            &&grids[0][0]->nowPiece->name=="Rook"
            &&grids[0][0]->nowPiece->getSide()=="BLACK"
            )
    {

        if(blacklongflag)
        {
            blacklongcastling=true;
            qDebug()<<"blacklong";
        }

    }

    if(grids[0][4]->ifhasPiece()
            &&grids[0][7]->ifhasPiece()
            &&grids[0][4]->nowPiece->name=="King"
            &&grids[0][4]->nowPiece->getSide()=="BLACK"
            &&!grids[0][5]->ifhasPiece()
            &&!grids[0][6]->ifhasPiece()
            &&grids[0][7]->nowPiece->name=="Rook"
            &&grids[0][7]->nowPiece->getSide()=="BLACK"
            )
    {

        if(blackshortflag)
        {
            blackshortcastling=true;
            qDebug()<<"blackshort";
        }

    }






}

bool Info::checkCheck()
{

    QList <Piece *> pList = alivePiece;
    for(size_t i = 0,n=pList.size(); i < n; i++ ) {

        King * p = dynamic_cast<King *> (pList[i]);
        if(p&&p->getSide()==getTurn()){
            continue;
        }
        pList[i]->findways();
        pList[i]->decolor();
        QList <Grid *> bList = pList[i]->moveLocation();
        for(size_t j = 0,n = bList.size(); j < n; j ++) {
            King * p = dynamic_cast<King *> (bList[j]->nowPiece);
            if(p) {
                if(p->getSide() == pList[i]->getSide())
                    continue;

                return true;

            }
        }


    }

    return false;

}


void Info::readfile()
{
    removeAll();
    Display->setPlainText("Turn : WHITE");

    drawBoard();
    pieceToMove = NULL;
    alivePiece.clear();
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {

            Grid *grid =grids[i][j];
            grid->setPiece(false);
            grid->setColor("NONE");
            grid->nowPiece = NULL;
        }
    }





    QString filename = QFileDialog::getOpenFileName(this,
                                         tr("选择文件"),
                                         "",
                                         tr("Text files (*.txt)"));
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        qFatal("Could not open file");
    QTextStream in(&f);
    QString linestr;
    int linenum=0;
    QString side;
    side.clear();
    while(!in.atEnd())
    {   linenum++;
        linestr=in.readLine();
        if(linenum==1)
        {
            if(linestr=="white")
            {setTurn("WHITE"); Display->setPlainText("Turn : WHITE");}
            if(linestr=="black")
            {setTurn("BLACK"); Display->setPlainText("Turn : BLACK");}

        }

        if(linestr=="white")
        {side="WHITE";}
        else if(linestr=="black")
        {side="BLACK";}
        else
        {
            QStringList list=linestr.split(' ');


            if(list[0]=="bishop")
            {
                for(int i=1;i<=list[1].toInt();i++)
                {
                   int row=8-list[1+i][1].digitValue();
                   int col;
                   if(list[1+i][0]=="a")
                       col=0;
                   if(list[1+i][0]=="b")
                       col=1;
                   if(list[1+i][0]=="c")
                       col=2;
                   if(list[1+i][0]=="d")
                       col=3;
                   if(list[1+i][0]=="e")
                       col=4;
                   if(list[1+i][0]=="f")
                       col=5;
                   if(list[1+i][0]=="g")
                       col=6;
                   if(list[1+i][0]=="h")
                       col=7;

                   Grid *grid =grids[row][col];
                   Bishop *piece=new Bishop(side);



                   grid->placePiece(piece);
                   piece->setIsPlaced(true);
                   piece->firstMove = true;   //Pawn处不行
                   addToScene(piece);
                   alivePiece.append(piece);

                }
            }
            else if(list[0]=="king")
            {
                for(int i=1;i<=list[1].toInt();i++)
                {
                   int row=8-list[1+i][1].digitValue();
                   int col;
                   if(list[1+i][0]=="a")
                       col=0;
                   if(list[1+i][0]=="b")
                       col=1;
                   if(list[1+i][0]=="c")
                       col=2;
                   if(list[1+i][0]=="d")
                       col=3;
                   if(list[1+i][0]=="e")
                       col=4;
                   if(list[1+i][0]=="f")
                       col=5;
                   if(list[1+i][0]=="g")
                       col=6;
                   if(list[1+i][0]=="h")
                       col=7;

                   Grid *grid =grids[row][col];
                   King *piece=new King(side);



                   grid->placePiece(piece);
                   piece->setIsPlaced(true);
                   piece->firstMove = true;   //Pawn处不行
                   addToScene(piece);
                   alivePiece.append(piece);
                  }
            }
            else if(list[0]=="knight")
            {
                    for(int i=1;i<=list[1].toInt();i++)
                    {
                       int row=8-list[1+i][1].digitValue();
                       int col;
                       if(list[1+i][0]=="a")
                           col=0;
                       if(list[1+i][0]=="b")
                           col=1;
                       if(list[1+i][0]=="c")
                           col=2;
                       if(list[1+i][0]=="d")
                           col=3;
                       if(list[1+i][0]=="e")
                           col=4;
                       if(list[1+i][0]=="f")
                           col=5;
                       if(list[1+i][0]=="g")
                           col=6;
                       if(list[1+i][0]=="h")
                           col=7;

                       Grid *grid =grids[row][col];
                       Knight *piece=new Knight(side);



                       grid->placePiece(piece);
                       piece->setIsPlaced(true);
                       piece->firstMove = true;   //Pawn处不行
                       addToScene(piece);
                       alivePiece.append(piece);
                    }

            }
            else if(list[0]=="pawn")
            {
                        for(int i=1;i<=list[1].toInt();i++)
                        {
                           int row=8-list[1+i][1].digitValue();
                           int col;
                           if(list[1+i][0]=="a")
                               col=0;
                           if(list[1+i][0]=="b")
                               col=1;
                           if(list[1+i][0]=="c")
                               col=2;
                           if(list[1+i][0]=="d")
                               col=3;
                           if(list[1+i][0]=="e")
                               col=4;
                           if(list[1+i][0]=="f")
                               col=5;
                           if(list[1+i][0]=="g")
                               col=6;
                           if(list[1+i][0]=="h")
                               col=7;

                           Grid *grid =grids[row][col];
                           Pawn *piece=new Pawn(side);



                           grid->placePiece(piece);
                           piece->setIsPlaced(true);
                           if((side=="WHITE"&&row==6)||(side=="BLACK"&&row==1))
                               piece->firstMove = true;
                           else piece->firstMove = false;
                           addToScene(piece);
                           alivePiece.append(piece);
                        }

            }
            else if(list[0]=="queen")
            {
                            for(int i=1;i<=list[1].toInt();i++)
                            {
                               int row=8-list[1+i][1].digitValue();
                               int col;
                               if(list[1+i][0]=="a")
                                   col=0;
                               if(list[1+i][0]=="b")
                                   col=1;
                               if(list[1+i][0]=="c")
                                   col=2;
                               if(list[1+i][0]=="d")
                                   col=3;
                               if(list[1+i][0]=="e")
                                   col=4;
                               if(list[1+i][0]=="f")
                                   col=5;
                               if(list[1+i][0]=="g")
                                   col=6;
                               if(list[1+i][0]=="h")
                                   col=7;

                               Grid *grid =grids[row][col];
                               Queen *piece =new Queen(side);



                               grid->placePiece(piece);
                               piece->setIsPlaced(true);
                               piece->firstMove = true;   //Pawn处不行
                               addToScene(piece);
                               alivePiece.append(piece);
                            }

            }
            else if(list[0]=="rook")
            {
                                for(int i=1;i<=list[1].toInt();i++)
                                {
                                   int row=8-list[1+i][1].digitValue();
                                   int col;
                                   if(list[1+i][0]=="a")
                                       col=0;
                                   if(list[1+i][0]=="b")
                                       col=1;
                                   if(list[1+i][0]=="c")
                                       col=2;
                                   if(list[1+i][0]=="d")
                                       col=3;
                                   if(list[1+i][0]=="e")
                                       col=4;
                                   if(list[1+i][0]=="f")
                                       col=5;
                                   if(list[1+i][0]=="g")
                                       col=6;
                                   if(list[1+i][0]=="h")
                                       col=7;

                                   Grid *grid =grids[row][col];
                                   Rook *piece=new Rook(side);



                                   grid->placePiece(piece);
                                   piece->setIsPlaced(true);
                                   piece->firstMove = true;   //Pawn处不行
                                   addToScene(piece);
                                   alivePiece.append(piece);
                                }

            }

        }


    }




}
void Info::savefile()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save File",QDir::currentPath());
    QFile *file=new QFile;
      file->setFileName(fileName);
      bool ok=file->open(QIODevice::WriteOnly);
      if(ok)
      {   QStringList side;
          side.clear();
          QTextStream out(file);
          if(this->turn=="WHITE")
              side<<"white"<<"black";
          else side<<"black"<<"white";


          int count[12]={0,0,0,0,0,0,0,0,0,0,0,0};
          QStringList whiteline;
          QStringList blackline;
          whiteline<<""<<""<<""<<""<<""<<"";
          blackline<<""<<""<<""<<""<<""<<"";
          for(int i=0;i<8;i++)
              for(int j=0;j<8;j++)
              {
                  if(grids[i][j]->ifhasPiece())
                  {
                     if(grids[i][j]->nowPiece->getSide()=="WHITE"
                          &&grids[i][j]->nowPiece->name=="King")
                     {
                        count[0]++;
                        whiteline[0]+=" ";
                        if(j==0) whiteline[0]+="a";
                        if(j==1) whiteline[0]+="b";
                        if(j==2) whiteline[0]+="c";
                        if(j==3) whiteline[0]+="d";
                        if(j==4) whiteline[0]+="e";
                        if(j==5) whiteline[0]+="f";
                        if(j==6) whiteline[0]+="g";
                        if(j==7) whiteline[0]+="h";
                        whiteline[0]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="WHITE"
                          &&grids[i][j]->nowPiece->name=="Queen")
                     {
                        count[1]++;
                        whiteline[1]+=" ";
                        if(j==0) whiteline[1]+="a";
                        if(j==1) whiteline[1]+="b";
                        if(j==2) whiteline[1]+="c";
                        if(j==3) whiteline[1]+="d";
                        if(j==4) whiteline[1]+="e";
                        if(j==5) whiteline[1]+="f";
                        if(j==6) whiteline[1]+="g";
                        if(j==7) whiteline[1]+="h";
                        whiteline[1]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="WHITE"
                          &&grids[i][j]->nowPiece->name=="Rook")
                     {
                        count[2]++;
                        whiteline[2]+=" ";
                        if(j==0) whiteline[2]+="a";
                        if(j==1) whiteline[2]+="b";
                        if(j==2) whiteline[2]+="c";
                        if(j==3) whiteline[2]+="d";
                        if(j==4) whiteline[2]+="e";
                        if(j==5) whiteline[2]+="f";
                        if(j==6) whiteline[2]+="g";
                        if(j==7) whiteline[2]+="h";
                        whiteline[2]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="WHITE"
                          &&grids[i][j]->nowPiece->name=="Bishop")
                     {
                        count[3]++;
                        whiteline[3]+=" ";
                        if(j==0) whiteline[3]+="a";
                        if(j==1) whiteline[3]+="b";
                        if(j==2) whiteline[3]+="c";
                        if(j==3) whiteline[3]+="d";
                        if(j==4) whiteline[3]+="e";
                        if(j==5) whiteline[3]+="f";
                        if(j==6) whiteline[3]+="g";
                        if(j==7) whiteline[3]+="h";
                        whiteline[3]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="WHITE"
                          &&grids[i][j]->nowPiece->name=="Knight")
                     {
                        count[4]++;
                        whiteline[4]+=" ";
                        if(j==0) whiteline[4]+="a";
                        if(j==1) whiteline[4]+="b";
                        if(j==2) whiteline[4]+="c";
                        if(j==3) whiteline[4]+="d";
                        if(j==4) whiteline[4]+="e";
                        if(j==5) whiteline[4]+="f";
                        if(j==6) whiteline[4]+="g";
                        if(j==7) whiteline[4]+="h";
                        whiteline[4]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="WHITE"
                          &&grids[i][j]->nowPiece->name=="Pawn")
                     {
                        count[5]++;
                        whiteline[5]+=" ";
                        if(j==0) whiteline[5]+="a";
                        if(j==1) whiteline[5]+="b";
                        if(j==2) whiteline[5]+="c";
                        if(j==3) whiteline[5]+="d";
                        if(j==4) whiteline[5]+="e";
                        if(j==5) whiteline[5]+="f";
                        if(j==6) whiteline[5]+="g";
                        if(j==7) whiteline[5]+="h";
                        whiteline[5]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="BLACK"
                          &&grids[i][j]->nowPiece->name=="King")
                     {
                        count[6]++;
                        blackline[0]+=" ";
                        if(j==0) blackline[0]+="a";
                        if(j==1) blackline[0]+="b";
                        if(j==2) blackline[0]+="c";
                        if(j==3) blackline[0]+="d";
                        if(j==4) blackline[0]+="e";
                        if(j==5) blackline[0]+="f";
                        if(j==6) blackline[0]+="g";
                        if(j==7) blackline[0]+="h";
                        blackline[0]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="BLACK"
                          &&grids[i][j]->nowPiece->name=="Queen")
                     {
                        count[7]++;
                        blackline[1]+=" ";
                        if(j==0) blackline[1]+="a";
                        if(j==1) blackline[1]+="b";
                        if(j==2) blackline[1]+="c";
                        if(j==3) blackline[1]+="d";
                        if(j==4) blackline[1]+="e";
                        if(j==5) blackline[1]+="f";
                        if(j==6) blackline[1]+="g";
                        if(j==7) blackline[1]+="h";
                        blackline[1]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="BLACK"
                          &&grids[i][j]->nowPiece->name=="Rook")
                     {
                        count[8]++;
                        blackline[2]+=" ";
                        if(j==0) blackline[2]+="a";
                        if(j==1) blackline[2]+="b";
                        if(j==2) blackline[2]+="c";
                        if(j==3) blackline[2]+="d";
                        if(j==4) blackline[2]+="e";
                        if(j==5) blackline[2]+="f";
                        if(j==6) blackline[2]+="g";
                        if(j==7) blackline[2]+="h";
                        blackline[2]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="BLACK"
                          &&grids[i][j]->nowPiece->name=="Bishop")
                     {
                        count[9]++;
                        blackline[3]+=" ";
                        if(j==0) blackline[3]+="a";
                        if(j==1) blackline[3]+="b";
                        if(j==2) blackline[3]+="c";
                        if(j==3) blackline[3]+="d";
                        if(j==4) blackline[3]+="e";
                        if(j==5) blackline[3]+="f";
                        if(j==6) blackline[3]+="g";
                        if(j==7) blackline[3]+="h";
                        blackline[3]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="BLACK"
                          &&grids[i][j]->nowPiece->name=="Knight")
                     {
                        count[10]++;
                        blackline[4]+=" ";
                        if(j==0) blackline[4]+="a";
                        if(j==1) blackline[4]+="b";
                        if(j==2) blackline[4]+="c";
                        if(j==3) blackline[4]+="d";
                        if(j==4) blackline[4]+="e";
                        if(j==5) blackline[4]+="f";
                        if(j==6) blackline[4]+="g";
                        if(j==7) blackline[4]+="h";
                        blackline[4]+=QString::number(8-i);
                     }
                     if(grids[i][j]->nowPiece->getSide()=="BLACK"
                          &&grids[i][j]->nowPiece->name=="Pawn")
                     {
                        count[11]++;
                        blackline[5]+=" ";
                        if(j==0) blackline[5]+="a";
                        if(j==1) blackline[5]+="b";
                        if(j==2) blackline[5]+="c";
                        if(j==3) blackline[5]+="d";
                        if(j==4) blackline[5]+="e";
                        if(j==5) blackline[5]+="f";
                        if(j==6) blackline[5]+="g";
                        if(j==7) blackline[5]+="h";
                        blackline[5]+=QString::number(8-i);
                     }


                }
              }

          out<<side[0]<<endl;

          if(getTurn()=="WHITE")
          {for(int i=0;i<6;i++)
            {
              if(count[i]>0)
              {
                  if(i==0)  out<<"king ";
                  if(i==1)  out<<"queen ";
                  if(i==2)  out<<"rook ";
                  if(i==3)  out<<"bishop ";
                  if(i==4)  out<<"knight ";
                  if(i==5)  out<<"pawn ";






                      out<<count[i]<<whiteline[i]<<endl;

              }
            }
          }
          else
          {for(int i=0;i<6;i++)
              {
                if(count[i+6]>0)
                {
                    if(i==0)  out<<"king ";
                    if(i==1)  out<<"queen ";
                    if(i==2)  out<<"rook ";
                    if(i==3)  out<<"bishop ";
                    if(i==4)  out<<"knight ";
                    if(i==5)  out<<"pawn ";






                        out<<count[i+6]<<blackline[i]<<endl;

                }
              }

          }
          out<<side[1]<<endl;
          if(getTurn()=="WHITE")
          {for(int i=0;i<6;i++)
            {
              if(count[i+6]>0)
              {
                  if(i==0)  out<<"king ";
                  if(i==1)  out<<"queen ";
                  if(i==2)  out<<"rook ";
                  if(i==3)  out<<"bishop ";
                  if(i==4)  out<<"knight ";
                  if(i==5)  out<<"pawn ";






                      out<<count[i+6]<<blackline[i]<<endl;

              }
            }
          }
          else
          {for(int i=0;i<6;i++)
              {
                if(count[i]>0)
                {
                    if(i==0)  out<<"king ";
                    if(i==1)  out<<"queen ";
                    if(i==2)  out<<"rook ";
                    if(i==3)  out<<"bishop ";
                    if(i==4)  out<<"knight ";
                    if(i==5)  out<<"pawn ";






                        out<<count[i]<<whiteline[i]<<endl;

                }
              }

          }

          file->close();
          delete file;
      }


}
