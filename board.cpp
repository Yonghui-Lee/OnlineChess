#include "board.h"
#include "grid.h"
#include "info.h"
#include "piece.h"

extern Info *info;
Board::Board()
{
    initArray();
}
void Board::drawGrids(int x,int y)
{
    int SHIFT = 100;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++)
        {
            Grid *grid = new Grid();
            info->grids[i][j] = grid;
            grid->rowLoc = i;
            grid->colLoc = j;
            grid->setPos(x+SHIFT*j,y+SHIFT*i);
            if((i+j)%2==0)
                grid->setOriginalColor(QColor(240,218,181));
            else
                grid->setOriginalColor(QColor(181,135,99));
            info->addToScene(grid);



        }
    }

}


void Board::addPiece() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++)
        {

            Grid *grid =info->grids[i][j];
            if(i < 2) {
                static int k;
                grid->placePiece(black[k]);
                info->alivePiece.append(black[k]);
                info->addToScene(black[k++]);
            }
            if(i > 5) {
                static int h;
                grid->placePiece(white[h]);
                info->alivePiece.append(white[h]);
                info->addToScene(white[h++]);
            }

        }
    }
}

void Board::initArray()
{
    Piece *piece;
    for(int i = 0; i < 8; i++) {
        piece = new Pawn("WHITE");
        white.append(piece);
    }
    piece = new Rook("WHITE");
    white.append(piece);
    piece = new Knight("WHITE");
    white.append(piece);
    piece = new Bishop("WHITE");
    white.append(piece);
    piece = new Queen("WHITE");
    white.append(piece);
    piece = new King("WHITE");
    white.append(piece);
    piece = new Bishop("WHITE");
    white.append(piece);
    piece = new Knight("WHITE");
    white.append(piece);
    piece = new Rook("WHITE");
    white.append(piece);



    piece = new Rook("BLACK");
    black.append(piece);
    piece = new Knight("BLACK");
    black.append(piece);
    piece = new Bishop("BLACK");
    black.append(piece);
    piece = new Queen("BLACK");
    black.append(piece);
    piece = new King("BLACK");
    black.append(piece);
    piece = new Bishop("BLACK");
    black.append(piece);
    piece = new Knight("BLACK");
    black.append(piece);
    piece = new Rook("BLACK");
    black.append(piece);
    for(int i = 0; i < 8; i++) {
        piece = new Pawn("BLACK");
        black.append(piece);
    }

}




void Board::reset() {
    int k = 0; int h = 0;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++)
        {

            Grid *grid =info->grids[i][j];
            grid->setPiece(false);
            grid->setColor("NONE");
            grid->nowPiece = NULL;
            if(i < 2) {

                grid->placePiece(black[k]);
               // black[k]->show();
                black[k]->setIsPlaced(true);
                black[k]->firstMove = true;
                 info->addToScene(black[k]);
                info->alivePiece.append(black[k++]);

            }
            if(i > 5) {

                grid->placePiece(white[h]);
              //  white[h]->show();
                white[h]->setIsPlaced(true);
                white[h]->firstMove = true;
                 info->addToScene(white[h]);
                info->alivePiece.append(white[h++]);

            }

        }
    }
}
