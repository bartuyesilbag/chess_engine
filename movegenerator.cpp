#include "movegenerator.h"
#include "board.h"
#include "safelist.h"
#include "pack.h"
#include "pieces.h"
#include "iostream"
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

void moveGenerator::findLegalMoves(int mode)
{
    srand(time(NULL));
    b->getBoard(mode);
    int list_size = list->size();
    int v1 = rand() % list_size;

    qDebug() << v1;
    for(int i = 0; i < list_size; i++){
        list->pop(pack);
        char _board[8][8];
        b->boardSimulate(QPoint(pack.X,pack.Y),QPoint(pack.destX,pack.destY),_board);
        //calculateBoard(_board);
        if(i EQ v1){
            calculateBoard(_board);
            b->slot_msg_to_board(QPoint(pack.X,pack.Y),QPoint(pack.destX,pack.destY),"aaaa");
            list->clearList();
        }
    }
}

void moveGenerator::calculateBoard(char board[8][8])
{
//    qDebug() << "pack.name:" << pack.name << "pack.x:" << pack.X << "pack.y:" << pack.Y
//             << "pack.destX:" << pack.destX << "pack.destY:" << pack.destY;
//    qDebug() << "";
    for(int h = 0 ; h<8; h++){
        qDebug() << h << 8 - h << " " << board[h][0] << board[h][1] << board[h][2] << board[h][3]
                 << board[h][4] << board[h][5] << board[h][6] << board[h][7];
    }
    qDebug() << "\n      A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
    qDebug() << "\n      0" << "1" << "2" << "3" << "4" << "5" << "6" << "7";
    qDebug() << "--------------------------------------------------------------------";
}
