#include "movegenerator.h"
#include "board.h"
#include "safelist.h"
#include "pack.h"
#include "pieces.h"
#include "iostream"
#include <QVector>

#include <cstdlib>
#include <ctime>

moveGenerator::moveGenerator(safelist<chess_pack> *_list, board *_b): QObject(NULL)
{
    list = _list;
    b    = _b;
}

moveGenerator::~moveGenerator()
{
    qDebug() << "moveGenerator deleted!";
}

void moveGenerator::findLegalBlackMoves()
{
    qDebug() << "lol";
    char _board[8][8];
    b->getBoard(1);
    int listSize = list->size();
    for(int i = 0; i < listSize;i++){
        list->pop(pack);
        if(b->boardSimulate(QPoint(pack.X,pack.Y),QPoint(pack.destX,pack.destY),_board,pack.flag.castling.cast_stat)){
            calculateBoard(_board,1);
        }
    }
}

void moveGenerator::find_legal_white_moves()
{

}

void moveGenerator::calculateBoard(char board[8][8], int mode)
{
    if(mode EQ 1){
        for(int h = 0 ; h<8; h++){
            qDebug() << h << 8 - h << " " << board[h][0] << board[h][1] << board[h][2] << board[h][3]
                     << board[h][4] << board[h][5] << board[h][6] << board[h][7];
        }
        qDebug() << "\n      A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
        qDebug() << "\n      0" << "1" << "2" << "3" << "4" << "5" << "6" << "7";
        qDebug() << "--------------------------------------------------------------------";
    }
    else{

    }

    pack.score.push_back(0);
}

void moveGenerator::sort_scores(QVector<int> _score)
{   
    qSort(_score);
}



QString moveGenerator::black_move_to_screen(QPoint ex_p, QPoint new_p)
{
    QString move;
    move  = horizontal[ex_p.y()] ;
    move += verticle[ex_p.x()]   ;

    move += horizontal[new_p.y()];
    move += verticle[new_p.x()]  ;
    return move;
}
