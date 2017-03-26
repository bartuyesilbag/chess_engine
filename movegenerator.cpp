#include "movegenerator.h"
#include "board.h"
#include "safelist.h"
#include "pack.h"
#include "pieces.h"
#include "iostream"

moveGenerator::moveGenerator(safelist<chess_pack> *_list, board *_b): QObject(NULL)
{
    list = _list;
    b    = _b;
}

moveGenerator::~moveGenerator()
{
    qDebug() << "moveGenerator deleted!";
}


void moveGenerator::findLegalMoves(int mode)
{
    b->getBoard(mode);
    int list_size = list->size();
    for(int i = 0; i < list_size; i++){
        list->pop(pack);
        qDebug() << "pack.name:" << pack.name << "pack.x:" << pack.X << "pack.y:" << pack.Y
                 << "pack.destX:" << pack.destX << "pack.destY:" << pack.destY << i;
        char _board[8][8];
        b->boardSimulate(QPoint(pack.X,pack.Y),QPoint(pack.destX,pack.destY),_board);
        calculateBoard(_board);
    }
    b->slot_msg_to_board(QPoint(1,3),QPoint(2,3),"from ai");
}

void moveGenerator::calculateBoard(char board[8][8])
{
        qDebug() << "";
        for(int h = 0 ; h<8; h++){
            qDebug() << 8 - h << " " << board[h][0] << board[h][1] << board[h][2] << board[h][3]
                     << board[h][4] << board[h][5] << board[h][6] << board[h][7];
        }
        qDebug() << "\n    A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";

}
