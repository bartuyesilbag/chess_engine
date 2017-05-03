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
    char _board[8][8];
    srand(time(NULL));
    b->getBoard(mode);
    if(list->size() EQ 0){
        qDebug() << "Check Mate! White Win!";
    }
TEST:
    int list_size = list->size();
    int v1 = rand() % list_size;
    if(b->boardSimulate(QPoint(list->at(v1).X,list->at(v1).Y),QPoint(list->at(v1).destX,list->at(v1).destY),_board,list->at(v1).flag.castling.cast_stat)){
        b->slot_msg_to_board(QPoint(list->at(v1).X,list->at(v1).Y),QPoint(list->at(v1).destX,list->at(v1).destY),"aaaa");
        list->clearList();
        delete _board;
    }
    goto TEST;
}

bool moveGenerator::calculateBoard(char board[8][8])
{
    for(int h = 0 ; h<8; h++){
        qDebug() << h << 8 - h << " " << board[h][0] << board[h][1] << board[h][2] << board[h][3]
                 << board[h][4] << board[h][5] << board[h][6] << board[h][7];
    }
    qDebug() << "\n      A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
    qDebug() << "\n      0" << "1" << "2" << "3" << "4" << "5" << "6" << "7";
    qDebug() << "--------------------------------------------------------------------";
    return true;
}
